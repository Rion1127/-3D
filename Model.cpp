#include <fstream>
#include <sstream>
#include "Model.h"


const std::string kBaseDirectory = "Resources/";

Obj* Obj::GetInstance()
{
	static Obj instance;
	return &instance;
}

bool Obj::loadOBJ(const std::string& modelname)
{
	const std::string filename = modelname + ".obj";
	const std::string directoryPath = kBaseDirectory + modelname + "/" + filename;
	// �t�@�C���X�g���[��
	std::ifstream file;
	// .obj�t�@�C�����J��
	file.open(directoryPath);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
		return 0;
	}

	std::string line;
	std::vector<std::string> v;


	while (getline(file, line)) {

		std::stringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		std::string key;
		getline(line_stream, key, ' ');

		if (key == "v") {
			// X,Y,Z���W�ǂݍ���
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			temp_vertices.push_back(position);
		}

		if (key == "vn") {
			// X,Y,Z�����ǂݍ���
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			// �@���x�N�g���f�[�^�ɒǉ�
			temp_normals.emplace_back(normal);
		}

		// �擪������vt�Ȃ�e�N�X�`��
		if (key == "vt") {
			// U,V�����ǂݍ���
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			// V�������]
			//texcoord.y = 1.0f - texcoord.y;
			// �e�N�X�`�����W�f�[�^�ɒǉ�
			temp_uvs.emplace_back(texcoord);
		}

		if (key == "f") {

			// ���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			std::string index_string;
			while (getline(line_stream, index_string, ' ')) {
				// ���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				// ���_�ԍ�
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur); // �X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur); // �X���b�V�����΂�
				index_stream >> indexNormal;
				Vertex vertex{};
				vertex.pos = temp_vertices[indexPosition - 1];
				vertex.normal = temp_normals[indexNormal - 1];
				vertex.uv = temp_uvs[indexTexcoord - 1];

				obj.push_back(vertex);


			}
		}
	}
}
