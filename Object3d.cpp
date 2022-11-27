#include <Windows.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <cassert>
#include <string>
#include "WorldTransform.h"
#include "Material.h"
#include "Texture.h"
#include <fstream>
#include <sstream>
#include "Util.h"
#include "Object3d.h"

const std::string kBaseDirectory = "Resources/";

//コマンドリストを格納する
static DirectXCommon* directX_ = nullptr;

Pipeline Object3d::pipeline_;

Object3d::~Object3d()
{
	for (auto m : vert_) {
		delete m;
	}
	vert_.clear();
	for (auto m : materials_) {
		delete m.second;
	}
	materials_.clear();
}

Object3d* Object3d::GetInstance()
{
	static Object3d instance;
	return &instance;
}

void Object3d::Ini()
{
	directX_ = DirectXCommon::GetInstance();

	ComPtr < ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr < ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr < ID3DBlob> errorBlob = nullptr; // エラーオブジェクト
#pragma region シェーダファイル読み込み
	// 頂点シェーダの読み込みとコンパイル
	ShaderCompileFromFile(L"Resources/shader/BasicVS.hlsl", "main", "vs_5_0", &vsBlob,errorBlob.Get());
	
	// ピクセルシェーダの読み込みとコンパイル
	ShaderCompileFromFile(L"Resources/shader/BasicPS.hlsl", "main", "ps_5_0", &psBlob,errorBlob.Get());
	
#pragma endregion

	pipeline_.Ini();
}

Object3d* Object3d::CreateOBJ(const std::string& modelname)
{
	Object3d* instance = new Object3d;
	instance->ModelIni(modelname);

	return instance;
}

std::unique_ptr<Object3d> Object3d::CreateOBJ_uniptr(const std::string& modelname)
{
	std::unique_ptr<Object3d> instance = std::make_unique<Object3d>();
	instance->ModelIni(modelname);

	return /*std::move(*/instance/*)*/;
}

void Object3d::SetBlend(int blend)
{
	pipeline_.SetBlend(blend);
}

void Object3d::SetNormal()
{
	pipeline_.SetNormalBlend();
}

void Object3d::SetModel(const Object3d* model)
{
	
	vert_.emplace_back(new Vertices);	//空の頂点データを入れる
	Vertices* vert = vert_.back();		//空のvert_のアドレスをvertに入れる

	for (size_t i = 0; i < model->vert_[0]->vertices.size(); i++) {
		vert->AddVertices(model->vert_[0]->vertices[i]);
	}

	for (size_t i = 0; i < model->vert_[0]->indices.size(); i++) {
		vert->AddIndex(model->vert_[0]->indices[i]);
	}


	//vert_.resize(model->vert_.size());
	//std::copy(model->vert_.begin(), model->vert_.end(), vert_.begin());

	//textureHandle_.resize(model->textureHandle_.size());
	//std::copy(model->textureHandle_.begin(), model->textureHandle_.end(), textureHandle_.begin());

	//for (size_t i = 0; i < model->materials_.size(); i++) {
	//	materials_.insert(model->materials_.at());
	//}

	//materials_.insert(model->materials_.begin(), model->materials_.end());

	for (size_t i = 0; i < model->textureHandle_.size();i++) {
		textureHandle_.push_back(model->textureHandle_[i]);
	}

	// メッシュのバッファ生成
	for (auto& m : vert_) {
		m->CreateBuffer(directX_->GetDevice());
	}
}

void Object3d::LoadOBJ(const std::string& modelname)
{
	const std::string filename = modelname + ".obj";
	const std::string directoryPath = kBaseDirectory + modelname + "/";
	// ファイルストリーム
	std::ifstream file;
	// .objファイルを開く
	file.open(directoryPath + filename);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	name_ = modelname;

	std::string line;	//ファイルの1行を入れる変数

	std::vector<XMFLOAT3> positions; //頂点座標
	std::vector<XMFLOAT3> normals;   // 法線ベクトル
	std::vector<XMFLOAT2> texcoords; // テクスチャUV

	vert_.emplace_back(new Vertices);	//空の頂点データを入れる
	Vertices* vert = vert_.back();		//空のvert_のアドレスをvertに入れる

	int indexCountTex = 0;

	while (getline(file, line)) {

		std::stringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		//マテリアル
		if (key == "mtllib") {
			// マテリアルのファイル名読み込み
			std::string filename;
			line_stream >> filename;
			// マテリアル読み込み
			LoadMaterial(directoryPath, filename);
		}

		if (key == "v") {
			// X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			positions.emplace_back(position);
		}

		if (key == "vn") {
			// X,Y,Z成分読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			// 法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		// 先頭文字列がvtならテクスチャ
		if (key == "vt") {
			// U,V成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			// V方向反転
			texcoord.y = 1.0f - texcoord.y;
			// テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}

		if (key == "f") {
			int faceIndexCount = 0;
			// 半角スペース区切りで行の続きを読み込む
			std::string index_string;
			while (getline(line_stream, index_string, ' ')) {
				// 頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				// 頂点番号
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur); // スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur); // スラッシュを飛ばす
				index_stream >> indexNormal;
				Vertices::VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];

				vert->AddVertices(vertex);

				// インデックスデータの追加
				if (faceIndexCount >= 3) {
					// 四角形ポリゴンの4点目なので、
					// 四角形の0,1,2,3の内 2,3,0で三角形を構築する
					vert->AddIndex(indexCountTex - 1);
					vert->AddIndex(indexCountTex);
					vert->AddIndex(indexCountTex - 3);
				}
				else {
					vert->AddIndex(indexCountTex);
				}
				faceIndexCount++;
				indexCountTex++;
			}
		}
	}

	LoadTexture();
}

