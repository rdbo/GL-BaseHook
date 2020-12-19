#include <pch.h>
#include <base.h>

//Data
HMODULE           Base::Data::hModule      = (HMODULE)NULL;
HWND              Base::Data::hWindow      = (HWND)NULL;
mem::voidptr_t    Base::Data::pSwapBuffers = (mem::voidptr_t)NULL;
SwapBuffers_t     Base::Data::oSwapBuffers = (SwapBuffers_t)NULL;
WndProc_t         Base::Data::oWndProc     = (WndProc_t)NULL;
#if defined(MEM_86)
mem::size_t       Base::Data::szSwapBuffers = 5;
#elif defined(MEM_64)
mem::size_t       Base::Data::szSwapBuffers = 13;
#endif
UINT              Base::Data::WmKeys[0xFF];
bool              Base::Data::Detached   = false;
bool              Base::Data::ShowMenu   = true;
bool              Base::Data::InitImGui  = false;


//Functions

bool Base::Init()
{
	Hooks::Init();
	return true;
}

bool Base::Shutdown()
{
	Hooks::Shutdown();
	return true;
}

bool Base::Detach()
{
	Base::Shutdown();
	CreateThread(nullptr, 0, ExitThread, Data::hModule, 0, nullptr);
	return true;
}