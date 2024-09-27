#include "PopEditor.h"
#include <imgui.h>
#include "Util.h"

PopEditor::PopEditor()
{
	outPut_.SetfileDir("application/Resources/PopData/");

	gameTime_ = 120;

	fileNames_ = FindFileNames("application/Resources/PopData/", ".txt", false);
	loadFileName_ = fileNames_[0];
}

PopEditor::~PopEditor()
{
	Save("AutoSave");
}

void PopEditor::Update()
{
	ImGui::Begin("PopEditor");

	ImGui::InputText("保存するファイル名", saveFileName_.data(), 20);
	if (ImGui::Button("Save", ImVec2(50, 50))) {
		Save(saveFileName_);
	}
	ImGui::SameLine();
	//クイックセーブ
	if (ImGui::Button("QuickSave", ImVec2(70, 50))) {
		Save(openFileName_);
	}
	// オブジェクトをプルダウン形式で選択可能
	if (ImGui::BeginCombo("オブジェクト", loadFileName_.c_str())) {
		for (size_t i = 0; i < fileNames_.size(); i++) {
			const bool isSame = (loadFileName_ == fileNames_[i]);
			// 選択したオブジェクト名を、選択中のアイテム名保持変数に代入
			if (ImGui::Selectable(fileNames_[i].c_str(), isSame)) {
				loadFileName_ = fileNames_[i].c_str();
			}
			// 選択したオブジェクトをプルダウンのフレームに表示
			if (isSame) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	if (ImGui::Button("Load", ImVec2(50, 50))) {
		data_ = Load(loadFileName_);
		currentData_ = &data_[0];
	}

	ImGui::End();

	DataInfo();
}

void PopEditor::Draw()
{
}

void PopEditor::DataInfo()
{
	
	ImGui::Begin("DataInfo");
	if (ImGui::Button("Add", ImVec2(50, 50))) {
		data_.emplace_back();
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete", ImVec2(50, 50))) {
		if (data_.size() > 0) data_.erase(data_.begin() + currentIndex_);
	}
	ImGui::InputInt("配列指定", &currentIndex_, 1, 1);
	int32_t max = Max(0, (int32_t)(data_.size() - 1));
	currentIndex_ = Clamp(currentIndex_,0, max);
	if (prevCurrentIndex_ != currentIndex_) {
		currentData_ = &data_[currentIndex_];
	}
	if (currentData_ != nullptr) {
		ImGui::DragInt("出現時間", &currentData_->popTime, 1.f, 0, 1000000);
		ImGui::Text("左");
		ImGui::DragInt("普通の人_左", &currentData_->popPos1.popNum_normal, 1.f, 0, 20);
		ImGui::DragInt("おでぶ_左", &currentData_->popPos1.popNum_debu, 1.f, 0, 20);
		ImGui::DragInt("グループ_左", &currentData_->popPos1.popNum_group, 1.f, 0, 20);
		ImGui::DragInt("グループ人数_左", &currentData_->popPos1.popNum_group_humans, 1.f, 0, 10);
		ImGui::Text("中央");
		ImGui::DragInt("普通の人_中央", &currentData_->popPos2.popNum_normal, 1.f, 0, 20);
		ImGui::DragInt("おでぶ_中央", &currentData_->popPos2.popNum_debu, 1.f, 0, 20);
		ImGui::DragInt("グループ_中央", &currentData_->popPos2.popNum_group, 1.f, 0, 20);
		ImGui::DragInt("グループ人数_中央", &currentData_->popPos2.popNum_group_humans, 1.f, 0, 10);
		ImGui::Text("右");
		ImGui::DragInt("普通の人_右", &currentData_->popPos3.popNum_normal, 1.f, 0, 20);
		ImGui::DragInt("おでぶ_右", &currentData_->popPos3.popNum_debu, 1.f, 0, 20);
		ImGui::DragInt("グループ_右", &currentData_->popPos3.popNum_group, 1.f, 0, 20);
		ImGui::DragInt("グループ人数_右", &currentData_->popPos3.popNum_group_humans, 1.f, 0, 10);
	}
	ImGui::End();

	prevCurrentIndex_ = currentIndex_;
}

std::vector<PopData> PopEditor::Load()
{
	std::vector<PopData> result;
	PopData* popData = nullptr;
	//読み込み
	std::ifstream file("application/Resources/PopData/PopData.txt");  // 読み込むファイルのパスを指定
	std::string line;

	std::string dir;

	while (std::getline(file, line)) {  // 1行ずつ読み込む
		std::cout << line << std::endl;

		std::stringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');
		if (key == "PopData") {
			result.emplace_back();
			popData = &result.back();
		}
		else if (key == "PopPos") {
			line_stream >> dir;
		}

		if (popData == nullptr)continue;

		if (key == "PopTime") {
			line_stream >> popData->popTime;
		}
		else if (key == "PopNum_Normal") {
			int32_t pop_normal = 0;
			line_stream >> pop_normal;

			if (dir == "Left") popData->popPos1.popNum_normal = pop_normal;
			else if (dir == "Center") popData->popPos2.popNum_normal = pop_normal;
			else if (dir == "Right") popData->popPos3.popNum_normal = pop_normal;
		}
		else if (key == "PopNum_Debu") {
			int32_t pop_debu = 0;
			line_stream >> pop_debu;

			if (dir == "Left") popData->popPos1.popNum_debu = pop_debu;
			else if (dir == "Center") popData->popPos2.popNum_debu = pop_debu;
			else if (dir == "Right") popData->popPos3.popNum_debu = pop_debu;
		}
		else if (key == "PopNum_group") {
			int32_t pop_group = 0;
			line_stream >> pop_group;

			if (dir == "Left") popData->popPos1.popNum_group = pop_group;
			else if (dir == "Center") popData->popPos2.popNum_group = pop_group;
			else if (dir == "Right") popData->popPos3.popNum_group = pop_group;
		}
		else if (key == "PopNum_group_humans") {
			int32_t group_humans = 0;
			line_stream >> group_humans;

			if (dir == "Left") popData->popPos1.popNum_group_humans = group_humans;
			else if (dir == "Center") popData->popPos2.popNum_group_humans = group_humans;
			else if (dir == "Right") popData->popPos3.popNum_group_humans = group_humans;
		}
	}
	return result;
}

std::vector<PopData> PopEditor::Load(const std::string& fileName)
{
	std::vector<PopData> result;
	PopData* popData = nullptr;

	std::string name = "application/Resources/PopData/";
	name += fileName;
	name += ".txt";

	openFileName_ = fileName;
	//読み込み
	std::ifstream file(name);  // 読み込むファイルのパスを指定
	std::string line;

	std::string dir;
	while (std::getline(file, line)) {  // 1行ずつ読み込む
		std::cout << line << std::endl;

		std::stringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');
		if (key == "PopData") {
			result.emplace_back();
			popData = &result.back();
		}
		else if (key == "PopPos") {
			line_stream >> dir;
		}

		if (popData == nullptr)continue;

		if (key == "PopTime") {
			line_stream >> popData->popTime;
		}
		else if (key == "PopNum_Normal") {
			int32_t pop_normal = 0;
			line_stream >> pop_normal;

			if (dir == "Left") popData->popPos1.popNum_normal = pop_normal;
			else if (dir == "Center") popData->popPos2.popNum_normal = pop_normal;
			else if (dir == "Right") popData->popPos3.popNum_normal = pop_normal;
		}
		else if (key == "PopNum_Debu") {
			int32_t pop_debu = 0;
			line_stream >> pop_debu;

			if (dir == "Left") popData->popPos1.popNum_debu = pop_debu;
			else if (dir == "Center") popData->popPos2.popNum_debu = pop_debu;
			else if (dir == "Right") popData->popPos3.popNum_debu = pop_debu;
		}
		else if (key == "PopNum_group") {
			int32_t pop_group = 0;
			line_stream >> pop_group;

			if (dir == "Left") popData->popPos1.popNum_group = pop_group;
			else if (dir == "Center") popData->popPos2.popNum_group = pop_group;
			else if (dir == "Right") popData->popPos3.popNum_group = pop_group;
		}
		else if (key == "PopNum_group_humans") {
			int32_t group_humans = 0;
			line_stream >> group_humans;

			if (dir == "Left") popData->popPos1.popNum_group_humans = group_humans;
			else if (dir == "Center") popData->popPos2.popNum_group_humans = group_humans;
			else if (dir == "Right") popData->popPos3.popNum_group_humans = group_humans;
		}
	}
	return result;
}

void PopEditor::Save(const std::string& fileName)
{
	outPut_.Begin(fileName);

	outPut_.Output("GameTime", gameTime_);
	outPut_.Space();

	int32_t index = 0;
	for (auto& data : data_) {
		if (data.popTime == 0)continue;
		outPut_.Output("PopData", index);

		outPut_.Output("PopTime", data.popTime);

		outPut_.Text("PopPos Left");
		outPut_.Output("PopNum_Normal", data.popPos1.popNum_normal);
		outPut_.Output("PopNum_Debu", data.popPos1.popNum_debu);
		outPut_.Output("PopNum_group", data.popPos1.popNum_group);
		outPut_.Output("PopNum_group_humans", data.popPos1.popNum_group_humans);
		outPut_.Text("PopPos Center");
		outPut_.Output("PopNum_Normal", data.popPos2.popNum_normal);
		outPut_.Output("PopNum_Debu", data.popPos2.popNum_debu);
		outPut_.Output("PopNum_group", data.popPos2.popNum_group);
		outPut_.Output("PopNum_group_humans", data.popPos2.popNum_group_humans);
		outPut_.Text("PopPos Right");
		outPut_.Output("PopNum_Normal", data.popPos3.popNum_normal);
		outPut_.Output("PopNum_Debu", data.popPos3.popNum_debu);
		outPut_.Output("PopNum_group", data.popPos3.popNum_group);
		outPut_.Output("PopNum_group_humans", data.popPos3.popNum_group_humans);

		outPut_.Space();

		index++;
	}
	
	outPut_.End();

	fileNames_ = FindFileNames("application/Resources/PopData/", ".txt", false);
}
