#include "GameEngine/GameEnginePCH.h"

#include "GameEngine/Core/Resource/AnimationClip.h"

#include "GameEngine/Core/Component/Transform.h"

#include "GameEngine/Core/GameObject.h"

namespace GameEngine
{
	namespace Core
	{
		AnimationSnap::AnimationSnap()
		{

		}

		AnimationSnap::~AnimationSnap()
		{

		}

		void AnimationSnap::AddKeyFrame(KeyFrame keyFrame, eKeyType type)
		{
			switch (type)
			{
				case AnimationSnap::Position:
					m_PosKeyFrameList.push_back(keyFrame);
					break;
				case AnimationSnap::Rotate:
					m_RotKeyFrameList.push_back(keyFrame);
					break;
				case AnimationSnap::Scale:
					m_ScaleKeyFrameList.push_back(keyFrame);
					break;
				default:
					assert(false);
					break;
			}
		}

		Math::Matrix AnimationSnap::GetTransform(Frame& frame) const
		{
			Vector3 _lerpPos, _lerpScale;
			Quaternion _slerpRot;

			if (frame._frameRate > m_MaxFrameRate)
			{
				frame._posFrame = 0;
				frame._rotFrame = 0;
				frame._scaleFrame = 0;
				frame._frameRate -= m_MaxFrameRate;
			}

			// pos
			if (frame._posFrame < (uint32)m_PosKeyFrameList.size() - 1)
			{
				uint32 _nextFrame = frame._posFrame + 1;

				if (frame._frameRate > m_PosKeyFrameList[_nextFrame].m_FrameRate)
				{
					frame._posFrame++;
					_nextFrame++;
				}

				Vector3 _prePosFrame =
				{
					m_PosKeyFrameList[frame._posFrame].m_Data.x,
					m_PosKeyFrameList[frame._posFrame].m_Data.y,
					m_PosKeyFrameList[frame._posFrame].m_Data.z
				};

				Vector3 _nextPosFrame =
				{
					m_PosKeyFrameList[_nextFrame].m_Data.x,
					m_PosKeyFrameList[_nextFrame].m_Data.y,
					m_PosKeyFrameList[_nextFrame].m_Data.z
				};

				float _alpha = (frame._frameRate - m_PosKeyFrameList[frame._posFrame].m_FrameRate)
					/ (m_PosKeyFrameList[_nextFrame].m_FrameRate - m_PosKeyFrameList[frame._posFrame].m_FrameRate);

				if (_alpha < 0.f)
				{
					_alpha = 0.0f;
				}
				else if (_alpha > 1.0f)
				{
					_alpha = 1.0f;
				}

				_lerpPos = Vector3::Lerp(_prePosFrame, _nextPosFrame, _alpha);
			}

			// rot
			if (frame._rotFrame < (uint32)m_RotKeyFrameList.size() - 1)
			{
				uint32 _nextFrame = frame._rotFrame + 1;

				if (frame._frameRate > m_RotKeyFrameList[_nextFrame].m_FrameRate)
				{
					frame._rotFrame++;
					_nextFrame++;
				}

				Quaternion _preQFrame =
				{
					m_RotKeyFrameList[frame._rotFrame].m_Data.x,
					m_RotKeyFrameList[frame._rotFrame].m_Data.y,
					m_RotKeyFrameList[frame._rotFrame].m_Data.z,
					m_RotKeyFrameList[frame._rotFrame].m_Data.w
				};

				Quaternion _nextQFrame =
				{
					m_RotKeyFrameList[_nextFrame].m_Data.x,
					m_RotKeyFrameList[_nextFrame].m_Data.y,
					m_RotKeyFrameList[_nextFrame].m_Data.z,
					m_RotKeyFrameList[_nextFrame].m_Data.w
				};

				float alpha = (frame._frameRate - m_RotKeyFrameList[frame._rotFrame].m_FrameRate)
					/ (m_RotKeyFrameList[_nextFrame].m_FrameRate - m_RotKeyFrameList[frame._rotFrame].m_FrameRate);

				if (alpha < 0.f)
				{
					alpha = 0.0f;
				}
				else if (alpha > 1.0f)
				{
					alpha = 1.0f;
				}

				_slerpRot = Quaternion::Slerp(_preQFrame, _nextQFrame, alpha);
			}

			// scale
			if (frame._scaleFrame < (uint32)m_ScaleKeyFrameList.size() - 1)
			{
				uint32 _nextFrame = frame._scaleFrame + 1;

				if (frame._frameRate > m_ScaleKeyFrameList[_nextFrame].m_FrameRate)
				{
					frame._scaleFrame++;
					_nextFrame++;
				}

				Vector3 _preScaleFrame =
				{
					m_ScaleKeyFrameList[frame._scaleFrame].m_Data.x,
					m_ScaleKeyFrameList[frame._scaleFrame].m_Data.y,
					m_ScaleKeyFrameList[frame._scaleFrame].m_Data.z
				};

				Vector3 _nextScaleFrame =
				{
					m_ScaleKeyFrameList[_nextFrame].m_Data.x,
					m_ScaleKeyFrameList[_nextFrame].m_Data.y,
					m_ScaleKeyFrameList[_nextFrame].m_Data.z
				};

				float alpha = (frame._frameRate - m_ScaleKeyFrameList[frame._scaleFrame].m_FrameRate)
					/ (m_ScaleKeyFrameList[_nextFrame].m_FrameRate - m_ScaleKeyFrameList[frame._scaleFrame].m_FrameRate);

				if (alpha < 0.f)
				{
					alpha = 0.0f;
				}
				else if (alpha > 1.0f)
				{
					alpha = 1.0f;
				}

				_lerpScale = Vector3::Lerp(_preScaleFrame, _nextScaleFrame, alpha);
			}

			return Matrix::CreateScale(_lerpScale) * Matrix::CreateFromQuaternion(_slerpRot) * Matrix::CreateTranslation(_lerpPos);
		}

		Math::Matrix AnimationClip::GetTransform(Frame& frame, uint32 snapIndex) const
		{
			assert(snapIndex < m_AnimationSnapList.size());

			return m_AnimationSnapList[snapIndex].GetTransform(frame);
		}

		bool AnimationClip::operator==(const tstring& value)
		{
			return (m_ClipName == value);
		}
	}
}