#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\SpriteBatchSceneRenderer.h"
#include "Content\SampleFpsTextRenderer.h"
#include "..\InputEvent.h"

// Renders Direct2D and 3D content on the screen.
namespace VirtualThumbsticksSandbox
{
	class VirtualThumbsticksSandboxMain : public DX::IDeviceNotify
	{
	public:
		VirtualThumbsticksSandboxMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~VirtualThumbsticksSandboxMain();
		void CreateWindowSizeDependentResources();
		void StartRenderLoop();
		void StopRenderLoop();
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

		void QueueEvent(std::shared_ptr<InputEvent>& args);

	private:
		void ProcessInput();
		void Update();
		bool Render();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		std::unique_ptr<SpriteBatchSceneRenderer> m_sceneRenderer;
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Concurrency::critical_section m_criticalSection;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		// Track current input pointer position.
		float m_pointerLocationX;
	};
}