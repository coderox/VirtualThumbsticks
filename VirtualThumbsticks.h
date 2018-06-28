#pragma once
#include "IThumbsticks.h"
#include <InputEvent.h>
#include <concurrent_queue.h>

namespace VirtualThumbsticksSandbox {

	class VirtualThumbsticks : public IThumbsticks {
	public:
		VirtualThumbsticks() { }

		DirectX::SimpleMath::Vector2 GetLeftThumbstick();
		DirectX::SimpleMath::Vector2 GetLeftThumbstickCenter();

		DirectX::SimpleMath::Vector2 GetRightThumbstick();
		DirectX::SimpleMath::Vector2 GetRightThumbstickCenter();

		void Update();

		void QueueEvent(std::shared_ptr<InputEvent>& event);

		int DisplayWidth;
		int DisplayHeight;

	private:
		concurrency::concurrent_queue<std::shared_ptr<InputEvent>> mInputEvents;
		bool mPointerPressed;
		int leftId = -1;
		int rightId = -1;

		DirectX::SimpleMath::Vector2 LeftThumbstickCenter;
		DirectX::SimpleMath::Vector2 RightThumbstickCenter;

		DirectX::SimpleMath::Vector2 LeftPosition;
		DirectX::SimpleMath::Vector2 RightPosition;

		float mMaxThumbstickDistance = 60.0;
	};
}