#include <pch.h>
#include <base.h>

static bool ContextCreated = false;
static HGLRC g_Context = (HGLRC)NULL;

BOOL __stdcall Base::Hooks::SwapBuffers(_In_ HDC hdc)
{
	Data::hWindow = WindowFromDC(hdc);
	HGLRC oContext = wglGetCurrentContext();

	if (!Data::oWndProc)
		Data::oWndProc = (WndProc_t)SetWindowLongPtr(Data::hWindow, WNDPROC_INDEX, (LONG_PTR)Hooks::WndProc);

	if (!ContextCreated)
	{
		g_Context = wglCreateContext(hdc);
		wglMakeCurrent(hdc, g_Context);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);

		glOrtho(0, m_viewport[2], m_viewport[3], 0, 1, -1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0, 0, 0, 1);
		ContextCreated = true;
	}

	wglMakeCurrent(hdc, g_Context);

	if (!Data::InitImGui && ContextCreated)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplWin32_Init(Data::hWindow);
		ImGui_ImplOpenGL2_Init();
		Data::InitImGui = true;
	}

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (Data::ShowMenu)
	{
		ImGui::Begin("ImGui Window");
		ImGui::Text("Test ImGUI Window");
		if (ImGui::Button("Detach"))
		{
			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
			wglMakeCurrent(hdc, oContext);
			wglDeleteContext(g_Context);
			Base::Detach();
			return Data::oSwapBuffers(hdc);
		}
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	wglMakeCurrent(hdc, oContext);
	return Data::oSwapBuffers(hdc);
}