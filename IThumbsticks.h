#pragma once
#include <SimpleMath.h>

namespace VirtualThumbsticksSandbox {

	class IThumbsticks {
	public:
		virtual void Update() = 0;
		virtual DirectX::SimpleMath::Vector2 GetLeftThumbstick() = 0;
		virtual DirectX::SimpleMath::Vector2 GetRightThumbstick() = 0;
	};
}