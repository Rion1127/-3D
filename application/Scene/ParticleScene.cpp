#include "ParticleScene.h"


ParticleScene::~ParticleScene()
{
}

void ParticleScene::Ini()
{
	directX = DirectXCommon::GetInstance();
	winApi_ = WinAPI::GetInstance();
	input_ = DirectXInput::GetInstance();
	controller_ = Controller::GetInstance();
	textureM = TextureManager::GetInstance();
	sound_ = SoundManager::GetInstance();

	//デバッグカメラ初期化
	debugCamera.DebugCameraIni();
	debugCamera.Update();

	gameCamera.SetEyePos(Vector3(0, 8, -20));
	gameCamera.SetTarget(Vector3(0, 0, 0));
	gameCamera.Update();
	//useVP = &gameCamera;
	useVP = debugCamera.GetViewProjection();
	useVP->SetOriginalPos();
	

	
	WT_.Ini();

	texture_ = textureM->LoadGraph("keyBlade2.png");
	whiteTexture_ = textureM->LoadGraph("uv.png");

	const wchar_t* modelFile = L"Resources/FBX/Alica/Alicia_solid_Unity.FBX";

	importSetting_.filename = modelFile;
	importSetting_.meshes = meshes;
	importSetting_.inverseU = false;
	importSetting_.inverseV = true;

	AssimpLoader loader;
	loader.Load(importSetting_);
	
	for (int i = 0; i < importSetting_.meshes.size(); i++)
	{
		importSetting_.meshes[i].Vertices.CreateBuffer(directX->GetDevice());
	}

	int a = 0;
}

void ParticleScene::Update()
{
	//カメラ更新
	if (input_->PushKey(DIK_LCONTROL)) {
		debugCamera.Update();
	}
	gameCamera.Update();


	WT_.Update();

	object_.Update(*useVP);

}

void ParticleScene::Draw()
{

	///////////////////
	/////パーティクル////
	///////////////////
	ParticleManager::GetInstance()->PreDraw();

	//object_.Draw(whiteTexture_);
}
