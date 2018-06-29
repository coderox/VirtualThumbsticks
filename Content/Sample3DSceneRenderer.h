#pragma once

#include "..\Common\DeviceResources.h"
#include "..\Common\StepTimer.h"

#include "..\Ship.h"
#include <SpriteBatch.h>
#include "..\VirtualThumbsticks.h"

namespace VirtualThumbsticksSandbox
{
	// This sample renderer instantiates a basic rendering pipeline.
	class Sample3DSceneRenderer
	{
	public:
		Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~Sample3DSceneRenderer();
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();
		void ProcessInput();
		void Update(DX::StepTimer const& timer);
		void Render();

		void QueueEvent(std::shared_ptr<InputEvent>& event);

	private:
		void DrawWorldBorder();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mBlankTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mThumbstickTexture;

		// Variables used with the rendering loop.
		bool m_loadingComplete;
		int mNumStars;
		int mWorldWidth;
		int mWorldHeight;
		float mGraphicsWidthHalf;
		float mGraphicsHeightHalf;
		int mWorldBorderThickness;
		int mWorldBorderThicknessDoubled;
		UINT mThumbstickTextureWidth;
		UINT mThumbstickTextureHeight;

		std::shared_ptr<PlayerShip> mPlayer;
		//std::shared_ptr<EnemyShip> mEnemy;
		std::shared_ptr<DirectX::SpriteBatch> mSpriteBatch;
		std::shared_ptr<VirtualThumbsticksSandbox::VirtualThumbsticks> mThumbsticks;
		std::vector<DirectX::SimpleMath::Vector3> mStars;
		std::vector<EnemyShip> mEnemies;

		double mSpawnTimer = 0;
		double mSpawnRate = 2000;
	};
}

