#pragma once
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include "Common/DeviceResources.h"
#include "InputEvent.h"

namespace VirtualThumbsticksSandbox {

	class IThumbsticks {
	public:
		virtual void Initialize(const std::shared_ptr<DX::DeviceResources>& deviceResources) = 0;
		virtual void Update() = 0;
		virtual void QueueEvent(std::shared_ptr<InputEvent>& event) = 0;
		virtual void Render(std::shared_ptr<DirectX::SpriteBatch> spriteBatch) = 0;
		virtual DirectX::SimpleMath::Vector2 GetLeftThumbstick() = 0;
		virtual DirectX::SimpleMath::Vector2 GetRightThumbstick() = 0;
	};
}