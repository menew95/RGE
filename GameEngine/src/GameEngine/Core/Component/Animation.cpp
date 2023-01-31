#include "GameEngine/GameEnginePCH.h"

#include "GameEngine/Time.h"

#include "GameEngine/Core/GameObject.h"

#include "GameEngine/Core/Component/Animation.h"
#include "GameEngine/Core/Component/Transform.h"

namespace GameEngine
{
	namespace Core
	{
		Animation::Animation(std::shared_ptr<class GameObject>& gameObject, const tstring& type /*= TEXT("Animation")*/)
			: Component(gameObject, type)
			, m_IsPlaying(false)
			, m_IsCallOnStart(false)
			, m_CurrTarget(nullptr)
		{

		}

		Animation::~Animation()
		{

		}

		void Animation::Play()
		{
			if (m_CurrAnimationClip != nullptr)
			{
				m_IsPlaying = true;
			}
		}

		void Animation::Play(const tstring& name)
		{
			m_IsPlaying = false;

			const AnimationClip* _findClip = nullptr;
			uint _findIndex = 0;
			for (auto& _clip : m_AnimationClipList)
			{
				if (_clip.second->GetClipName() == name)
				{
					_findClip = _clip.second.get();

					m_CurrAnimationClip = _clip.second;
					m_CurrTarget = &_clip.first;
					m_IsPlaying = true;
					m_CurrIndex = _findIndex;
					return;
				}
				_findIndex++;
			}

			m_CurrTarget = nullptr;
		}

		std::shared_ptr<AnimationClip> Animation::GetAnimationClip(uint32 index)
		{
			assert(index < m_AnimationClipList.size());

			return m_AnimationClipList[index].second;
		}

		std::shared_ptr<AnimationClip> Animation::GetAnimationClip(const tstring& clipName)
		{
			for (auto& _clip : m_AnimationClipList)
			{
				if (_clip.second->GetClipName() == clipName)
				{
					return _clip.second;
				}
			}

			assert(false);
		}

		void Animation::AddClip(std::shared_ptr<AnimationClip> clip)
		{
			assert(clip != nullptr);

			auto _iter = m_AnimationClipList.emplace_back(std::make_pair(TargetList(), clip));

			if (m_IsCallOnStart)
			{
				MakeTargetList(clip, _iter.first);
			}
		}

		void Animation::Update()
		{
			if (m_IsPlaying == true && m_CurrAnimationClip != nullptr && m_CurrTarget->_isInit)
			{
				assert(m_CurrTarget != nullptr);

				for (uint32 i = 0; i < m_CurrAnimationClip->GetAnimationSnapCount(); i++)
				{
					m_Frame._frameRate += 0.001f;//(float)GameTime::GetDeltaTime();

					auto _transform = m_CurrAnimationClip->GetTransform(m_Frame, i);

					m_CurrTarget->_targets[i]->GetTransform()->SetLocal(_transform);
				}
			}
		}

		void Animation::Start()
		{
			m_IsCallOnStart = true;

			for (auto& _pair : m_AnimationClipList)
			{
				MakeTargetList(_pair.second, _pair.first);
			}

			if (m_CurrAnimationClip != nullptr && m_CurrTarget != nullptr && m_CurrTarget->_isInit)
			{
				MakeTargetList(m_CurrAnimationClip, *m_CurrTarget);
			}
		}

		void Animation::RemoveClip(std::shared_ptr<AnimationClip> clip)
		{
			for (auto _iter = m_AnimationClipList.begin(); _iter != m_AnimationClipList.end(); _iter++)
			{
				if ((*_iter).second == clip)
				{
					m_AnimationClipList.erase(_iter);

					break;
				}
			}
		}

		void Animation::RemoveClip(const tstring& name)
		{
			for (auto _iter = m_AnimationClipList.begin(); _iter != m_AnimationClipList.end(); _iter++)
			{
				if ((*_iter).second->GetClipName() == name)
				{
					m_AnimationClipList.erase(_iter);

					break;
				}
			}
		}

		void Animation::MakeTargetList(std::shared_ptr<AnimationClip>& clip, TargetList& list)
		{
			list._targets.reserve(clip->GetAnimationSnapCount());
			list._isInit = true;
			for (uint32 i = 0; i < clip->GetAnimationSnapCount(); i++)
			{
				auto* _findObject = GetGameObject()->FindGameObject(clip->GetSnapList()[i].m_TargetName);

				list._targets.emplace_back(_findObject);
			}
		}
	}
}