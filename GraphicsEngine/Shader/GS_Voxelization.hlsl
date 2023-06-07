#include "Header/H_Voxel.hlsli"

struct GSInput
{
    float4 posW		: POSITION;
    float4 color	: COLOR;
    float2 uv		: TEXCOORD;
    float3 normal	: NORMAL;

#if defined(_NORMAL_MAP)
    float3 tangent : TANGENT;
#endif
};

struct GSOutput
{
    float4 posH     : SV_POSITION;
    float4 posW     : POSITION1;
    float4 color	: COLOR;
    float2 uv       : TEXCOORD;
    float3 normal   : NORMAL;

#if defined(_NORMAL_MAP)
    float3 tangent	: TANGENT;
#endif
};

[maxvertexcount(3)]
void main(triangle GSInput input[3] : SV_POSITION, inout TriangleStream<GSOutput> outputStream)
{
    // 노말값을 전부 더하고 절대값을 구함
    float3 _faceNormal = abs(input[0].normal + input[1].normal + input[2].normal);
    
    // 노말 절대값중 제일 큰 값을 찾음
    uint _maxi = _faceNormal[1] > _faceNormal[0] ? 1 : 0;
    _maxi = _faceNormal[2] > _faceNormal[_maxi] ? 2 : _maxi;

    [unroll(3)]
    for (uint i = 0; i < 3; i++)
    {
        GSOutput _output;

        // WS 위치 값에서 복셀공간의 중점으로부터의 거리를 구하고 복셀 사이즈 만큼 나눠줌
        // 복셀 공간의 점으로 변환 
        _output.posH.xyz = (input[i].posW.xyz - voxel_radiance._gridCenter) * voxel_radiance._dataSizeRCP;

        // 주요 축에 투영시킴 노말의 절대값이 제일 크다는 의미는 해당 축에서 제일 수직인 값이라는 뜻
        // 투영 시켰을때 제일 넓이가 큰 축을 찾아야함

        [flatten]
        if (_maxi == 0)
        {
            // x축이 제일 클경우 zy 값을 사용
            _output.posH.xyz = _output.posH.zyx;
        }
        else if (_maxi == 1)
        {
            // y 축이 제일 클경우 xz 값을 사용
            _output.posH.xyz = _output.posH.xzy;
        }

        // 아니면 그대로 xy값을 사용
        
        // 그리고 복셀 해상도만큼 나눠줌
        _output.posH.xy *= voxel_radiance._dataResRCP;
        _output.posH.zw = float2(1, 1);

        _output.posW = input[i].posW;
        _output.color = input[i].color;
        _output.uv = input[i].uv;
        _output.normal = input[i].normal;

#if defined(_NORMAL_MAP)
        _output.tangent = input[i].tangent;
#endif

        outputStream.Append(_output);
    }
}