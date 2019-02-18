#pragma once

// Should this really be necessary?
#define DIRECTX_NO_WFRECT

#include <wrl.h>
#include <wrl/client.h>
#include <dxgi1_4.h>
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <d2d1effects_2.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <memory>
#include "App.h"

#include <SpriteBatch.h>
#include <SimpleMath.h>
#include <Common\DirectXHelper.h>

#include "Ship.h"
#include "Bullet.h"
#include "VirtualThumbsticks.h"

#include <windows.h>
#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.Graphics.Display.h>