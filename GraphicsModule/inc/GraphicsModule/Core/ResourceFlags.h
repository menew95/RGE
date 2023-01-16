#pragma once

namespace Graphics
{
    enum class ResourceType
    {
        Undefined,
        Buffer,
		Texture,
		Sampler,
	};

	struct BindFlags
	{
		enum eBindFlags
		{
			VertexBuffer    = (1 << 0),
			IndexBuffer     = (1 << 1),
			ConstantBuffer  = (1 << 2),
            ShaderResource  = (1 << 3),
            StreamOutput    = (1 << 4),
            RenderTarget    = (1 << 5),
            DepthStencil    = (1 << 6),
            UnorderedAccess = (1 << 7),
            Decoder         = (1 << 8),
            VideoEncoder    = (1 << 9),
        };
    };

    struct CPUAccessFlags
    {
        enum eCPUAccessFlags
        {
            Read = (1 << 0),
            Write = (1 << 1),
            ReadWrite = (Read | Write),
        };
    };

    struct MiscFlags
	{
		enum eMiscFlags
		{
			DynamicUsage = (1 << 0),
			FixedSamples = (1 << 1),
			GenerateMips = (1 << 2),
			NoInitialData = (1 << 3),
			Append = (1 << 4),
			Counter = (1 << 5),
		};
	};
}