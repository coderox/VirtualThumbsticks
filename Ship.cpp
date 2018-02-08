#include <pch.h>
#include <Ship.h>
#include <math.h>
#include <WICTextureLoader.h>
#include <Common\DirectXHelper.h>
#include <VirtualThumbsticks.h>

using namespace std;
using namespace DirectX;
using namespace Platform;
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;
using namespace VirtualThumbsticksSandbox;

/*********************************************************/

Ship::Ship() {

}

void Ship::LoadContent(std::shared_ptr<DX::DeviceResources> deviceResources, wstring filename) {
	ComPtr<ID3D11Resource> res;
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(deviceResources->GetD3DDevice(), filename.c_str(), res.ReleaseAndGetAddressOf(), mTexture.ReleaseAndGetAddressOf(), 0)
		);
	DX::GetTextureSize(res.Get(), &mTextureWidth, &mTextureHeight);
}

void Ship::Draw(std::shared_ptr<DirectX::SpriteBatch>& spriteBatch) {
	spriteBatch->Draw(mTexture.Get(), mPosition, nullptr, DirectX::Colors::White, mRotation,
		Vector2(mTextureWidth / 2.0, mTextureHeight / 2.0), 1.0, SpriteEffects::SpriteEffects_None, 0.0);
}

/*********************************************************/

EnemyShip::EnemyShip() : Ship() {

}

void EnemyShip::LoadContent(std::shared_ptr<DX::DeviceResources> deviceResources, wstring filename) {
	Ship::LoadContent(deviceResources, filename);

	mRadius = sqrt(mTextureWidth * mTextureWidth + mTextureHeight * mTextureHeight) * 0.75;
}

void EnemyShip::Update(DX::StepTimer const& timer) {
	auto playerPosition = Player->GetPosition();
	Vector2 d(playerPosition.x - mPosition.x, playerPosition.y - mPosition.y);
	d.Normalize();

	mRotation = atan2(d.y, d.x);

	d.x *= 4.0;
	d.y *= 4.0;

	mPosition += d;
}

bool EnemyShip::ContainsPoint(DirectX::SimpleMath::Vector2 point) {
	return Vector2::Distance(mPosition, point) < mRadius;
}

/*********************************************************/

PlayerShip::PlayerShip(Vector2 position) 
	: Ship()
{
	mPosition.x = position.x;
	mPosition.y = position.y;
}

void PlayerShip::LoadContent(std::shared_ptr<DX::DeviceResources> deviceResources, wstring filename, wstring bulletFilename) {
	Ship::LoadContent(deviceResources, filename);

	mBulletModel.reset(new BulletModel());
	mBulletModel->LoadContent(deviceResources, bulletFilename);
}

void PlayerShip::ProcessInput(std::shared_ptr<IThumbsticks> thumbsticks) {
	if (thumbsticks) {
		auto leftThumbstick = thumbsticks->GetLeftThumbstick();
		auto rightThumbstick = thumbsticks->GetRightThumbstick();
		
		mVelocity += leftThumbstick * mAcceleration;
		
		if (rightThumbstick.Length() > 0.3) {
			mRotation = -atan2(-rightThumbstick.y, rightThumbstick.x);
			if (mFireTimer <= 0) {
				rightThumbstick.Normalize();
				Vector2 bulletVelocity(rightThumbstick * mBulletSpeed);

				Bullets.push_back(Bullet(mBulletModel, mPosition, bulletVelocity, SimpleMath::Color(1.0, 0.0, 0.0)));

				mFireTimer = mCooldown;
			}
		} else if (leftThumbstick.Length() > 0.2) {
			mRotation = -atan2(-leftThumbstick.y, leftThumbstick.x);
		}
	}

	mPosition += mVelocity;
	mVelocity *= 0.98;
}

void PlayerShip::Update(DX::StepTimer const& timer) {

	mFireTimer -= timer.GetElapsedSeconds() * 1000;

	for (auto &b : Bullets) {
		b.Update();
	}

	for (int i = 0; i < Bullets.size(); i++) {
		auto b = Bullets[i];
		if (b.GetPosition().x < -WorldWidth / 2.0 ||
			b.GetPosition().x > WorldWidth / 2.0 ||
			b.GetPosition().y < -WorldHeight /2.0 ||
			b.GetPosition().y > WorldHeight /2.0) {
			Bullets.erase(Bullets.begin() + i);
		}
	}

	ClampPlayerShip();
}

void PlayerShip::ClampPlayerShip() {
	if (mPosition.x < -WorldWidth / 2.0)
	{
		mPosition.x = -WorldWidth / 2.0;
		if (mVelocity.x < 0.0)
			mVelocity.x = 0.0;
	}

	if (mPosition.x > WorldWidth / 2.0)
	{
		mPosition.x = WorldWidth / 2.0;
		if (mVelocity.x > 0.0)
			mVelocity.x = 0.0;
	}

	if (mPosition.y < -WorldHeight / 2.0)
	{
		mPosition.y = -WorldHeight / 2.0;
		if (mVelocity.y < 0.0)
			mVelocity.y = 0.0;
	}

	if (mPosition.y > WorldHeight / 2.0)
	{
		mPosition.y = WorldHeight / 2.0;
		if (mVelocity.y > 0.0)
			mVelocity.y = 0.0;
	}
}

void PlayerShip::Draw(std::shared_ptr<DirectX::SpriteBatch>& spriteBatch) {
	for (auto &b : Bullets)
	{
		b.Draw(spriteBatch);
	}

	Ship::Draw(spriteBatch);
}