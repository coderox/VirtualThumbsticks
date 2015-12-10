#pragma once
#include <SimpleMath.h>

enum InputEventType {
	PointerPressed,
	PointerMoved,
	PointerReleased
};

class InputEvent {
public:
	InputEvent(InputEventType type, int id, DirectX::SimpleMath::Vector2 position)
		: mType(type)
		, mId(id)
		, mPosition(position)
	{ }

	InputEventType GetType() { return mType; }

	int GetId() { return mId; }

	DirectX::SimpleMath::Vector2 GetPosition() { return mPosition; }

private:
	InputEventType mType;
	int mId;
	DirectX::SimpleMath::Vector2 mPosition;
};