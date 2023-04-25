#include "GraphicsEnginePCH.h"

#include "GraphicsEngine/Resource/ResourceManager.h"
#include "GraphicsEngine/RenderPass/Voxel.h"

namespace Graphics
{
	DECLSPEC_ALIGN(16) struct VoxelCBuffer
	{
		DirectX::XMFLOAT3 grid_center;
		float   data_size;        // voxel half-extent in world space units
		float   data_size_rcp;    // 1.0 / voxel-half extent
		uint32    data_res;         // voxel grid resolution
		float   data_res_rcp;     // 1.0 / voxel grid resolution
		uint32    num_cones;
		float   num_cones_rcp;
		float   max_distance;
		float   ray_step_size;
		uint32    mips;
	};

	Voxel::Voxel(CommandBuffer* command, ResourceManager* resourceManager)
		: m_CommandBuffer(command)
		, m_ResourceManager(resourceManager)
	{
		Initialize();
	}

	Voxel::~Voxel()
	{

	}

	void Voxel::Excute()
	{

	}

	//struct GfxBufferDesc
	//{
	//	uint64 size = 0;
	//	GfxResourceUsage resource_usage = GfxResourceUsage::Default;
	//	GfxCpuAccess cpu_access = GfxCpuAccess::None;
	//	GfxBindFlag bind_flags = GfxBindFlag::None;
	//	GfxBufferMiscFlag misc_flags = GfxBufferMiscFlag::None;
	//	uint32 stride = 0; //structured buffers, (vertex buffers, index buffers, needed for count calculation not for srv as structured buffers)
	//	GfxFormat format = GfxFormat::UNKNOWN; //typed buffers, index buffers
	//	std::strong_ordering operator<=>(GfxBufferDesc const& other) const = default;
	//};

	//template<typename T>
	//static GfxBufferDesc StructuredBufferDesc(uint64 count, bool uav = true, bool dynamic = false)
	//{
	//	GfxBufferDesc desc{};
	//	desc.resource_usage = (uav || !dynamic) ? GfxResourceUsage::Default : GfxResourceUsage::Dynamic;
	//	desc.bind_flags = GfxBindFlag::ShaderResource;
	//	if (uav) desc.bind_flags |= GfxBindFlag::UnorderedAccess;
	//	desc.misc_flags = GfxBufferMiscFlag::BufferStructured;
	//	desc.cpu_access = !dynamic ? GfxCpuAccess::None : GfxCpuAccess::Write;
	//	desc.stride = sizeof(T);
	//	desc.size = desc.stride * count;
	//	return desc;
	//}

	void Voxel::CreateVoxelResource()
	{
		BufferDesc _bufferDesc{};
		
		//voxels = std::make_unique<Buffer>(StructuredBufferDesc<VoxelType>(VOXEL_RESOLUTION * VOXEL_RESOLUTION * VOXEL_RESOLUTION));

		_bufferDesc._stride = sizeof(VoxelType);
		_bufferDesc._size = _bufferDesc._stride * VOXEL_RESOLUTION * VOXEL_RESOLUTION * VOXEL_RESOLUTION;
		_bufferDesc._bindFlags = BindFlags::ShaderResource | BindFlags::UnorderedAccess;
		_bufferDesc._miscFlags = MiscFlags::DynamicUsage;
		//_bufferDesc._cpuAccessFlags = CPUAccess::ReadWrite;

		m_Voxel = m_ResourceManager->CreateBuffer(TEXT("VoxelBuffer"), _bufferDesc);

		TextureDesc _textureDesc;

		_textureDesc._textureType = TextureType::Texture3D;
		_textureDesc._extend = { VOXEL_RESOLUTION, VOXEL_RESOLUTION , VOXEL_RESOLUTION };
		_textureDesc._miscFlags = MiscFlags::GenerateMips;
		_textureDesc._mipLevels = 1;
		_textureDesc._bindFlags = BindFlags::UnorderedAccess | BindFlags::ShaderResource | BindFlags::RenderTarget;
		_textureDesc._format = Format::R16G16B16A16_FLOAT;

		m_VoxelTexture = m_ResourceManager->CreateTexture(TEXT("VoxelTexture"), _textureDesc);
		m_VoxelBoundTexture = m_ResourceManager->CreateTexture(TEXT("VoxelBoundTexture"), _textureDesc);
	}

	void Voxel::CreateVoxelizePass()
	{
		ShaderDesc _voxelCopyDesc;
		{
			_voxelCopyDesc._shaderType = ShaderType::Compute;
			_voxelCopyDesc._sourceType = ShaderSourceType::HLSL;
			_voxelCopyDesc._filePath = TEXT("Asset\\Shader\\CS_VoxelCopy.hlsl");
			_voxelCopyDesc._sourceSize = 0;
			_voxelCopyDesc._entryPoint = "main";
			_voxelCopyDesc._profile = "cs_5_0";
		}

		auto* _copy = m_ResourceManager->CreateShader(TEXT("VoxelCopy"), _voxelCopyDesc);

		PipelineLayoutDesc _voxelCopyLayoutDesc;
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Buffer, BindFlags::ConstantBuffer, StageFlags::CS, 4
			};

			_voxelCopyLayoutDesc._bindings.push_back(_bindDesc);
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Buffer, BindFlags::UnorderedAccess, StageFlags::CS, 0
			};

			_voxelCopyLayoutDesc._bindings.push_back(_bindDesc);
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Texture, BindFlags::UnorderedAccess, StageFlags::CS, 1
			};

			_voxelCopyLayoutDesc._bindings.push_back(_bindDesc);
		}

		m_VoxelizeLayout = m_ResourceManager->CreatePipelineLayout(TEXT("VoxelCopy"), _voxelCopyLayoutDesc);

		ComputePipelineDesc _voxelCSODesc;
		_voxelCSODesc.pipelineLayout = m_VoxelizeLayout;
		_voxelCSODesc._shaderProgram._computeShader = _copy;

		m_VoxelCopyCSO = m_ResourceManager->CreatePipelineState(TEXT("VoxelCopy"), _voxelCSODesc);
	}

	void Voxel::RegistRenderObject(RenderObject* renderObject)
	{
		m_RenderObjectList.push_back(renderObject);
	}

	void Voxel::Initialize()
	{
		CreateVoxelResource();

		CreateVoxelizePass();
	}

}