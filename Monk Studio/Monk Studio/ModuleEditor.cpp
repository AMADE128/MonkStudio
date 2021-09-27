#include "Application.h"
#include "ModuleEditor.h"
#include "imgui.h"
#include "ModuleInterface.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleEditor::~ModuleEditor()
{

}

bool ModuleEditor::Init()
{
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}


bool ModuleEditor::CleanUp()
{
	return UPDATE_CONTINUE;
}

