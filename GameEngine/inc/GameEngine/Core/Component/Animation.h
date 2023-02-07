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

			/// @brief 현재 애니메이션 클립이 설정되면 애니메이션을 실행 시켜 준다.
			void Play();

			/// @brief 들어온 이름에 맞는 클립을 가지고 있을 경우 애니메이션을 실행 시켜 준다.
			/// @param name 클립 이름
			void Play(const tstring& name);

			/// @brief 애니메이션을 멈추고 모든 타겟오브젝트를 초기 상태로 되돌려 준다.
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
			/// @brief 애니메이션을 적용하기위한 게임오브젝트의 리스트
			/// 애니메이션이 적용되어야할 게임 오브젝트가 삭제되는 경우가 없음이 보장되어야 한다.
			/// 만약 보장되지 못한다면 매 프레임 애니메이션이 적용되어야할 게임오브젝트를 찾아야 한다.
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

			/// @brief 애니메이션이 실행여부
			bool m_IsPlaying;

			/// @brief OnStart가 불렸다는 것은 게임 오브젝트의 계층 구조가 생성 되었는지 체크하기 위한 변수
			bool m_IsCallOnStart;

			/// @brief 현재 적용 중인 클립 캐싱
			//std::shared_ptr<AnimationClip> m_CurrAnimationClip;
			//TargetList* m_CurrTarget;
			uint32 m_CurrIndex;
			AnimationInfo* m_CurrAnimationInfo;

			/// @brief 애니메이션클립과 클립과 페어를 이루고 있는 타겟 게임 오브젝트들을 담고 있는 컨테이너
			std::vector<AnimationInfo> m_AnimationClipList;

			RTTR_ENABLE(Component)

			RTTR_REGISTRATION_FRIEND
		};
	}
}