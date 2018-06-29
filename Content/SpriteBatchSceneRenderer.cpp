#include "pch.h"
#include "SpriteBatchSceneRenderer.h"

#include "..\Common\DirectXHelper.h"
#include "WICTextureLoader.h"

using namespace VirtualThumbsticksSandbox;

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
SpriteBatchSceneRenderer::SpriteBatchSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources)
	: m_loadingComplete(false)
	, m_deviceResources(deviceResources)
	, mWorldBorderThickness(4)
	, mWorldBorderThicknessDoubled(8)
	, mNumStars(500)
	, mWorldWidth(2000)
	, mWorldHeight(2000)
	, mSpawnTimer(2000)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

SpriteBatchSceneRenderer::~SpriteBatchSceneRenderer() {
	mPlayer.reset();
	mPlayer = nullptr;

	mEnemies.clear();
	
	mThumbsticks.reset();
	mThumbsticks = nullptr;
}

double RandomNextDouble() {
	return (double)rand() / RAND_MAX;
}

double Random(int min, int max) {
	return min + (rand() % (max - min));
}

// Initializes view parameters when the window size changes.
void SpriteBatchSceneRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	mSpriteBatch.reset(new SpriteBatch(m_deviceResources->GetD3DDeviceContext()));

	auto screenViewport = m_deviceResources->GetScreenViewport();
	mGraphicsWidthHalf = outputSize.Width / 2.0f;
	mGraphicsHeightHalf = outputSize.Height / 2.0f;

	mPlayer.reset(new PlayerShip(DirectX::SimpleMath::Vector2(mGraphicsWidthHalf, mGraphicsHeightHalf)));
	mPlayer->WorldHeight = mWorldHeight;
	mPlayer->WorldWidth = mWorldWidth;
	mPlayer->LoadContent(m_deviceResources, L"Assets/player1.png", L"Assets/bullet.png");

	//mEnemy.reset(new EnemyShip());
	//mEnemy->Player = mPlayer.get();
	//mEnemy->LoadContent(m_deviceResources, "Assets/alien.png");

	mThumbsticks.reset(new VirtualThumbsticksSandbox::VirtualThumbsticks());
	mThumbsticks->DisplayHeight = static_cast<int>(screenViewport.Height);
	mThumbsticks->DisplayWidth = static_cast<int>(screenViewport.Width);

	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), L"Assets/blank.png", nullptr, mBlankTexture.ReleaseAndGetAddressOf(), 0)
		);

	Microsoft::WRL::ComPtr<ID3D11Resource> res;
	DX::ThrowIfFailed(
		DirectX::CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), L"Assets/thumbstick.png", res.ReleaseAndGetAddressOf(), mThumbstickTexture.ReleaseAndGetAddressOf(), 0)
		);
	DX::GetTextureSize(res.Get(), &mThumbstickTextureWidth, &mThumbstickTextureHeight);

	mStars.clear();
	for (int i = 0; i < mNumStars; i++) {
		float random = static_cast<float>(RandomNextDouble());
		float x = random  * (mWorldWidth + outputSize.Width) - (mWorldWidth / 2.0f + mGraphicsWidthHalf);
		random = static_cast<float>(RandomNextDouble());
		float y = random * (mWorldHeight + outputSize.Height) - (mWorldHeight / 2.0f + mGraphicsHeightHalf);
		float z = static_cast<float>(Random(1, 3));
		mStars.push_back(Vector3(x,y,z));
	}
}

void SpriteBatchSceneRenderer::ProcessInput() {
	mThumbsticks->Update();
	mPlayer->ProcessInput(mThumbsticks);
}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void SpriteBatchSceneRenderer::Update(DX::StepTimer const& timer)
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}

	mSpawnTimer -= timer.GetElapsedSeconds() * 1000;
	if (mSpawnTimer <= 0) {
		int numToSpan = static_cast<int>(Random(1, 3));
		for (int i = 0; i < numToSpan; i++) {
			EnemyShip enemy;
			enemy.Player = mPlayer;
			enemy.LoadContent(m_deviceResources, L"Assets/alien.png");

			Vector2 position;
			if (rand() % 2 == 0) {
				position.x = -mWorldWidth / 2.0f - (mGraphicsWidthHalf + 10);
			} else {
				position.x = mWorldWidth / 2.0f + (mGraphicsWidthHalf + 10);
			}
			if (rand() % 2 == 0) {
				position.y = -mWorldHeight / 2.0f - (mGraphicsHeightHalf + 10);
			} else {
				position.y = mWorldHeight / 2.0f + (mGraphicsHeightHalf + 10);
			}
			enemy.SetPosition(position);
			mEnemies.push_back(enemy);
		}
		mSpawnTimer = mSpawnRate;
	}

	mPlayer->Update(timer);
	
	for (auto & enemy : mEnemies)
	{
		enemy.Update(timer);
	}

	std::vector<Bullet*> bulletsToRemove;
	std::vector<EnemyShip*> enemiesToRemove;

	for (auto & bullet : mPlayer->Bullets) {
		for (auto & enemy : mEnemies) {
			if (enemy.ContainsPoint(bullet.GetPosition())) {
				bulletsToRemove.push_back(&bullet);
				enemiesToRemove.push_back(&enemy);
				break;
			}
		}
	}

	for (auto b : bulletsToRemove) {
		for (int i = 0; i < mPlayer->Bullets.size(); i++) {
			if (&mPlayer->Bullets.at(i) == b) {
				mPlayer->Bullets.erase(mPlayer->Bullets.begin() + i);
			}
		}
	}

	for (auto e : enemiesToRemove) {
		for (int i = 0; i < mEnemies.size(); i++) {
			if (&mEnemies.at(i) == e) {
				mEnemies.erase(mEnemies.begin() + i);
			}
		}
	}
}

