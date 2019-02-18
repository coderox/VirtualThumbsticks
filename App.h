#pragma once

#include "App.xaml.g.h"
#include "DirectXPage.h"

namespace winrt::VirtualThumbsticksSandbox::implementation
{
	struct App : AppT<App>
	{
		App();

		void OnLaunched(winrt::Windows::ApplicationModel::Activation::LaunchActivatedEventArgs const& e);
		void OnSuspending(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::ApplicationModel::SuspendingEventArgs const& e);
		void OnResuming(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);

	private:
		std::unique_ptr<DirectXPage> m_directXPage;
	};
}
