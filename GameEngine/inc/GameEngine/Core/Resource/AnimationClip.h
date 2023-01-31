#pragma once

#include "Common.h"

#include "GameEngine/Core/Resource/Resource.h"

namespace GameEngine
{
	namespace Core
	{
		/**
		* \brief 게임 오브젝트의 움직임의 한 프레임
		*/
		struct KeyFrame
		{
			KeyFrame() = default;
			KeyFrame(float rate, Math::Vector3 data)
				: m_FrameRate{ rate }
				, m_Data{ data }
			{}
			KeyFrame(float rate, Math::Quaternion data)
				: m_FrameRate{ rate }
				, m_Data{ data }
			{}
			float m_FrameRate;
			Math::Vector4 m_Data;
		};

		/**
		* \brief 프레임 관련 정보를 담고있는 컨테이너
		*/
		struct Frame
		{
			float _frameRate = 0;
			uint32 _posFrame = 0;
			uint32 _rotFrame = 0;
			uint32 _scaleFrame = 0;
		};

		/**
		* \brief 한 게임 오브젝트의 움직임
		*/
		class AnimationSnap
		{
		public:
			AnimationSnap();
			virtual ~AnimationSnap();

		public:


			enum eKeyType
			{
				Position,
				Rotate,
				Scale,
			};


			/**
			 * \brief type 에 따라 해당하는 컨테이너에 key frame 을 push
			 * @param keyFrame 키프레임 데이터
			 * @param type 데이터의 타입
			 */
			void AddKeyFrame(KeyFrame keyFrame, eKeyType type);

			Math::Matrix GetTransform(Frame& frame) const;

			/**
			 * \brief 애니메이션 초기화시 타겟될 게임 오브젝트를 찾기 위한 값 게임 오브젝트의 이름과 같은 값을 타겟 해준다.
			 */
			tstring m_TargetName;

			/**
			 * \brief 이 스냅이 담고있는 애니메이션의 최대 길이 값
			 */
			float m_MaxFrameRate = 0;

			/**
			 * \brief Transform Animation 를 담고 있는 컨테이너
			 */
			std::vector<KeyFrame> m_PosKeyFrameList;
			std::vector<KeyFrame> m_RotKeyFrameList;;
			std::vector<KeyFrame> m_ScaleKeyFrameList;
		};

		class GAME_ENGINE_API AnimationClip
		{
		public:
			AnimationClip() {}
			AnimationClip(const tstring& clipName)
				: m_ClipName(clipName)
			{}
			AnimationClip(const tstring& clipName, std::vector<AnimationSnap>& snapList)
				: m_ClipName(clipName)
				, m_AnimationSnapList(snapList)
			{}
			~AnimationClip() {}

			Math::Matrix GetTransform(Frame& frame, uint32 snapIndex) const;

			inline bool operator==(const tstring& value);

			const tstring& GetClipName() const { return m_ClipName; }
			void SetClipName(const tstring& value) { m_ClipName = value; }

			void AddAniamtionSnap(AnimationSnap& newSnap) { m_AnimationSnapList.emplace_back(newSnap); }
			AnimationSnap& GetAnimationSnap(uint32 index) { assert(index < m_AnimationSnapList.size());  return m_AnimationSnapList[index]; }
			uint32 GetAnimationSnapCount() const { return (uint32)m_AnimationSnapList.size(); }

			const std::vector<AnimationSnap>& GetSnapList() const { return m_AnimationSnapList; }

		private:
			/**
			 * \brief frame 에 담긴 frameRate에 따라 이전 프레임과 다음 프레임 사이를 보간 시켜 gameObject에 적용 시켜준다.
			 * @param m_ClipName 크립의 이름
			 */
			tstring m_ClipName;

			// 게임 오브젝트 갯수 만큼 스냅 데이터가 있음
			std::vector<AnimationSnap> m_AnimationSnapList;
		};
	}
}