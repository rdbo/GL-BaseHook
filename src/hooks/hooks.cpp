#include <pch.h>
#include <base.h>

//Functions

bool Base::Hooks::Init()
{
	Data::pSwapBuffers = mem::in::get_symbol(mem::in::get_module("opengl32.dll"), "wglSwapBuffers");
#	if defined(MEM_86)
	Data::oSwapBuffers = (SwapBuffers_t)mem::in::detour_trampoline(Data::pSwapBuffers, Hooks::SwapBuffers, Data::szSwapBuffers, mem::MEM_DT_M1);
#	elif defined(MEM_64)
	Data::oSwapBuffers = (SwapBuffers_t)mem::in::detour_trampoline(Data::pSwapBuffers, Hooks::SwapBuffers, Data::szSwapBuffers);
#	endif
	//Data::oWndProc  = (WndProc_t)SetWindowLongPtr(Data::hWindow, WNDPROC_INDEX, (LONG_PTR)Hooks::WndProc); //<- Will be hooked inside the SwapBuffers function, as Data::hWindow will be got there
	return true;
}

bool Base::Hooks::Shutdown()
{
	if (Data::InitImGui)
	{
		ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	mem::in::detour_restore(Data::pSwapBuffers, (mem::byte_t*)Data::oSwapBuffers, Data::szSwapBuffers);
	SetWindowLongPtr(Data::hWindow, WNDPROC_INDEX, (LONG_PTR)Data::oWndProc);

	return true;
}