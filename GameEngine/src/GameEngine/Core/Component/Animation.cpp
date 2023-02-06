#include "GameEngine/GameEnginePCH.h"

#include "GameEngine/Time.h"

#include "GameEngine/Core/GameObject.h"

#include "GameEngine/Core/Component/Animation.h"
#include "GameEngine/Core/Component/Transform.h"

#include <rttr/registration>
using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<GameEngine::Core::Animation>("Animation")
	.constructor<std::shared_ptr<GameEngine::Core::GameObject>&, const tstring&>()
	.property("m_AnimationClipList", &GameEngine::Core::Animation::GetAnimationClips, &GameEngine::Core::Animation::SetAnimationClips)
	(
		metadata(GameEngine::Core::MetaData::Serializable, GameEngine::Core::MetaDataType::UUID),
		metadata(GameEngine::Core::Util::Check_Vaild, "CheckVaild"),
		metadata(GameEngine::Core::MetaDataType::UUID, "GetName")
	)
	.method("GetName", &GameEngine::Core::AnimationClip::GetName)
	.method("CheckVaild", &GameEngine::Core::Animation::CheckVaild);
}

namespace GameEngine
{
	namespace Core
	{
		Animation::Animation(std::shared_ptr<class GameObject>& gameObject, const tstring& type /*= TEXT("Animation")*/)
			: Component(gameObject, type)
			, m_IsPlaying(false)
			, m_IsCallOnStart(false)
			, m_CurrAnimationInfo(nullptr)
			, m_CurrIndex(0)
		{

		}

		Animation::~Animation()
		{

		}

		void Animation::Play()
		{
			if (m_CurrAnimationInfo != nullptr)
			{
				m_IsPlaying = true;
			}
		}

		void Animation::Play(const tstring& name)
		{
			m_IsPlaying = false;

			uint _findIndex = 0;
			for (auto& _info : m_AnimationClipList)
			{
				if (_info._clip->GetClipName() == name)
				{
					m_IsPlaying = true;
					
					m_CurrAnimationInfo = &_info;

					m_CurrIndex = _findIndex;
					
					return;
				}
				_findIndex++;
			}

			m_CurrAnimationInfo = nullptr;
		}

		std::shared_ptr<AnimationClip> Animation::GetAnimationClip(uint32 index)
		{
			if(index < m_AnimationClipList.size()) return std::shared_ptr<AnimationClip>(nullptr);

			return m_AnimationClipList[index]._clip;
		}

		std::shared_ptr<AnimationClip> Animation::GetAnimationClip(const tstring& clipName)
		{
			for (auto& _info : m_AnimationClipList)
			{
				if (_info._clip->GetClipName() == clipName)
				{
					return _info._clip;
				}
			}

			return std::shared_ptr<AnimationClip>(nullptr);
		}

		void Animation::AddClip(std::shared_ptr<AnimationClip> clip)
		{
			assert(clip != nullptr);

			AnimationInfo _newInfo;

			_newInfo._clip = clip;
			
			auto& _iter = m_AnimationClipList.emplace_back(_newInfo);

			if (m_CurrAnimationInfo == nullptr)
			{
				m_CurrAnimationInfo = &_iter;

				m_CurrIndex = 0;

				m_IsPlaying = true;
			}

			if (m_IsCallOnStart)
			{
				MakeTargetList(_iter);
			}
		}

		void Animation::Update()
		{
			assert(m_CurrAnimationInfo != nullptr);

			if (m_IsPlaying == true && m_CurrAnimationInfo->IsVaild())
			{
				m_Frame._frameRate += static_cast<float>(Time::GetDeltaTime());

				Log::Core_Info("Animation : " + std::to_string(m_Frame._frameRate));

				for (uint32 i = 0; i < m_CurrAnimationInfo->_clip->GetAnimationSnapCount(); i++)
				{
					if (m_CurrAnimationInfo->_targetList._targets[i] == nullptr) continue;

					auto _transform = m_CurrAnimationInfo->_clip->GetTransform(m_Frame, i);

					m_CurrAnimationInfo->_targetList._targets[i]->GetTransform()->SetLocalTM(_transform);
				}
			}
		}

		void Animation::Start()
		{
			m_IsCallOnStart = true;

			for (auto& _pair : m_AnimationClipList)
			{
				MakeTargetList(_pair);
			}
		}

		void Animation::RemoveClip(std::shared_ptr<AnimationClip> clip)
		{
			for (auto _iter = m_AnimationClipList.begin(); _iter != m_AnimationClipList.end(); _iter++)
			{
				if ((*_iter)._clip == clip)
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
				if ((*_iter)._clip->GetClipName() == name)
				{
					m_AnimationClipList.erase(_iter);

					break;
				}
			}
		}

		void Animation::SetAnimationClips(std::vector<std::shared_ptr<AnimationClip>> clips)
		{
			for (auto& _clip : clips)
			{
				AddClip(_clip);
			}
		}

		std::vector<std::shared_ptr<AnimationClip>> Animation::GetAnimationClips()
		{
			std::vector<std::shared_ptr<AnimationClip>> _clipList;

			_clipList.reserve(m_AnimationClipList.size());

			for (auto& _clip : m_AnimationClipList)
			{
				_clipList.push_back(_clip._clip);
			}

			return _clipList;
		}

		void Animation::MakeTargetList(AnimationInfo& info)
		{
			if (info._targetList._isInit && info._targetList._targets.size() > 0 && info._targetList._initLocalTMs.size() > 0) return;

			info._targetList._targets.reserve(info._clip->GetAnimationSnapCount());
			info._targetList._isInit = true;
			for (uint32 i = 0; i < info._clip->GetAnimationSnapCount(); i++)
			{
				auto* _findObject = GetGameObject()->FindGameObject(info._clip->GetSnapList()[i].m_TargetName);

				info._targetList._targets.emplace_back(_findObject);

				info._targetList._initLocalTMs.emplace_back(_findObject->GetTransform()->GetLocalTM());
			}
		}

	}
}