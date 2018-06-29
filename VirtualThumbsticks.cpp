#include <pch.h>
#include <VirtualThumbsticks.h>

using namespace std;
using namespace DirectX;
using namespace SimpleMath;
using namespace VirtualThumbsticksSandbox;

void VirtualThumbsticks::Initialize(const std::shared_ptr<DX::DeviceResources>& deviceResources) {
	auto screenViewport = deviceResources->GetScreenViewport();
	
	mDisplayHeight = static_cast<int>(screenViewport.Height);
	mDisplayWidth = static_cast<int>(screenViewport.Width);
	Microsoft::WRL::ComPtr<ID3D11Resource> res;
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(deviceResources->GetD3DDevice(), L"Assets/thumbstick.png", res.ReleaseAndGetAddressOf(), mThumbstickTexture.ReleaseAndGetAddressOf(), 0)
	);
	DX::GetTextureSize(res.Get(), &mThumbstickTextureWidth, &mThumbstickTextureHeight);
}

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
			if (position.x < mDisplayWidth / 2.0 && mLeftId == -1) {
				mLeftId = id;
				mLeftThumbstickCenter = position;
			} else if(position.x >= mDisplayWidth / 2.0 && mRightId == -1){
				mRightId = id;
				mRightThumbstickCenter = position;
			}
			break;
		case InputEventType::PointerMoved:
			if (mPointerPressed) {
				if (id == mLeftId) {
					mLeftPosition = position;
				}
				if (id == mRightId) {
					mRightPosition = position;
				}
			}
			break;
		case InputEventType::PointerReleased:
			mPointerPressed = !mInputEvents.empty();
			if (id == mLeftId) {
				mLeftId = -1;
				mLeftPosition = Vector2::Zero;
				mLeftThumbstickCenter = Vector2::Zero;
			}

			if (id == mRightId) {
				mRightId = -1;
				mRightPosition = Vector2::Zero;
				mRightThumbstickCenter = Vector2::Zero;
			}

			break;
		default:
			break;
		}
	}
	if (!mPointerPressed) {
		mLeftPosition = Vector2::Zero;
		mRightPosition = Vector2::Zero;

		mLeftThumbstickCenter = Vector2::Zero;
		mRightThumbstickCenter = Vector2::Zero;
	}
}

void VirtualThumbsticks::Render(shared_ptr<SpriteBatch> spriteBatch) {
	if (mLeftThumbstickCenter != Vector2::Zero) {
		Vector2 position(mLeftThumbstickCenter - Vector2(mThumbstickTextureWidth / 2.0f, mThumbstickTextureHeight / 2.0f));
		spriteBatch->Draw(mThumbstickTexture.Get(), position, Colors::Green);
	}

	if (mRightThumbstickCenter != Vector2::Zero) {
		spriteBatch->Draw(mThumbstickTexture.Get(), mRightThumbstickCenter - Vector2(mThumbstickTextureWidth / 2.0f, mThumbstickTextureHeight / 2.0f), Colors::Blue);
	}
}

DirectX::SimpleMath::Vector2 VirtualThumbsticks::GetLeftThumbstick() {
	if (mLeftThumbstickCenter == Vector2::Zero)
		return Vector2::Zero;

	// calculate the scaled vector from the touch position to the center,
	// scaled by the maximum thumbstick distance
	Vector2 l = (mLeftPosition - mLeftThumbstickCenter) / mMaxThumbstickDistance;

	// if the length is more than 1, normalize the vector
	if (l.LengthSquared() > 1.0)
		l.Normalize();

	return l;
}

DirectX::SimpleMath::Vector2 VirtualThumbsticks::GetRightThumbstick() {
	if (mRightThumbstickCenter == Vector2::Zero)
		return Vector2::Zero;

	// calculate the scaled vector from the touch position to the center,
	// scaled by the maximum thumbstick distance
	Vector2 l = (mRightPosition - mRightThumbstickCenter) / mMaxThumbstickDistance;

	// if the length is more than 1, normalize the vector
	if (l.LengthSquared() > 1.0)
		l.Normalize();

	return l;
}

void VirtualThumbsticks::QueueEvent(std::shared_ptr<InputEvent>& event) {
	mInputEvents.push(event);
}
