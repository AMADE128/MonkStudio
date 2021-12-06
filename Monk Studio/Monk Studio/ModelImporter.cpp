
#include "ModelImporter.h"
#include "parson.h"
#include "GameObject.h"

#include <string>

void ModelImporter::Save(GameObject* root)
{
	JSON_Value* file = json_value_init_object();
	JSON_Object* obj = json_value_get_object(file);

	JSON_Value* val = json_value_init_array();
	root->SaveObjectData(json_value_get_array(val));
	json_object_set_value(obj, "Game Objects", val);

	std::string nameLibPath = "Library/Models/" + std::to_string(root->uid) + ".body";
	json_serialize_to_file_pretty(file, nameLibPath.c_str());
	//dir.clear();

	//Free memory
	json_value_free(file);
}

void ModelImporter::Load(const char* name)
{
}
