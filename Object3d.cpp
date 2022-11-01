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
#include "mRootParameter.h"
#include "Object3d.h"

const std::string kBaseDirectory = "Resources/";

// ルートシグネチャ
static ComPtr<ID3D12RootSignature> rootSignature;
// パイプランステートの生成
static ComPtr<ID3D12PipelineState> pipelineState;
// グラフィックスパイプライン設定
static D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
//コマンドリストを格納する
static ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
static ComPtr<ID3D12Device> device_ = nullptr;
//頂点データ(ボックスオブジェ)
static Vertices vertices_;

Object3d::~Object3d()
{
	for (auto m : vert_) {
		delete m;
	}
	vert_.clear();
	for (auto &m : materials_) {
		delete m.second;
	}
	materials_.clear();
}

Object3d* Object3d::GetInstance()
{
	static Object3d instance;
	return &instance;
}

void Object3d::Ini(ID3D12Device* device)
{
	HRESULT result;
	device_ = device;
	ComPtr < ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr < ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr < ID3DBlob> errorBlob = nullptr; // エラーオブジェクト
#pragma region シェーダファイル読み込み
	// 頂点シェーダの読み込みとコンパイル
	ShaderCompileFromFile(L"Resources/shader/BasicVS.hlsl", "main", "vs_5_0", &vsBlob,errorBlob.Get());
	
	// ピクセルシェーダの読み込みとコンパイル
	ShaderCompileFromFile(L"Resources/shader/BasicPS.hlsl", "main", "ps_5_0", &psBlob,errorBlob.Get());
	
#pragma endregion

	//ルートのパラメータ設定
	// //ルートパラメータの設定
	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	//デスクリプタレンジの設定
	descriptorRange.NumDescriptors = 1;					//一度の描画に好かうテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;				//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートのパラメータ設定
	D3D12_ROOT_PARAMETER rootParams[3];
	//定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	////テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;					//デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
	//定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//種類
	rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し（タイリング）
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し（タイリング）
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し（タイリング）
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//全てリニア保管
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダからのみ使用可能

	// ルートシグネチャ
	// // ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	// ルートシグネチャの設定
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;	//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);			//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ルートシグネチャのシリアライズ
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	// グラフィックスパイプライン設定

	// シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // カリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	pipelineDesc.DepthStencilState.DepthEnable = true;	//深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット

	// ブレンドステート
#pragma region ブレンド設定
// ブレンドステート
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA全てのチャンネルを描画
	//共通設定（アルファ値）
	blenddesc.BlendEnable = true;					//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う
	////加算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//加算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;			//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;			//デストの値を100%使う
	////減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;			//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;			//デストの値を100%使う
	////色反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;			//使わない
	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;		//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-ソースのアルファ値
#pragma endregion

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{
	"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	D3D12_APPEND_ALIGNED_ELEMENT,
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	{
	"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	D3D12_APPEND_ALIGNED_ELEMENT,
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	{
	"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
	D3D12_APPEND_ALIGNED_ELEMENT,
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// パイプランステートの生成
	result = device_->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	vertices_.Ini(device_.Get());
}

Object3d* Object3d::CreateOBJ(const std::string& modelname)
{
	Object3d* instance = new Object3d;
	instance->ModelIni(modelname);

	return instance;
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
			material = Material::Create(device_.Get());
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
		m->CreateBuffer(device_.Get());
	}
}

void Object3d::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	commandList_ = commandList;
	// パイプラインステートとルートシグネチャの設定コマンド
	commandList_->SetPipelineState(pipelineState.Get());
	commandList_->SetGraphicsRootSignature(rootSignature.Get());

	// プリミティブ形状の設定コマンド
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
}

void Object3d::ChangeColor(float x, float y, float z, float w)
{
	vertices_.ChangeColor(x, y, z, w);
}

void Object3d::ChangeColor(XMFLOAT4 color_)
{
	vertices_.ChangeColor(color_);
}

void Object3d::DrawCube(WorldTransform* worldTransform, uint32_t descriptorSize)
{
	vertices_.Draw(vertices_.GetIndices().size(), commandList_.Get(), worldTransform, descriptorSize);
}

void Object3d::DrawOBJ(WorldTransform* worldTransform)
{
	for (auto& v : vert_) {
		v->Draw(commandList_.Get(), worldTransform, textureHandle_.at(0));
	}
}

void Object3d::DrawOBJ(WorldTransform* worldTransform, uint32_t textureHandle)
{
	for (auto& v : vert_) {
		v->Draw(commandList_.Get(), worldTransform, textureHandle);
	}
}
