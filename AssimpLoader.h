#pragma once
#include <string>
#include <vector>

#include "Vertices.h"

struct aiMesh;
struct aiMaterial;

struct Mesh {
	Vertices Vertices; // ���_�f�[�^�̔z��
	std::wstring diffuseMap; // �e�N�X�`���̃t�@�C���p�X
};

struct ImportSettings // �C���|�[�g����Ƃ��̃p�����[�^
{
public:
	const wchar_t* filename = nullptr; // �t�@�C���p�X
	std::vector<Mesh>& meshes; // �o�͐�̃��b�V���z��
	bool inverseU = false; // U���W�𔽓]�����邩
	bool inverseV = false; // V���W�𔽓]�����邩
};

class AssimpLoader
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static AssimpLoader* GetInstance();

	bool Load(ImportSettings setting); // ���f�������[�h����

private:
	void LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV);
	void LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src);
};
