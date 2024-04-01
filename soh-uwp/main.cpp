/*
    SDL_winrt_main_NonXAML.cpp, placed in the public domain by David Ludwig  3/13/14
*/

/* At least one file in any SDL/WinRT app appears to require compilation
   with C++/CX, otherwise a Windows Metadata file won't get created, and
   an APPX0702 build error can appear shortly after linking.

   The following set of preprocessor code forces this file to be compiled
   as C++/CX, which appears to cause Visual C++ 2012's build tools to
   create this .winmd file, and will help allow builds of SDL/WinRT apps
   to proceed without error.

   If other files in an app's project enable C++/CX compilation, then it might
   be possible for SDL_winrt_main_NonXAML.cpp to be compiled without /ZW,
   for Visual C++'s build tools to create a winmd file, and for the app to
   build without APPX0702 errors.  In this case, if
   SDL_WINRT_METADATA_FILE_AVAILABLE is defined as a C/C++ macro, then
   the #error (to force C++/CX compilation) will be disabled.

   Please note that /ZW can be specified on a file-by-file basis.  To do this,
   right click on the file in Visual C++, click Properties, then change the
   setting through the dialog that comes up.
*/

/* Prevent MSVC++ from warning about threading models when defining our
   custom WinMain.  The threading model will instead be set via a direct
   call to Windows::Foundation::Initialize (rather than via an attributed
   function).

   To note, this warning (C4447) does not seem to come up unless this file
   is compiled with C++/CX enabled (via the /ZW compiler flag).
*/
#ifdef _MSC_VER
#pragma warning(disable : 4447)
#endif

/* Make sure the function to initialize the Windows Runtime gets linked in. */
#ifdef _MSC_VER
#pragma comment(lib, "runtimeobject.lib")
#endif

#include <Windows.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.UI.ViewManagement.Core.h>
#include <winrt/Windows.Graphics.Display.Core.h>
#include <winrt/Windows.Gaming.Input.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.System.h>

using namespace winrt::Windows;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Graphics::Display::Core;
using namespace winrt::Windows::Foundation::Numerics;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Composition;
using namespace winrt::Windows::Storage;

#include "../libultraship/src/graphic/Fast3D/wininfo.h"

#include "SDL2/SDL.h"

// Window state vars
static int width, height;

static double refreshRate;
static float scale;


void* WinInfo::getCurrentWindow() {
    void* ptr = nullptr;
    while (ptr == nullptr) {
        ptr = winrt::get_abi(CoreWindow::GetForCurrentThread());
        Sleep(10);
    }

    return ptr;
}

int WinInfo::getHostWidth() {
    return width;
}

int WinInfo::getHostHeight() {
    return height;
}

double WinInfo::getHostRefresh() {
    return refreshRate;
}

std::string WinInfo::getSavePath() {
    return winrt::to_string(ApplicationData::Current().LocalFolder().Path());
}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Initialize height/width for later usage
    HdmiDisplayInformation hdi = HdmiDisplayInformation::GetForCurrentView();
    if (hdi)
    {
        width = hdi.GetCurrentDisplayMode().ResolutionWidthInRawPixels();
        height = hdi.GetCurrentDisplayMode().ResolutionHeightInRawPixels();
        refreshRate = hdi.GetCurrentDisplayMode().RefreshRate();
        scale = ((float)width / 1920.0f) * 1.75f;
    }
    else {
        width = 1920;
        height = 1080;
        scale = 1.0;
    }

    return SDL_WinRTRunApp(SDL_main, nullptr);
}