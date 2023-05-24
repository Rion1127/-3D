#pragma once
#include <string>
class JsonLoader
{
private:
	std::string kDefaultBaseDirectory;

public:
	static JsonLoader* GetInstance();
	JsonLoader();


	void LoadFile(std::string fileName);
};

