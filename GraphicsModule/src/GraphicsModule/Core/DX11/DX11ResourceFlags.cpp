#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11ResourceFlags.h"

namespace Graphics
{
    uint32 DXGetBufferBindFlags(long bindFlags)
    {
        uint32 flagsD3D = 0;

        if ((bindFlags & BindFlags::VertexBuffer) != 0)
            flagsD3D |= D3D11_BIND_VERTEX_BUFFER;
        if ((bindFlags & BindFlags::IndexBuffer) != 0)
            flagsD3D |= D3D11_BIND_INDEX_BUFFER;
        if ((bindFlags & BindFlags::ConstantBuffer) != 0)
            flagsD3D |= D3D11_BIND_CONSTANT_BUFFER;
        if ((bindFlags & BindFlags::StreamOutput) != 0)
            flagsD3D |= D3D11_BIND_STREAM_OUTPUT;
        if ((bindFlags & BindFlags::ShaderResource) != 0)
            flagsD3D |= D3D11_BIND_SHADER_RESOURCE;
        if ((bindFlags & BindFlags::UnorderedAccess) != 0)
            flagsD3D |= D3D11_BIND_UNORDERED_ACCESS;

        return flagsD3D;
    }

    uint32 DXGetTextureBindFlags(const TextureDesc& desc)
    {
        uint32 flagsD3D = 0;

        if ((desc._bindFlags & BindFlags::DepthStencil) != 0)
            flagsD3D |= D3D11_BIND_DEPTH_STENCIL;
        else if ((desc._bindFlags & BindFlags::RenderTarget) != 0)
            flagsD3D |= D3D11_BIND_RENDER_TARGET;

        if ((desc._bindFlags & BindFlags::ShaderResource) != 0)
            flagsD3D |= D3D11_BIND_SHADER_RESOURCE;
        if ((desc._bindFlags & BindFlags::UnorderedAccess) != 0)
            flagsD3D |= D3D11_BIND_UNORDERED_ACCESS;

        return flagsD3D;
    }

    bool DXBindFlagsNeedBufferWithRV(long bindFlags)
    {
        return ((bindFlags & (BindFlags::ShaderResource | BindFlags::UnorderedAccess)) != 0);
    }

    uint32 DXGetCPUAccessFlagsForMiscFlags(long miscFlags)
    {
        uint32 flagsD3D = 0;

        if ((miscFlags & MiscFlags::DynamicUsage) != 0)
            flagsD3D |= D3D11_CPU_ACCESS_WRITE;

        return flagsD3D;
    }

    uint32 DXGetCPUAccessFlags(long cpuAccessFlags)
    {
        uint32 flagsD3D = 0;

        if ((cpuAccessFlags & CPUAccessFlags::Read) != 0)
            flagsD3D |= D3D11_CPU_ACCESS_READ;
        if ((cpuAccessFlags & CPUAccessFlags::Write) != 0)
            flagsD3D |= D3D11_CPU_ACCESS_WRITE;

        return flagsD3D;
    }

	uint32 DXGetBufferMiscFlags(const BufferDesc& desc)
	{
		uint32 flagsD3D = 0;

		/*if ((desc._bindFlags & BindFlags::IndirectBuffer) != 0)
			flagsD3D |= D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;*/

		if (IsStructuredBuffer(desc))
			flagsD3D |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		else if (IsByteAddressBuffer(desc))
			flagsD3D |= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

		return flagsD3D;
	}

    uint32 DXGetTextureMiscFlags(const TextureDesc& desc)
    {
        uint32 flagsD3D = 0;

        if (IsMipMappedTexture(desc))
        {
            const long requiredFlags = BindFlags::RenderTarget | BindFlags::ShaderResource;
            const long disallowedFlags = BindFlags::DepthStencil;
            if ((desc._bindFlags & (requiredFlags | disallowedFlags)) == requiredFlags)
                flagsD3D |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
        }

        if (IsCubeTexture(desc._textureType))
            flagsD3D |= D3D11_RESOURCE_MISC_TEXTURECUBE;

        return flagsD3D;
    }

    D3D11_USAGE DXGetBufferUsage(const BufferDesc& desc)
    {
        if ((desc._bindFlags & BindFlags::UnorderedAccess) == 0)
        {
            if ((desc._miscFlags & MiscFlags::DynamicUsage) != 0)
                return D3D11_USAGE_DYNAMIC;
        }
        return D3D11_USAGE_DEFAULT;
    }

    D3D11_USAGE DXGetCPUAccessBufferUsage(const BufferDesc& desc)
    {
        if ((desc._cpuAccessFlags & CPUAccessFlags::Read) != 0)
            return D3D11_USAGE_STAGING;
        if ((desc._cpuAccessFlags & CPUAccessFlags::Write) != 0)
            return D3D11_USAGE_DYNAMIC;
        return D3D11_USAGE_DEFAULT;
    }

    D3D11_USAGE DXGetTextureUsage(const TextureDesc& /*desc*/)
    {
        return D3D11_USAGE_DEFAULT;
    }

    D3D11_MAP DXGetMapWrite(bool writeDiscard)
    {
        return (writeDiscard ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE);
    }
}