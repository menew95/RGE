﻿/**

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
	class RenderQueue;

	class RenderQueueManager
	{
		SINGTON(RenderQueueManager)
	public:

		RenderQueueManager();
		~RenderQueueManager();

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

		/**
			@brief 들어온 RenderQueue를 이번 프레임에 렌더링 되어야할 큐를 채워줌 
			@param renderQueue - 큐를 생성할 텐더큐 레퍼런스
		**/
		void CreateRenderQueue(RenderQueue& renderQueue);

		/**
		    @brief 렌더 큐 상태를 업데이트 해줌
		    @param camBuffer - 렌더큐를 업데이트 할 카메라 버퍼
		**/
		void UpdateRenderQueue(CameraBuffer* camBuffer);

	private:
		std::vector<std::shared_ptr<RenderObject>> m_RenderObjectContainer;

		RenderQueue m_OpaqueMeshRenderQueue;

		RenderQueue m_TransparentMeshRenderQueue;

		RenderQueue 
	};
}
