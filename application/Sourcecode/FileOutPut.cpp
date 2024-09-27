#include "FileOutPut.h"


FileOutPut::FileOutPut()
{
	fileDir_ = "application/Resources";
}

FileOutPut::~FileOutPut()
{
}

void FileOutPut::Begin(const std::string& string)
{
	assert((writing_file_ptr == nullptr));
	std::string saveDir = fileDir_;
	saveDir.append(string.c_str());
	saveDir += ".txt";
	
	writing_file_ptr = new std::ofstream;
	writing_file_ptr->open(saveDir, std::ios::out);
}

void FileOutPut::End()
{
	writing_file_ptr->close();
	writing_file_ptr = nullptr;
	delete writing_file_ptr;
}

void FileOutPut::Space()
{
	assert(writing_file_ptr);
	*writing_file_ptr << std::endl;
}

void FileOutPut::Text(const std::string& itemName)
{
	assert(writing_file_ptr);
	*writing_file_ptr << itemName.c_str() << std::endl;
}
