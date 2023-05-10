#include "Header/H_Input.hlsli"
#include "Header/H_ConstBuffer.hlsli"

Texture2D depthBuffer : register(t0);
Texture2D normalBuffer: register(t1);

// MIN_MAG_MIP_POINT
SamplerState samPoint : register(s0);

struct VertexOut
{
    float4 posH : SV_POSITION;
    //float2 posH : POSITION;
    float3 viewRay : VIEWRAY;
    float2 uv : TEXCOORD;
};

float distanceSquared(float2 a, float2 b)
{
    a -= b;
    return dot(a, a);
}

bool intersectsDepthBuffer(float z, float minZ, float maxZ)
{
    /*
     * Based on how far away from the camera the depth is,
     * adding a bit of extra thickness can help improve some
     * artifacts. Driving this value up too high can cause
     * artifacts of its own.
     */

     //Originally:
     //	depthThicknessDynamicBias = clamp( (z + biasStart) * biasRange, 0, 1 ) * biasAmount;
     //	= clamp( (z - biasStart) * biasRange * biasAmount, 0, biasAmount );
     //	= clamp( (z - biasStart) * biasRangeTimesAmount, 0, biasAmount );
     //	= clamp( (z * biasRangeTimesAmount + -biasStartTimesRangeTimesAmount), 0, biasAmount );
     //The goal is to preserver precision as biasRange may be too small,
     //and to allow fmad optimization.
    float depthScale = min(1.0f, z * _strideZCutoff);
    z += _zThickness + lerp(0.0f, 2.0f, depthScale);
    return (maxZ >= z) && (minZ - _zThickness <= z);
}

void swap(inout float a, inout float b)
{
    float t = a;
    a = b;
    b = t;
}


float linearizeDepth(float depth)
{
    //float z = depth * 2.0 - 1.0; // back to NDC 
    //return (2.0 * camera._near * camera._far) / (camera._far + camera._near - depth * (camera._far - camera._near));

   /* float A = camera._far / (camera._far - camera._near);
    float B = (-camera._far * camera._near) / (camera._far - camera._near);

    return B / (depth - A);*/

    // A = f / (f - n);
    // B = -f * n / (f - n)
    // B / (d - A) = f * n / (f - d * (f - n)) = f * n / (f + d * (n - f))
    return camera._near * camera._far / (camera._far + depth * (camera._near - camera._far));
}

float linearDepthTexelFetch(int2 hitPixel)
{
    // Load returns 0 for any value accessed out of bounds
    return linearizeDepth(depthBuffer.Load(int3(hitPixel, 0)).r);
}

// Returns true if the ray hit something
bool traceScreenSpaceRay(
    // Camera-space ray origin, which must be within the view volume
    float3 csOrig,
    // Unit length camera-space ray direction
    float3 csDir,
    // Number between 0 and 1 for how far to bump the ray in stride units
    // to conceal banding artifacts. Not needed if stride == 1.
    float jitter,
    // Pixel coordinates of the first intersection with the scene
    out float2 hitPixel,
    // Camera space location of the ray hit
    out float3 hitPoint)
{
    // Clip to the near plane
    float rayLength = ((csOrig.z + csDir.z * _maxDistance) < _nearPlaneZ) ?
    (_nearPlaneZ - csOrig.z) / csDir.z : _maxDistance;
    float3 csEndPoint = csOrig + csDir * rayLength;

    // Project into homogeneous clip space
    float4 H0 = mul(float4(csOrig, 1.0f), camera._viewToTextureSpaceMatrix);
    H0.xy *= _depthBufferSize;
    float4 H1 = mul(float4(csEndPoint, 1.0f), camera._viewToTextureSpaceMatrix);
    H1.xy *= _depthBufferSize;
    float k0 = 1.0f / H0.w;
    float k1 = 1.0f / H1.w;

    // The interpolated homogeneous version of the camera-space points
    float3 Q0 = csOrig * k0;
    float3 Q1 = csEndPoint * k1;

    // Screen-space endpoints
    float2 P0 = H0.xy * k0;
    float2 P1 = H1.xy * k1;

    // If the line is degenerate, make it cover at least one pixel
    // to avoid handling zero-pixel extent as a special case later
    P1 += (distanceSquared(P0, P1) < 0.0001f) ? float2(0.01f, 0.01f) : 0.0f;
    float2 delta = P1 - P0;

    // Permute so that the primary iteration is in x to collapse
    // all quadrant-specific DDA cases later
    bool permute = false;
    if(abs(delta.x) < abs(delta.y))
    {
        // This is a more-vertical line
        permute = true;
        delta = delta.yx;
        P0 = P0.yx;
        P1 = P1.yx;
    }

    float stepDir = sign(delta.x);
    float invdx = stepDir / delta.x;

    // Track the derivatives of Q and k
    float3 dQ = (Q1 - Q0) * invdx;
    float dk = (k1 - k0) * invdx;
    float2 dP = float2(stepDir, delta.y * invdx);

    // Scale derivatives by the desired pixel stride and then
    // offset the starting values by the jitter fraction
    float strideScale = 1.0f - min(1.0f, csOrig.z * _strideZCutoff);
    float stride = 1.0f + strideScale * _stride;
    dP *= stride;
    dQ *= stride;
    dk *= stride;

    P0 += dP * jitter;
    Q0 += dQ * jitter;
    k0 += dk * jitter;

    // Slide P from P0 to P1, (now-homogeneous) Q from Q0 to Q1, k from k0 to k1
    float4 PQk = float4(P0, Q0.z, k0);
    float4 dPQk = float4(dP, dQ.z, dk);
    float3 Q = Q0; 

    // Adjust end condition for iteration direction
    float end = P1.x * stepDir;

    float stepCount = 0.0f;
    float prevZMaxEstimate = csOrig.z;
    float rayZMin = prevZMaxEstimate;
    float rayZMax = prevZMaxEstimate;
    float sceneZMax = rayZMax + 100.0f;
    for(;
        ((PQk.x * stepDir) <= end) && (stepCount < _maxSteps) &&
        !intersectsDepthBuffer(sceneZMax, rayZMin, rayZMax) &&
        (sceneZMax != 0.0f);
        ++stepCount)
    {
        rayZMin = prevZMaxEstimate;
        rayZMax = (dPQk.z * 0.5f + PQk.z) / (dPQk.w * 0.5f + PQk.w);
        prevZMaxEstimate = rayZMax;
        if(rayZMin > rayZMax)
        {
            swap(rayZMin, rayZMax);
        }

        hitPixel = permute ? PQk.yx : PQk.xy;
        // You may need hitPixel.y = depthBufferSize.y - hitPixel.y; here if your vertical axis
        // is different than ours in screen space
        sceneZMax = linearDepthTexelFetch(int2(hitPixel));

        PQk += dPQk;
    }

    // Advance Q based on the number of steps
    Q.xy += dQ.xy * stepCount;
    hitPoint = Q * (1.0f / PQk.w);
    return intersectsDepthBuffer(sceneZMax, rayZMin, rayZMax);
}

