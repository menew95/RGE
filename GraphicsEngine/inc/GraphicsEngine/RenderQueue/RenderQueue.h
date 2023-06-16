/**

    @file      RenderQueue.h
	@brief     Render Queue
	@details   렌더 오브젝트들의 스마트 포인터들을 관리 및 렌더 큐에 등록 하는 클래스
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
		    @brief  렌더 오브젝트를 생성후 컨테이너에 넣고 포인터를 반환
		    @retval  - 생성된 새로운 렌더 오브젝트
		**/
		RenderObject* CreateRenderObject();

		/**
		    @brief  렌더 오브젝트 삭제
		    @param  renderObject - 삭제할 렌더 오브젝트의 Raw 포인터
		    @retval              - 삭제 성공 여부
		**/
		bool RemoveRenderObject(RenderObject* renderObject);

		/**
			@brief Deferred Pass에 렌더링할 객체 등록
			@param deferred -
		**/
		void RegistDeferredPass(Deferred* deferred);

		/**
		 @brief Shadow Pass에 렌더링할 객체 등록
		 @param light - 
		**/
		void RegistShadowPass(Light* light);

		/**
		    @brief Voxel Pass에 렌더링할 객체 등록
		    @param voxel - 
		**/
		void RegistVoxelPass(Voxel* voxel);

	private:

		std::vector<std::shared_ptr<RenderObject>> m_RenderObjectContainer;
	};
}
