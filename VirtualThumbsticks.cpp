#include <pch.h>
#include <VirtualThumbsticks.h>

using namespace DirectX::SimpleMath;
using namespace VirtualThumbsticksSandbox;

void VirtualThumbsticks::Update() {
	std::shared_ptr<InputEvent> e;
	while (mInputEvents.try_pop(e)) {
		auto type = e->GetType();
		auto position = e->GetPosition();
		auto id = e->GetId();

		switch (type)
		{
		case InputEventType::PointerPressed:
			mPointerPressed = true;
			if (position.x < DisplayWidth / 2.0 && leftId == -1) {
				leftId = id;
				LeftThumbstickCenter = position;
			} else if(position.x >= DisplayWidth / 2.0 && rightId == -1){
				rightId = id;
				RightThumbstickCenter = position;
			}
			break;
		case InputEventType::PointerMoved:
			if (mPointerPressed) {
				if (id == leftId) {
					LeftPosition = position;
				}
				if (id == rightId) {
					RightPosition = position;
				}
			}
			break;
		case InputEventType::PointerReleased:
			mPointerPressed = !mInputEvents.empty();
			if (id == leftId) {
				leftId = -1;
				LeftPosition = Vector2::Zero;
				LeftThumbstickCenter = Vector2::Zero;
			}

			if (id == rightId) {
				rightId = -1;
				RightPosition = Vector2::Zero;
				RightThumbstickCenter = Vector2::Zero;
			}

			break;
		default:
			break;
		}
	}
	if (!mPointerPressed) {
		LeftPosition = Vector2::Zero;
		RightPosition = Vector2::Zero;

		LeftThumbstickCenter = Vector2::Zero;
		RightThumbstickCenter = Vector2::Zero;
	}
}

DirectX::SimpleMath::Vector2 VirtualThumbsticks::GetLeftThumbstick() {
	if (LeftThumbstickCenter == Vector2::Zero)
		return Vector2::Zero;

	// calculate the scaled vector from the touch position to the center,
	// scaled by the maximum thumbstick distance
	Vector2 l = (LeftPosition - LeftThumbstickCenter) / mMaxThumbstickDistance;

	// if the length is more than 1, normalize the vector
	if (l.LengthSquared() > 1.0)
		l.Normalize();

	return l;
}

DirectX::SimpleMath::Vector2 VirtualThumbsticks::GetRightThumbstick() {
	if (RightThumbstickCenter == Vector2::Zero)
		return Vector2::Zero;

	// calculate the scaled vector from the touch position to the center,
	// scaled by the maximum thumbstick distance
	Vector2 l = (RightPosition - RightThumbstickCenter) / mMaxThumbstickDistance;

	// if the length is more than 1, normalize the vector
	if (l.LengthSquared() > 1.0)
		l.Normalize();

	return l;
}

DirectX::SimpleMath::Vector2 VirtualThumbsticks::GetLeftThumbstickCenter() {
	return LeftThumbstickCenter;
}

DirectX::SimpleMath::Vector2 VirtualThumbsticks::GetRightThumbstickCenter() {
	return RightThumbstickCenter;
}

void VirtualThumbsticks::QueueEvent(std::shared_ptr<InputEvent>& event) {
	mInputEvents.push(event);
}