float4 main(VertexOut input) : SV_TARGET
{
    // 해당 픽셀의 세계 공간 노말값
    float3 normal = normalBuffer.Sample(samPoint, input.uv).xyz;

    if (!any(normal))
    {
        return 0.0f;
    }

    normal = normal * 2.0f - 1.0f;

    // 시야 공간의 노말값
    float3 normalVS = mul(float4(normal, 0.0f), camera._view).xyz;

    if(!any(normalVS))
    {
        return 0.0f;
    }

    // 해당 픽셀의 깊이값
    float depth = depthBuffer.Sample(samPoint, input.uv).r;

    // 카메라에서 쏜 레이 uv 좌표를 프로젝션 인버스를 곱해서 카메라 공간으로 옮긴 시야 공간의 광선
    float3 rayOriginVS = input.viewRay * linearizeDepth(depth);

    /*
     * Since position is reconstructed in view space, just normalize it to get the
     * vector from the eye to the position and then reflect that around the normal to
     * get the ray direction to trace.
     */
    
     // 정규화된 시야 공간 광선
    float3 toPositionVS = normalize(rayOriginVS);
    // 시야 공간 광선을 시야 공간 노말 값에 반사 시켜 구한 시야 공간 반사 광선 방향
    float3 rayDirectionVS = normalize(reflect(toPositionVS, normalVS)); 

    // output rDotV to the alpha channel for use in determining how much to fade the ray
    // 시야 공간 반사광선의 방향과 시야 공간의 광선의 내적
    float rDotV = dot(rayDirectionVS, toPositionVS);

    // out parameters
    float2 hitPixel = float2(0.0f, 0.0f);
    float3 hitPoint = float3(0.0f, 0.0f, 0.0f);

    // 
    //float jitter = _stride > 1.0f ? float(int(input.posH.x + input.posH.y) & 1) * 0.5f : 0.0f;
    float jitter = _stride > 1.0f ? float(int(input.viewRay.x + input.viewRay.y) & 1) * 0.5f : 0.0f;

    // perform ray tracing - true if hit found, false otherwise
    bool intersection = traceScreenSpaceRay(rayOriginVS, rayDirectionVS, jitter, hitPixel, hitPoint);

    depth = depthBuffer.Load(int3(hitPixel, 0)).r;

    // move hit pixel from pixel position to UVs
    hitPixel *= float2(1.f / 1280.f, 1.f / 720.f);
    if(hitPixel.x > 1.0f || hitPixel.x < 0.0f || hitPixel.y > 1.0f || hitPixel.y < 0.0f)
    {
        intersection = false;
    }

    return float4(hitPixel, depth, rDotV) * (intersection ? 1.0f : 0.0f);
}