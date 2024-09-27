#pragma once
#include "Sprite.h"
#include "GameStatus.h"
#include <vector>

#include "FileOutPut.h"



class PopEditor
{
public:
	PopEditor();
	~PopEditor();

	void Update();
	void Draw();

	std::vector<PopData> Load();
private:
	void DataInfo();

	std::vector<PopData> Load(const std::string& fileName);
	void Save(const std::string& fileName);
private:
	int32_t gameTime_;
	int32_t currentIndex_;
	int32_t prevCurrentIndex_;
	std::vector<PopData> data_;
	PopData* currentData_ = nullptr;

	FileOutPut outPut_;

	std::string openFileName_;	//現在開いているファイル名
	std::string saveFileName_;	//保存するファイル名
	std::string loadFileName_;	//読み込むファイル名
	std::vector<std::string> fileNames_;
};

