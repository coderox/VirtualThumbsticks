#pragma once
#include <..\Common\DeviceResources.h>

class BulletModel {
public:
	BulletModel();
	~BulletModel();
	void LoadContent(std::shared_ptr<DX::DeviceResources>& deviceResources, Platform::String^ filename);
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture;
	UINT TextureWidth = 0;
	UINT TextureHeight = 0;
};

class Bullet {
public:
	Bullet(BulletModel* bulletModel, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 vel, DirectX::SimpleMath::Color col);
	void Update();
	void Draw(std::shared_ptr<DirectX::SpriteBatch>& spriteBatch);

	DirectX::SimpleMath::Vector2 GetPosition() { return mPosition; }

	DirectX::SimpleMath::Vector2 GetVelocity() { return mVelocity; }

	DirectX::SimpleMath::Color GetColor() { return mColor; }

private:
	BulletModel* mBulletModel;
	float mRotation;
	DirectX::SimpleMath::Vector2 mPosition;
	DirectX::SimpleMath::Vector2 mVelocity;
	DirectX::SimpleMath::Color mColor;
};