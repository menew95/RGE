#include "../../pch.h"
#include "IncludeDX11.h"
#include "InputLayout.h"
#include "ObjectFactory.h"

RenderCore::InputLayout::InputLayout()
{

}

RenderCore::InputLayout::~InputLayout()
{

}

void RenderCore::InputLayout::BuildInputLayout(ID3D::Blob* blob, const D3D11_INPUT_ELEMENT_DESC desc[], uint32 count)
{
	/*mInputLayout
		= ObjectFactory::CreateInputLayout(desc, count, blob->GetBufferPointer(), blob->GetBufferSize());*/
}
