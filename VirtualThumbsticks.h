#pragma once
#include "IThumbsticks.h"
#include <InputEvent.h>
#include <concurrent_queue.h>

namespace VirtualThumbsticksSandbox {

	class VirtualThumbsticks : public IThumbsticks {
	public:
		VirtualThumbsticks() { }

		DirectX::SimpleMath::Vector2 GetLeftThumbstick();
		DirectX::SimpleMath::Vector2 GetRightThumbstick();

		void Initialize(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void Update();
		void Render(std::shared_ptr<DirectX::SpriteBatch> spriteBatch);

		void QueueEvent(std::shared_ptr<InputEvent>& event);

	private:
		concurrency::concurrent_queue<std::shared_ptr<InputEvent>> mInputEvents;
		bool mPointerPressed;
		int mLeftId = -1;
		int mRightId = -1;
		int mDisplayWidth;
		int mDisplayHeight;

		DirectX::SimpleMath::Vector2 mLeftThumbstickCenter;
		DirectX::SimpleMath::Vector2 mRightThumbstickCenter;

		DirectX::SimpleMath::Vector2 mLeftPosition;
		DirectX::SimpleMath::Vector2 mRightPosition;

		float mMaxThumbstickDistance = 60.0;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mThumbstickTexture;
		UINT mThumbstickTextureWidth;
		UINT mThumbstickTextureHeight;
	};
}