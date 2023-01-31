#pragma once

#include "Common.h"

#include "GameEngine/Core/Resource/Resource.h"

namespace GameEngine
{
	namespace Core
	{
		/**
		* \brief ���� ������Ʈ�� �������� �� ������
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
		* \brief ������ ���� ������ ����ִ� �����̳�
		*/
		struct Frame
		{
			float _frameRate = 0;
			uint32 _posFrame = 0;
			uint32 _rotFrame = 0;
			uint32 _scaleFrame = 0;
		};

		/**
		* \brief �� ���� ������Ʈ�� ������
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
			 * \brief type �� ���� �ش��ϴ� �����̳ʿ� key frame �� push
			 * @param keyFrame Ű������ ������
			 * @param type �������� Ÿ��
			 */
			void AddKeyFrame(KeyFrame keyFrame, eKeyType type);

			Math::Matrix GetTransform(Frame& frame) const;

			/**
			 * \brief �ִϸ��̼� �ʱ�ȭ�� Ÿ�ٵ� ���� ������Ʈ�� ã�� ���� �� ���� ������Ʈ�� �̸��� ���� ���� Ÿ�� ���ش�.
			 */
			tstring m_TargetName;

			/**
			 * \brief �� ������ ����ִ� �ִϸ��̼��� �ִ� ���� ��
			 */
			float m_MaxFrameRate = 0;

			/**
			 * \brief Transform Animation �� ��� �ִ� �����̳�
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
			 * \brief frame �� ��� frameRate�� ���� ���� �����Ӱ� ���� ������ ���̸� ���� ���� gameObject�� ���� �����ش�.
			 * @param m_ClipName ũ���� �̸�
			 */
			tstring m_ClipName;

			// ���� ������Ʈ ���� ��ŭ ���� �����Ͱ� ����
			std::vector<AnimationSnap> m_AnimationSnapList;
		};
	}
}