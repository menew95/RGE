#pragma once

#include "GameEngine\Core\Component\Component.h"

namespace GameEngine
{
	namespace Core
	{
		class MonoBehaviour
			: public Component
		{
		public:
			MonoBehaviour(std::shared_ptr<GameObject>& gameObject, const tstring& componentName = TEXT("MonoBehaviour"));
			virtual ~MonoBehaviour();

			virtual void Awake() {}
			virtual void Start() {}
			virtual void Update() {}
			virtual void LateUpdate() {}
			virtual void FixedUpdate() {}
			virtual void OnEnable() {}
			virtual void OnDisable() {}

		protected:

		};
	}
}