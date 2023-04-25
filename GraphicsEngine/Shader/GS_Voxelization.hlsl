#include "Header/H_Voxel.hlsli"

cbuffer voxelBuffer :register(b1)
{
    VoxelRadiance voxel_radiance;
}
struct GSInput
{
    float4 posW		: POSITION;
    float2 uv		: TEXCOORD;
    float3 normalWS	: NORMAL;
};

struct GSOutput
{
    float4 position     : SV_POSITION;
    float4 positionWS   : POSITION;
    float2 uv           : TEXCOORD;
    float3 normalWS     : NORMAL;
};

[maxvertexcount(3)]
void main(triangle GSInput input[3] : SV_POSITION, inout TriangleStream<GSOutput> outputStream)
{
    // 노말값을 전부 더하고 절대값을 구함
    float3 _faceNormal = abs(input[0].normalWS + input[1].normalWS + input[2].normalWS);
    
    // 노말 절대값중 제일 큰 값을 찾음
    uint _maxi = _faceNormal[1] > _faceNormal[0] ? 1 : 0;
    _maxi = _faceNormal[2] > _faceNormal[_maxi] ? 2 : _maxi;

    for (uint i = 0; i < 3; i++)
    {
        GSOutput _output;

        //WS 위치 값에서 voxel공간의 위치값으로 변경
        _output.position.xyz = (input[i].posW.xyz - voxel_radiance._gridCenter) * voxel_radiance._dataSizeRCP;

        // 주요 축에 투영시킴 노말의 절대값이 제일 크다는 의미는 해당 축에서 제일 수직인 값이라는 뜻
        // 투영 시켰을때 제일 넓이가 큰 축을 찾아야함

        [flatten]
        if (_maxi == 0)
        {
            // x축이 제일 클경우 zy 값을 사용
            _output.position.xyz = _output.position.zyx;
        }
        else if (_maxi == 1)
        {
            // y 축이 제일 클경우 xz 값을 사용
            _output.position.xyz = _output.position.xzy;
        }

        // 아니면 그대로 xy값을 사용
        
        // 그리고 복셀 해상도만큼 나눠줌
        _output.position.xy *= voxel_radiance._dataResRCP;
        _output.position.zw = float2(0, 1);

        _output.uv = input[i].uv;
        _output.normalWS = input[i].normalWS;
        _output.positionWS = input[i].posW;

        outputStream.Append(_output);
    }
}