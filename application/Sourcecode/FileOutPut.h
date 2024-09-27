#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <assert.h>
class FileOutPut
{
public:
	FileOutPut();
	~FileOutPut();

	void SetfileDir(const std::string& fileDir) { fileDir_ = fileDir; }
	void Begin(const std::string& fileName);
	void End();
	template<typename T>
	void Output(const std::string& itemName, T value);
	void Space();
	void Text(const std::string& itemName);
private:
	std::string fileDir_;
	std::ofstream* writing_file_ptr;
};

template<typename T>
inline void FileOutPut::Output(const std::string& itemName, T value)
{
	assert(writing_file_ptr);
	*writing_file_ptr << itemName.c_str() << " " << value << std::endl;
}