// Renders one frame using the vertex and pixel shaders.
void SpriteBatchSceneRenderer::Render()
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}
	auto viewport = m_deviceResources->GetScreenViewport();
	Matrix cameraTransform(Matrix::CreateTranslation(-mPlayer->GetPosition().x + viewport.Width/2.0f, -mPlayer->GetPosition().y + viewport.Height/2.0f, 0.0f));
	mSpriteBatch->Begin(SpriteSortMode::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, nullptr, cameraTransform);

	for (auto star : mStars)
	{
		RECT rect = RECT{ (int)star.x, (int)star.y, (int)star.x + (int)star.z, (int)star.y + (int)star.z };
		mSpriteBatch->Draw(mBlankTexture.Get(), rect, Colors::White);
	}

	DrawWorldBorder();

	for (auto & enemy : mEnemies)
	{
		enemy.Draw(mSpriteBatch);
	}

	mPlayer->Draw(mSpriteBatch);

	mSpriteBatch->End();

	mSpriteBatch->Begin();

	auto leftThumbstick = mThumbsticks->GetLeftThumbstickCenter();
	if (leftThumbstick != Vector2::Zero) {
		Vector2 position(leftThumbstick - Vector2(mThumbstickTextureWidth / 2.0f, mThumbstickTextureHeight / 2.0f));
		mSpriteBatch->Draw(mThumbstickTexture.Get(), position, Colors::Green);
	}

	auto rightThumbstick = mThumbsticks->GetRightThumbstickCenter();
	if (rightThumbstick != Vector2::Zero) {
		mSpriteBatch->Draw(mThumbstickTexture.Get(), rightThumbstick - Vector2(mThumbstickTextureWidth / 2.0f, mThumbstickTextureHeight / 2.0f), Colors::Blue);
	}

	mSpriteBatch->End();
}

void SpriteBatchSceneRenderer::DrawWorldBorder() {
	RECT r = RECT{
		-mWorldWidth / 2 - mWorldBorderThickness,
		-mWorldHeight / 2 - mWorldBorderThickness,
		-mWorldWidth / 2 - mWorldBorderThickness + mWorldBorderThickness,
		-mWorldHeight / 2 - mWorldBorderThickness + mWorldHeight + mWorldBorderThickness * 2 };
	mSpriteBatch->Draw(mBlankTexture.Get(), r, Colors::Red);

	r = RECT {
		-mWorldWidth / 2 - mWorldBorderThickness,
		-mWorldHeight / 2 - mWorldBorderThickness,
		-mWorldWidth / 2 - mWorldBorderThickness + mWorldWidth + mWorldBorderThickness * 2,
		-mWorldHeight / 2 - mWorldBorderThickness + mWorldBorderThickness };
	mSpriteBatch->Draw(mBlankTexture.Get(), r, Colors::Red);

	r = RECT{
		mWorldWidth / 2,
		-mWorldHeight / 2 - mWorldBorderThickness,
		mWorldWidth / 2 + mWorldBorderThickness,
		-mWorldHeight / 2 - mWorldBorderThickness + mWorldHeight + mWorldBorderThickness * 2 };
	mSpriteBatch->Draw(mBlankTexture.Get(), r, Colors::Red);

	r = RECT{
		-mWorldWidth / 2 - mWorldBorderThickness,
		mWorldHeight / 2,
		-mWorldWidth / 2 - mWorldBorderThickness + mWorldWidth + mWorldBorderThickness * 2,
		mWorldHeight / 2 + mWorldBorderThickness };
	mSpriteBatch->Draw(mBlankTexture.Get(), r, Colors::Red);
}

void SpriteBatchSceneRenderer::CreateDeviceDependentResources()
{
	m_loadingComplete = true;
}

void SpriteBatchSceneRenderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
}

void SpriteBatchSceneRenderer::QueueEvent(std::shared_ptr<InputEvent>& event) {
	mThumbsticks->QueueEvent(event);
}