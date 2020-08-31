#pragma once

#define USE_IMGUI (true)

#if USE_IMGUI

#include"imgui.h"
// DirectX
#include <stdio.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <vector>
#include <filesystem>
#include <array>
#include <DirectXMath.h>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace ImGuiSeting
{
	IMGUI_IMPL_API bool     ImGui_ImplDX11_Init(ID3D11Device* device, ID3D11DeviceContext* device_context);
	IMGUI_IMPL_API void     ImGui_ImplDX11_Shutdown();
	IMGUI_IMPL_API void     ImGui_ImplDX11_NewFrame();
	IMGUI_IMPL_API void     ImGui_ImplDX11_RenderDrawData(ImDrawData* draw_data);

	// Use if you want to reset your rendering device without losing ImGui state.
	IMGUI_IMPL_API void     ImGui_ImplDX11_InvalidateDeviceObjects();
	IMGUI_IMPL_API bool     ImGui_ImplDX11_CreateDeviceObjects();

	IMGUI_IMPL_API bool     ImGui_ImplWin32_Init(void* hwnd);
	IMGUI_IMPL_API void     ImGui_ImplWin32_Shutdown();
	IMGUI_IMPL_API void     ImGui_ImplWin32_NewFrame();
}

class [[maybe_unused]] ImguiTool
{
private:
	ImguiTool() = delete;
	~ImguiTool() = delete;
public:
	ImguiTool(const ImguiTool&) = delete;
	ImguiTool(ImguiTool&&) noexcept = delete;
	auto& operator=(const ImguiTool&) = delete;
	auto& operator=(ImguiTool&&) noexcept = delete;

private:
	static inline void ShowHelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(desc);
	}
public:
	// �\���������ꏊ�̉��ɐ錾����i�⏕�������}�E�X���u(�H)�v�Ɏ����Ă���Ɠ��e���\�������j
	static inline void ShowHelp(const char* help_coments)
	{
		assert(is_init && "����������Ă��Ȃ�");

		ImGui::SameLine(); ShowHelpMarker(help_coments);
	}

	[[nodiscard]] static inline bool InitImgui(void* hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& font_name = nullptr)
	{
		bool rv { true };

		assert(!is_init && "���ɏ���������Ă���");

		// Setup Dear ImGui context
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			auto& io = ImGui::GetIO(); (void)io;
		}

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		rv = ImGuiSeting::ImGui_ImplWin32_Init(hwnd);
		rv = ImGuiSeting::ImGui_ImplDX11_Init(pDevice, pDeviceContext);

		if (!rv)	return false;

		// ���{��t�H���g�ݒ�
		if (!font_name.empty())
		{
			auto& io { ImGui::GetIO() };
			io.Fonts->AddFontFromFileTTF(font_name.data(), 20.f, nullptr,
										 io.Fonts->GetGlyphRangesJapanese());
		}

		OutputDebugString(L"ImGui�̏���������\n");

		is_init = true;

		return true;
	}

	static inline void NewFrameImgui()
	{
		assert(is_init && "����������Ă��Ȃ�");

		ImGuiSeting::ImGui_ImplDX11_NewFrame();
		ImGuiSeting::ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	static inline void ShutdownImgui()
	{
		assert(is_init && "����������Ă��Ȃ�");

		ImGuiSeting::ImGui_ImplDX11_Shutdown();
		ImGuiSeting::ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		is_init = false;

		OutputDebugString(L"ImGui�̏I����������\n");
	}

	static inline void RenderImgui()
	{
		assert(is_init && "����������Ă��Ȃ�");

		ImGui::EndFrame();
		ImGui::Render();
		ImGuiSeting::ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

private:
	static inline bool is_init { false };
};

#endif // USE_IMGUI