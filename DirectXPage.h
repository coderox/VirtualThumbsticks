#pragma once

#include <winrt/Windows.Graphics.Display.h>

#include "DirectXPage.g.h"

#include "Common\DeviceResources.h"
#include "VirtualThumbsticksSandboxMain.h"

namespace winrt::VirtualThumbsticksSandbox::implementation
{
	struct DirectXPage : DirectXPageT<DirectXPage>
	{
		DirectXPage();
		~DirectXPage();

		void SaveInternalState(winrt::Windows::Foundation::Collections::IPropertySet const& state);
		void LoadInternalState(winrt::Windows::Foundation::Collections::IPropertySet const& state);

	private:
		// XAML low-level rendering event handler.
		void OnRendering(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& args);
		DirectX::SimpleMath::Vector2 ConvertPositionToVectorHandleDpi(winrt::Windows::Foundation::Point const& position);

		// Window event handlers.
		void OnVisibilityChanged(winrt::Windows::UI::Core::CoreWindow const& sender, winrt::Windows::UI::Core::VisibilityChangedEventArgs const& args);

		// DisplayInformation event handlers.
		void OnDpiChanged(winrt::Windows::Graphics::Display::DisplayInformation const& sender, winrt::Windows::Foundation::IInspectable const& args);
		void OnOrientationChanged(winrt::Windows::Graphics::Display::DisplayInformation const& sender, winrt::Windows::Foundation::IInspectable const& args);
		void OnDisplayContentsInvalidated(winrt::Windows::Graphics::Display::DisplayInformation const& sender, winrt::Windows::Foundation::IInspectable const& args);

		// Other event handlers.
		void AppBarButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
		void OnCompositionScaleChanged(winrt::Windows::UI::Xaml::Controls::SwapChainPanel const& sender, winrt::Windows::Foundation::IInspectable const& args);
		void OnSwapChainPanelSizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::SizeChangedEventArgs const& e);

		// Track our independent input on a background worker thread.
		winrt::Windows::Foundation::IAsyncAction m_inputLoopWorker;
		winrt::Windows::UI::Core::CoreIndependentInputSource m_coreInput;

		// Independent input handling functions.
		void OnPointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Core::PointerEventArgs const& e);
		void OnPointerMoved(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Core::PointerEventArgs const& e);
		void OnPointerReleased(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Core::PointerEventArgs const& e);

		// Resources used to render the DirectX content in the XAML page background.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<::VirtualThumbsticksSandbox::VirtualThumbsticksSandboxMain> m_main;
		bool m_windowVisible;
		float mDpi;
	};
}

