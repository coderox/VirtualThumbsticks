#include "pch.h"
#include "DirectXPage.h"

using namespace winrt::VirtualThumbsticksSandbox::implementation;

using namespace winrt;
using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::Windows::UI::Xaml::Controls::Primitives;
using namespace winrt::Windows::UI::Xaml::Data;
using namespace winrt::Windows::UI::Xaml::Input;
using namespace winrt::Windows::UI::Xaml::Interop;
using namespace winrt::Windows::UI::Xaml::Media;
using namespace winrt::Windows::UI::Xaml::Navigation;

App::App()
{
	InitializeComponent();
	
	Suspending({ this, &App::OnSuspending });
	Resuming({ this, &App::OnResuming });
}

void App::OnLaunched(LaunchActivatedEventArgs const& e)
{
#if _DEBUG
	if (IsDebuggerPresent())
	{
		//DebugSettings->EnableFrameRateCounter = true;
	}
#endif

	if (m_directXPage == nullptr)
	{
		m_directXPage = make<DirectXPage>();
	}

	if (e.PreviousExecutionState() == ApplicationExecutionState::Terminated())
	{
		m_directXPage.LoadInternalState(ApplicationData::Current().LocalSettings().Values());
	}

	// Place the page in the current window and ensure that it is active.
	Window::Current().Content(m_directXPage);
	Window::Current().Activate();
}

void App::OnSuspending(IInspectable const& sender, SuspendingEventArgs const& e)
{
	(void) sender;	// Unused parameter
	(void) e;	// Unused parameter

	m_directXPage.SaveInternalState(ApplicationData::Current().LocalSettings().Values());
}

void App::OnResuming(IInspectable const& sender, IInspectable const& args)
{
	(void) sender; // Unused parameter
	(void) args; // Unused parameter

	m_directXPage.LoadInternalState(ApplicationData::Current().LocalSettings().Values());
}
