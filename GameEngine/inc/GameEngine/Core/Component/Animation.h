#pragma once

#include "GameEngine/Core/Component/Component.h"

#include "GameEngine/Core/Resource/AnimationClip.h"

namespace GameEngine
{
	namespace Core
	{
		class AnimationClip;;

		class GAME_ENGINE_API Animation : public Component, public std::enable_shared_from_this<Animation>
		{
		public:
			Animation(std::shared_ptr<class GameObject>& gameObject, const tstring& type = TEXT("Animation"));
			~Animation() override;

			//////////////////////////////////////////////////////////////////////////
			// Inherited Component
			//////////////////////////////////////////////////////////////////////////
			//void PreTick() override;

			void Update() override;

			//void PostTick() override;

			//void FixedTick() override;

			void Start() override;

			//void OnDestroy() override;

			//void OnChangeEnable(bool _newEnable) override;

			//////////////////////////////////////////////////////////////////////////
			// Member Function
			//////////////////////////////////////////////////////////////////////////

			/// @brief ���� �ִϸ��̼� Ŭ���� �����Ǹ� �ִϸ��̼��� ���� ���� �ش�.
			void Play();

			/// @brief ���� �̸��� �´� Ŭ���� ������ ���� ��� �ִϸ��̼��� ���� ���� �ش�.
			/// @param name Ŭ�� �̸�
			void Play(const tstring& name);

			/// @brief �ִϸ��̼��� ���߰� ��� Ÿ�ٿ�����Ʈ�� �ʱ� ���·� �ǵ��� �ش�.
			void Stop();

			std::shared_ptr<AnimationClip> GetAnimationClip(uint32 index);
			std::shared_ptr<AnimationClip> GetAnimationClip(const tstring& clipName);

			Frame& GetFrame() { return m_Frame; }
			bool GetIsPlaying() { return m_IsPlaying; }

			uint32 GetAnimationClipCount() { return (uint32)m_AnimationClipList.size(); }

			void AddClip(std::shared_ptr<AnimationClip> clip);
			void RemoveClip(std::shared_ptr<AnimationClip> clip);
			void RemoveClip(const tstring& name);

			void SetAnimationClips(std::vector<std::shared_ptr<AnimationClip>> clips);
			std::vector<std::shared_ptr<AnimationClip>> GetAnimationClips();

		private:
			bool CheckVaild() { return m_AnimationClipList.size(); }
			/// @brief �ִϸ��̼��� �����ϱ����� ���ӿ�����Ʈ�� ����Ʈ
			/// �ִϸ��̼��� ����Ǿ���� ���� ������Ʈ�� �����Ǵ� ��찡 ������ ����Ǿ�� �Ѵ�.
			/// ���� ������� ���Ѵٸ� �� ������ �ִϸ��̼��� ����Ǿ���� ���ӿ�����Ʈ�� ã�ƾ� �Ѵ�.
			struct TargetList
			{
				bool _isInit = false;
				std::vector<GameObject*> _targets;
				std::vector<Matrix> _initLocalTMs;
			};

			struct AnimationInfo
			{
				std::shared_ptr<AnimationClip> _clip;
				TargetList _targetList;

				inline bool IsVaild()
				{
					return (_clip != nullptr && _targetList._isInit);
				}
			};

			void MakeTargetList(AnimationInfo& info);

			Frame m_Frame;

			/// @brief �ִϸ��̼��� ���࿩��
			bool m_IsPlaying;

			/// @brief OnStart�� �ҷȴٴ� ���� ���� ������Ʈ�� ���� ������ ���� �Ǿ����� üũ�ϱ� ���� ����
			bool m_IsCallOnStart;

			/// @brief ���� ���� ���� Ŭ�� ĳ��
			//std::shared_ptr<AnimationClip> m_CurrAnimationClip;
			//TargetList* m_CurrTarget;
			uint32 m_CurrIndex;
			AnimationInfo* m_CurrAnimationInfo;

			/// @brief �ִϸ��̼�Ŭ���� Ŭ���� �� �̷�� �ִ� Ÿ�� ���� ������Ʈ���� ��� �ִ� �����̳�
			std::vector<AnimationInfo> m_AnimationClipList;

			RTTR_ENABLE(Component)

			RTTR_REGISTRATION_FRIEND
		};
	}
}