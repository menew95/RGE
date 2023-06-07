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
    // �븻���� ���� ���ϰ� ���밪�� ����
    float3 _faceNormal = abs(input[0].normal + input[1].normal + input[2].normal);
    
    // �븻 ���밪�� ���� ū ���� ã��
    uint _maxi = _faceNormal[1] > _faceNormal[0] ? 1 : 0;
    _maxi = _faceNormal[2] > _faceNormal[_maxi] ? 2 : _maxi;

    [unroll(3)]
    for (uint i = 0; i < 3; i++)
    {
        GSOutput _output;

        // WS ��ġ ������ ���������� �������κ����� �Ÿ��� ���ϰ� ���� ������ ��ŭ ������
        // ���� ������ ������ ��ȯ 
        _output.posH.xyz = (input[i].posW.xyz - voxel_radiance._gridCenter) * voxel_radiance._dataSizeRCP;

        // �ֿ� �࿡ ������Ŵ �븻�� ���밪�� ���� ũ�ٴ� �ǹ̴� �ش� �࿡�� ���� ������ ���̶�� ��
        // ���� �������� ���� ���̰� ū ���� ã�ƾ���

        [flatten]
        if (_maxi == 0)
        {
            // x���� ���� Ŭ��� zy ���� ���
            _output.posH.xyz = _output.posH.zyx;
        }
        else if (_maxi == 1)
        {
            // y ���� ���� Ŭ��� xz ���� ���
            _output.posH.xyz = _output.posH.xzy;
        }

        // �ƴϸ� �״�� xy���� ���
        
        // �׸��� ���� �ػ󵵸�ŭ ������
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