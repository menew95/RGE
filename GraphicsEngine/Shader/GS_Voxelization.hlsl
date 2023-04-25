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
    // �븻���� ���� ���ϰ� ���밪�� ����
    float3 _faceNormal = abs(input[0].normalWS + input[1].normalWS + input[2].normalWS);
    
    // �븻 ���밪�� ���� ū ���� ã��
    uint _maxi = _faceNormal[1] > _faceNormal[0] ? 1 : 0;
    _maxi = _faceNormal[2] > _faceNormal[_maxi] ? 2 : _maxi;

    for (uint i = 0; i < 3; i++)
    {
        GSOutput _output;

        //WS ��ġ ������ voxel������ ��ġ������ ����
        _output.position.xyz = (input[i].posW.xyz - voxel_radiance._gridCenter) * voxel_radiance._dataSizeRCP;

        // �ֿ� �࿡ ������Ŵ �븻�� ���밪�� ���� ũ�ٴ� �ǹ̴� �ش� �࿡�� ���� ������ ���̶�� ��
        // ���� �������� ���� ���̰� ū ���� ã�ƾ���

        [flatten]
        if (_maxi == 0)
        {
            // x���� ���� Ŭ��� zy ���� ���
            _output.position.xyz = _output.position.zyx;
        }
        else if (_maxi == 1)
        {
            // y ���� ���� Ŭ��� xz ���� ���
            _output.position.xyz = _output.position.xzy;
        }

        // �ƴϸ� �״�� xy���� ���
        
        // �׸��� ���� �ػ󵵸�ŭ ������
        _output.position.xy *= voxel_radiance._dataResRCP;
        _output.position.zw = float2(0, 1);

        _output.uv = input[i].uv;
        _output.normalWS = input[i].normalWS;
        _output.positionWS = input[i].posW;

        outputStream.Append(_output);
    }
}