void Object3d::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	// ファイルストリーム
	std::ifstream file;
	// マテリアルファイルを開く
	file.open(directoryPath + filename);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	Material* material = nullptr;

	// 1行ずつ読み込む
	std::string line;
	while (getline(file, line)) {
		// 先頭のタブ文字は無視する
		if (line[0] == '\t') {
			line.erase(line.begin()); // 先頭の文字を削除
		}
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		// 先頭のタブ文字は無視する
		if (key[0] == '\t') {
			key.erase(key.begin()); // 先頭の文字を削除
		}

		// 先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl") {

			// 既にマテリアルがあれば
			if (material) {
				// マテリアルをコンテナに登録
				AddMaterial(material);
			}

			// 新しいマテリアルを生成
			material = Material::Create(directX_->GetDevice());
			// マテリアル名読み込み
			line_stream >> material->name_;
			
		}

		// 先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd") {
			// テクスチャのファイル名読み込み
			line_stream >> material->textureFilename_;

			// フルパスからファイル名を取り出す
			size_t pos1;
			pos1 = material->textureFilename_.rfind('\\');
			if (pos1 != std::string::npos) {
				material->textureFilename_ = material->textureFilename_.substr(
					pos1 + 1, material->textureFilename_.size() - pos1 - 1);
			}

			pos1 = material->textureFilename_.rfind('/');
			if (pos1 != std::string::npos) {
				material->textureFilename_ = material->textureFilename_.substr(
					pos1 + 1, material->textureFilename_.size() - pos1 - 1);
			}
		}
	}
	// ファイルを閉じる
	file.close();

	
	if (material) {
		// マテリアルを登録
		AddMaterial(material);
	}
}

void Object3d::LoadTexture()
{
	std::string directoryPath = name_ + "/";

	for (auto& m : materials_) {
		Material* material = m.second;

		// テクスチャあり
		if (material->textureFilename_.size() > 0) {
			// マテリアルにテクスチャ読み込み
			material->LoadTexture(directoryPath);
		}
		// テクスチャなし
		else {
			// マテリアルにテクスチャ読み込み
			material->LoadTexture("white1x1.png");
		}
		textureHandle_.push_back(material->textureHandle_);
	}
}

void Object3d::AddMaterial(Material* material)
{
	// コンテナに登録
	materials_.emplace(material->name_, material);
}

void Object3d::ModelIni(const std::string& modelname)
{
	LoadOBJ(modelname);
	// メッシュのバッファ生成
	for (auto& m : vert_) {
		m->CreateBuffer(directX_->GetDevice());
	}
}

void Object3d::PreDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	directX_->GetCommandList()->SetPipelineState(pipeline_.gerPipelineState());
	directX_->GetCommandList()->SetGraphicsRootSignature(pipeline_.GetRootSignature());

	// プリミティブ形状の設定コマンド
	directX_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
}

void Object3d::ObjChangeColor(float x, float y, float z, float w)
{
	for (auto& v : vert_) {
		v->ChangeColor(x, y, z, w);
	}
}

void Object3d::ObjChangeColor(XMFLOAT4 color_)
{
	for (auto& v : vert_) {
		v->ChangeColor(color_);
	}
}

void Object3d::DrawOBJ(WorldTransform* worldTransform)
{
	for (auto& v : vert_) {
		v->Draw(directX_->GetCommandList(), worldTransform, textureHandle_.at(0));
	}
}

void Object3d::DrawOBJ(WorldTransform* worldTransform, uint32_t textureHandle)
{
	for (auto& v : vert_) {
		v->Draw(directX_->GetCommandList(), worldTransform, textureHandle);
	}
}

