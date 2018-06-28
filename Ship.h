#pragma once

#include <SimpleMath.h>
#include <SpriteBatch.h>
#include <Bullet.h>
#include <VirtualThumbsticks.h>

class Ship {
public:

	Ship();
	~Ship() {}
	virtual void LoadContent(std::shared_ptr<DX::DeviceResources> deviceResources, std::wstring filename);
	virtual bool ContainsPoint(DirectX::SimpleMath::Vector2 point) { return false; }
	virtual void Update(DX::StepTimer const& timer) { mPosition += mVelocity; }
	virtual void Draw(std::shared_ptr<DirectX::SpriteBatch>& spriteBatch);

	DirectX::SimpleMath::Vector2 GetPosition() {
		return mPosition;
	}	

	void SetPosition(DirectX::SimpleMath::Vector2 position) {
		mPosition = position;
	}

	DirectX::SimpleMath::Vector2 GetVelocity() {
		return mVelocity;
	}

	float GetRotation() {
		return mRotation; 
	}

protected:
	float mRotation;
	DirectX::SimpleMath::Vector2 mPosition;
	DirectX::SimpleMath::Vector2 mVelocity;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;
	UINT mTextureWidth = 0;
	UINT mTextureHeight = 0;
};

class EnemyShip : public Ship {
public:
	EnemyShip();
	void LoadContent(std::shared_ptr<DX::DeviceResources>, std::wstring filename) override;
	void Update(DX::StepTimer const& timer) override;
	bool ContainsPoint(DirectX::SimpleMath::Vector2 point) override;

	std::shared_ptr<Ship> Player;

private:
	float mRadius;
};

class PlayerShip : public Ship {
public:
	PlayerShip(DirectX::SimpleMath::Vector2 position);
	void LoadContent(std::shared_ptr<DX::DeviceResources>, std::wstring filename, std::wstring bulletFilename);
	void ProcessInput(VirtualThumbsticksSandbox::IThumbsticks* thumbsticks);
	void Update(DX::StepTimer const& timer) override;
	void Draw(std::shared_ptr<DirectX::SpriteBatch>& spriteBatch) override;

	int WorldWidth;
	int WorldHeight;

	std::vector<Bullet> Bullets;

private:
	void ClampPlayerShip();

	float mAcceleration = 0.75;
	float mBulletSpeed = 20.0;

	double mCooldown = 150;
	double mFireTimer;

	std::shared_ptr<BulletModel> mBulletModel;
};