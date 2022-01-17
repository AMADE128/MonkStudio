#include "AboutMenu.h"

#include <SDL_syswm.h>
#include <GL\glew.h>

#include "assimp/version.h"
#include "DevIL/include/IL/il.h"
#include "PhysFS/include/physfs.h"

#include "Profiling.h"

AboutMenu::AboutMenu() : Menu(false)
{
}

AboutMenu::~AboutMenu()
{
}

bool AboutMenu::Update(float dt)
{
	if (ImGui::Begin("About Monk Studio", &active))
	{
		ImGui::TextWrapped("Monk Studio v1");
		ImGui::NewLine();
		ImGui::TextWrapped
		(" The purpose of this project is to create a friendly-user, well optimized Game Engine for the subject VideoGame Engines from the 3rd year. The short term plan is to implement the basic features of a Game Engine, such as loading models, working in a 3D environment, etc..."
			"More specifically, we want to develop a good Audio engine, capable of loading audios, edit multiple parameters and create effects with them."
		);
		ImGui::TextWrapped("By Himar Bravo, Marc Pavon & Pol Vazquez, forked from Ragnar Engine v0.8");
		ImGui::NewLine();

		ImGui::TextWrapped("3rd Party Libraries used:");
		SDL_version sdlVer;
		SDL_GetVersion(&sdlVer);
		ImGui::TextWrapped("-SDL %d.%d.%d", sdlVer.major, sdlVer.minor, sdlVer.patch);
		ImGui::TextWrapped("-Glew %s", glewGetString(GLEW_VERSION));
		ImGui::TextWrapped("-ImGui %s", ImGui::GetVersion());
		ImGui::TextWrapped("-MathGeoLib 1.5");
		ImGui::TextWrapped("-OpenGl version %s", glGetString(GL_VERSION));
		ImGui::TextWrapped("-Assimp version %d.%d", aiGetVersionMajor(), aiGetVersionMinor());
		ImGui::TextWrapped("-DevIL version 1.8.0");
		ImGui::TextWrapped("-PhysFS version %d.%d.%d", PHYSFS_VER_MAJOR, PHYSFS_VER_MINOR, PHYSFS_VER_PATCH);
		ImGui::TextWrapped("WWise version 20021.1.5");
		ImGui::TextWrapped("OpenAl verison 1.1");
		ImGui::TextWrapped("AudioFile 1.1.0");

		ImGui::NewLine();
		ImGui::TextWrapped("License:");
		ImGui::NewLine();
		ImGui::TextWrapped("MIT License");
		ImGui::NewLine();
		ImGui::TextWrapped("Copyright(c) 2021 Himar Bravo, Marc Pavon & Pol Vazquez");
		ImGui::NewLine();
		ImGui::TextWrapped(
			"Permission is hereby granted, free of charge, to any person obtaining a copy"
			"of this softwareand associated documentation files(the 'Software'), to deal"
			"in the Software without restriction, including without limitation the rights"
			"to use, copy, modify, merge, publish, distribute, sublicense, and /or sell"
			"copies of the Software, and to permit persons to whom the Software is"
			"furnished to do so, subject to the following conditions : "
		);
		ImGui::NewLine();
		ImGui::TextWrapped(
			"The above copyright noticeand this permission notice shall be included in all"
			"copies or substantial portions of the Software."
		);
		ImGui::NewLine();
		ImGui::TextWrapped(
			"THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR"
			"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,"
			"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE"
			"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER"
			"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,"
			"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE"
			"SOFTWARE."
		);
	}
	ImGui::End();

	return true;
}