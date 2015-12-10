#include <pch.h>
#include <Bullet.h>
#include <math.h>
#include <Common\DirectXHelper.h>
#include <WICTextureLoader.h>

using namespace DX;
using namespace DirectX;
using namespace Platform;
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;

BulletModel::BulletModel() {

}

BulletModel::~BulletModel() {

}

void BulletModel::LoadContent(std::shared_ptr<DeviceResources>& deviceResources, String^ filename) {
	ComPtr<ID3D11Resource> res;
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(deviceResources->GetD3DDevice(), filename->Data(), res.ReleaseAndGetAddressOf(), Texture.ReleaseAndGetAddressOf(), 0)
		);
	DX::GetTextureSize(res.Get(), &TextureWidth, &TextureHeight);
}

/************************************************/

Bullet::Bullet(BulletModel* bulletModel, Vector2 pos, Vector2 vel, SimpleMath::Color col)
	: mBulletModel(bulletModel)
	, mPosition(pos)
	, mVelocity(vel)
	, mColor(col)
{
	mRotation = atan2(vel.y, vel.x);
}

void Bullet::Update() {
	mPosition += mVelocity;
}

void Bullet::Draw(std::shared_ptr<SpriteBatch>& spriteBatch) {
	spriteBatch->Draw(mBulletModel->Texture.Get(), mPosition, nullptr, mColor, mRotation,
		Vector2(mBulletModel->TextureWidth / 2.0, mBulletModel->TextureHeight / 2.0), 1.0, SpriteEffects::SpriteEffects_None, 0.0);
}