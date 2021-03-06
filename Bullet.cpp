#include <pch.h>
#include <Bullet.h>
#include <math.h>
#include <Common\DirectXHelper.h>
#include <WICTextureLoader.h>

using namespace std;
using namespace DX;
using namespace DirectX;
using namespace Platform;
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;

BulletModel::BulletModel() {

}

BulletModel::~BulletModel() {

}

void BulletModel::LoadContent(shared_ptr<DeviceResources>& deviceResources, wstring filename) {
	ComPtr<ID3D11Resource> res;
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(deviceResources->GetD3DDevice(), filename.c_str(), res.ReleaseAndGetAddressOf(), Texture.ReleaseAndGetAddressOf(), 0)
		);
	DX::GetTextureSize(res.Get(), &TextureWidth, &TextureHeight);
}

/************************************************/

Bullet::Bullet(shared_ptr<BulletModel> bulletModel, Vector2 pos, Vector2 vel, SimpleMath::Color col)
	: mBulletModel(bulletModel)
	, mPosition(pos)
	, mVelocity(vel)
	, mColor(col)
{
	mRotation = atan2(vel.y, vel.x);
}

Bullet::~Bullet() {
	OutputDebugString(L"Bullet destructor");
}

void Bullet::Update() {
	mPosition += mVelocity;
}

void Bullet::Draw(std::shared_ptr<SpriteBatch>& spriteBatch) {
	spriteBatch->Draw(mBulletModel->Texture.Get(), mPosition, nullptr, mColor, mRotation,
		Vector2(mBulletModel->TextureWidth / 2.0f, mBulletModel->TextureHeight / 2.0f), 1.0f, SpriteEffects::SpriteEffects_None, 0.0);
}