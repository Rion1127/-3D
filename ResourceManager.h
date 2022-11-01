#pragma once
#include <string>
#include "Object3d.h"
class ResourceManager
{
public:
	static void LoadAllModels();
	static void LoadAllSound();
	//static Object3d* Get(std::string key);

	//static std::map<std::string, Object3d> modelMap;

	static void RegisterModel(std::string modelName, std::string key);

	static void ClearModelMap();
private:

};

