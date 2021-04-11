#include "Editor.h"
#include <imgui.h>

Editor::Editor(uint32_t width, uint32_t height)
{

}

auto Editor::init() -> void
{

}

auto Editor::onImGui() -> void
{
	drawMenuBar();
}

auto Editor::drawMenuBar() -> void
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				
			}

			if (ImGui::MenuItem("Open File"))
			{
			}

			if (ImGui::MenuItem("New Scene"))
			{
			}

			if (ImGui::BeginMenu("Style"))
			{
				if (ImGui::MenuItem("Dark", ""))
				{
				}
				if (ImGui::MenuItem("Black", ""))
				{
				}
				if (ImGui::MenuItem("Grey", ""))
				{
				}
				if (ImGui::MenuItem("Light", ""))
				{
				}
				if (ImGui::MenuItem("Cherry", ""))
				{
				}
				if (ImGui::MenuItem("Blue", ""))
				{
				}
				if (ImGui::MenuItem("Cinder", ""))
				{
				}
				if (ImGui::MenuItem("Classic", ""))
				{
				}
				if (ImGui::MenuItem("ClassicDark", ""))
				{
				}
				if (ImGui::MenuItem("ClassicLight", ""))
				{
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}
