#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "Mesh.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 20.0f);
	ReferenceFPS = vec3(0.0f, 0.0f, 20.0f);
	Reference = vec3(0.0f, 0.0f, 5.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{

	vec3 newPos(0,0,0);
	float speed = 4.0f * dt;
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	/*if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
		FrontObjectView();*/

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		ReferenceFPS = Position;

		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y += speed; 
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) App->input->LogToConsole("E");

		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y -= speed;
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) App->input->LogToConsole("R");

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed; 
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) App->input->LogToConsole("W");

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed; 
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) App->input->LogToConsole("S");

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed; 
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) App->input->LogToConsole("A");

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) App->input->LogToConsole("D");

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.5f;

		Position -= ReferenceFPS;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = ReferenceFPS + Z * length(Position);

		ReferenceFPS = Position;
	}

	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT && (App->input->GetKey(SDL_SCANCODE_LALT)))
	{
		if (App->input->GetMouseXMotion() < 0)
		{
			newPos += X * -App->input->GetMouseXMotion()/20;
		}
		else if (App->input->GetMouseXMotion() > 0)
		{
			newPos -= X * App->input->GetMouseXMotion()/20;
		}

		if (App->input->GetMouseYMotion() > 0)
		{
			newPos += Y * App->input->GetMouseYMotion()/20;
		}
		else if (App->input->GetMouseYMotion() < 0)
		{
			newPos -= Y * -App->input->GetMouseYMotion()/20;
		}
	}

	// Wheel zoom ------------------

	if (App->input->GetMouseZ() > 0) newPos -= Z * speed * 10;
	else if (App->input->GetMouseZ() < 0) newPos += Z * speed * 10;

	Position += newPos;

	if (App->editor->selectedNode)
	{
		ComponentTransform* meshPosition = dynamic_cast<ComponentTransform*>(App->editor->selectedNode->GetComponent(Component::Type::TRANSFORM));
		Reference = meshPosition->GetCombinedPosition(App->editor->selectedNode);
	}
	else Reference += newPos;

	// Focus object ----------------

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && App->editor->selectedNode &&App->editor->selectedNode->GetComponent(Component::Type::MESH))
	{
		FocusObject();
	}

	// Mouse motion ----------------

	if(App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && (App->input->GetKey(SDL_SCANCODE_LALT)) && App->input->GetMouseButton(SDL_BUTTON_RIGHT) != KEY_REPEAT)
	{

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.5f;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if(Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::FocusObject()
{
	OBBmaxVector = (0, 0, 0);
	OBBminVector = (0, 0, 0);

	ComponentMesh* meshOBB = dynamic_cast<ComponentMesh*>(App->editor->selectedNode->GetComponent(Component::Type::MESH));
	for (int i = 0; i < meshOBB->GetMesh()->mPosition.size(); i++)
	{
		//OBBmaxVector
		if (meshOBB->GetMesh()->mPosition[i].x > OBBmaxVector.x) OBBmaxVector.x = meshOBB->GetMesh()->mPosition[i].x;
		if (meshOBB->GetMesh()->mPosition[i].y > OBBmaxVector.y) OBBmaxVector.y = meshOBB->GetMesh()->mPosition[i].y;
		if (meshOBB->GetMesh()->mPosition[i].z > OBBmaxVector.z) OBBmaxVector.z = meshOBB->GetMesh()->mPosition[i].z;

		//OBBminVector
		if (meshOBB->GetMesh()->mPosition[i].x < OBBminVector.x) OBBminVector.x = meshOBB->GetMesh()->mPosition[i].x;
		if (meshOBB->GetMesh()->mPosition[i].y < OBBminVector.y) OBBminVector.y = meshOBB->GetMesh()->mPosition[i].y;
		if (meshOBB->GetMesh()->mPosition[i].z < OBBminVector.z) OBBminVector.z = meshOBB->GetMesh()->mPosition[i].z;
	}


	float dx = OBBmaxVector.x - OBBminVector.x;
	float dy = OBBmaxVector.y - OBBminVector.y;
	float dz = OBBmaxVector.z - OBBminVector.z;

	focusDistance = sqrt(dx * dx + dy * dy + dz * dz);

	if (App->editor->selectedNode->transform->scale.x > App->editor->selectedNode->transform->scale.y && App->editor->selectedNode->transform->scale.x > App->editor->selectedNode->transform->scale.z)
	{
		focusScale = App->editor->selectedNode->transform->scale.x;
	}
	else if (App->editor->selectedNode->transform->scale.y > App->editor->selectedNode->transform->scale.z && App->editor->selectedNode->transform->scale.y > App->editor->selectedNode->transform->scale.z)
	{
		focusScale = App->editor->selectedNode->transform->scale.y;
	}
	else if (App->editor->selectedNode->transform->scale.z > App->editor->selectedNode->transform->scale.x && App->editor->selectedNode->transform->scale.z > App->editor->selectedNode->transform->scale.y)
	{
		focusScale = App->editor->selectedNode->transform->scale.z;
	}
	else focusScale = App->editor->selectedNode->transform->scale.x;

	Position = (App->editor->selectedNode->transform->position) + (0, 0, focusDistance * 2 + focusScale);

	LookAt(App->editor->selectedNode->transform->position);
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

void ModuleCamera3D::FrontObjectView()
{
	GameObject* object = App->editor->selectedNode;
	vec3 destination = { 0,0,0 };
	vec3 newPos = { 0,0,0 };

	if (object != nullptr)
	{
		destination = object->transform->GetPosition();

		newPos = vec3(destination.x, destination.y, destination.z);

		Position = newPos + vec3(0, 10, -30);
		LookAt(newPos);
	}
}
