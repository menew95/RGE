#pragma once

#include "../ID3D.h"

namespace RenderCore
{
	class InputLayout
	{
	public:
		InputLayout();
		~InputLayout();

		ID3D::InputLayout* GetInputLayout() const { return mInputLayout.Get(); }
		ID3D::InputLayout** GetInputLayoutRef() { return mInputLayout.GetAddressOf(); }

		void BuildInputLayout(ID3D::Blob* blob, const D3D11_INPUT_ELEMENT_DESC desc[], uint32 count);

	private:
		ComPtr<ID3D::InputLayout> mInputLayout;
	};
}