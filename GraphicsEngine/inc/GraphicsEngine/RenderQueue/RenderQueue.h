/**

    @file      RenderQueue.h
	@brief     Render Queue
	@details   ���� ������Ʈ���� ����Ʈ �����͵��� ���� �� ���� ť�� ��� �ϴ� Ŭ����
    @author    LWT
    @date      7.06.2023

**/
#pragma once

#pragma once

#include "Common.h"

#include "GraphicsEngine/RenderObject.h"

namespace Graphics
{
	class Deferred;
	class Light;
	class Voxel;

	class RenderQueue
	{
	public:
		RenderQueue();
		~RenderQueue();

		/**
		    @brief  ���� ������Ʈ�� ������ �����̳ʿ� �ְ� �����͸� ��ȯ
		    @retval  - ������ ���ο� ���� ������Ʈ
		**/
		RenderObject* CreateRenderObject();

		/**
		    @brief  ���� ������Ʈ ����
		    @param  renderObject - ������ ���� ������Ʈ�� Raw ������
		    @retval              - ���� ���� ����
		**/
		bool RemoveRenderObject(RenderObject* renderObject);

		/**
			@brief Deferred Pass�� �������� ��ü ���
			@param deferred -
		**/
		void RegistDeferredPass(Deferred* deferred);

		/**
		 @brief Shadow Pass�� �������� ��ü ���
		 @param light - 
		**/
		void RegistShadowPass(Light* light);

		/**
		    @brief Voxel Pass�� �������� ��ü ���
		    @param voxel - 
		**/
		void RegistVoxelPass(Voxel* voxel);

	private:
		std::vector<std::shared_ptr<RenderObject>> m_RenderObjectContainer;
	};
}
