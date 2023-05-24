#include "JsonLoader.h"
#include "json.hpp"
#include <fstream>
#include <cassert>

JsonLoader* JsonLoader::GetInstance()
{
	static JsonLoader instance;
	return &instance;
}

JsonLoader::JsonLoader()
{
	kDefaultBaseDirectory = "Resources/JSON/";
}

void JsonLoader::LoadFile(std::string fileName)
{
	//�A�����ăt���p�X�𓾂�
	const std::string fullpath = kDefaultBaseDirectory + fileName;

	//�t�@�C���X�g���[��
	std::ifstream file;
	//�t�@�C�����J��
	file.open(fullpath);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	//��
	file >> deserialized;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"�𕶎���Ƃ��Ď擾
	std::string name =
		deserialized["name"].get<std::string>();
	//�����d����ʃf�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	

}
