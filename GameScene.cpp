#include "Texture.h"
#include <random>
#include "GameScene.h"

GameScene::~GameScene()
{
	delete skyDome;
	delete gumiShipObj;
}

void GameScene::Ini()
{
	directX = DirectXCommon::GetInstance();
	winApi_ = WinAPI::GetInstance();
	input_ = DirectXInput::GetInstance();
	textureM = TextureManager::GetInstance();
	sound_ = SoundManager::GetInstance();

	Object3d::Ini(directX->GetDevice());
	BoardObject::Ini(directX->GetDevice());

	debugCamera.DebugCameraIni(&winApi_->hwnd);

	marioGraph = textureM->LoadGraph("Resources/mario.jpg");
	khGraph = textureM->LoadGraph("Resources/KH.jpg");
	enemyGraph = textureM->LoadGraph("Resources/enemy.jpg");
	keyBladeGraph = textureM->LoadGraph("Resources/keyBlade2.png");
	gumishipGraph = textureM->LoadGraph("Resources/gumiship.png");
	graph = marioGraph;

	skyDome = Object3d::CreateOBJ("skydome");
	skyDomePos.Initialize();
	skyDomePos.SetPosition(0, 0, 0);
	skyDomePos.SetRotation(0, 0, 0);
	skyDomePos.SetScale(5, 5, 5);

	gumiShipObj = Object3d::CreateOBJ("sphere");
	gumiShipPos.Initialize();
	gumiShipPos.SetPosition(0, 0, 0);
	gumiShipPos.SetRotation(0, 0, 0);
	gumiShipPos.SetScale(1, 1, 1);


	sound_->LoadWave("result.wav", "ResultBGM");

	boardPos.Initialize();
	boardPos.SetPosition(0, 0, 0);
	boardPos.SetRotation(ConvertAngleToRadian(90),0,0);
	
	viewProjection.SetEyePos(0, 0, -200);
	viewProjection.SetTarget(0, 0, 0);
	viewProjection.SetUpVec(0, 1, 0);
	viewProjection.Ini();

	sphere_.center = XMVectorSet(0, 0, 0, 1);
	sphere_.radius = 0.8f;

	plane_.normal = XMVectorSet(0, 1, 0, 0);
	plane_.distance = 0.0f;

	triangle.p0 = XMVectorSet(-1.0f, 0, -1.0f, 1);	//左手前
	triangle.p1 = XMVectorSet(-1.0f, 0, +1.0f, 1);	//左奥
	triangle.p2 = XMVectorSet(+1.0f, 0, -1.0f, 1);	//右手前
	triangle.normal = XMVectorSet(0.0f,1.0f,0.0f,0);	//上向き

	trianglePos[0].position.x = triangle.p0.m128_f32[0];
	trianglePos[0].position.y = triangle.p0.m128_f32[1];
	trianglePos[0].position.z = triangle.p0.m128_f32[2]; 

	trianglePos[1].position.x = triangle.p1.m128_f32[0];
	trianglePos[1].position.y = triangle.p1.m128_f32[1];
	trianglePos[1].position.z = triangle.p1.m128_f32[2];

	trianglePos[2].position.x = triangle.p2.m128_f32[0];
	trianglePos[2].position.y = triangle.p2.m128_f32[1];
	trianglePos[2].position.z = triangle.p2.m128_f32[2];

	for (int i = 0; i < 3; i++) {
		trianglePos[i].Initialize();
		trianglePos[i].scale = { 0.1f,0.1,0.1f };
		trianglePos[i].UpdateObject3d(debugCamera.GetViewProjection());
	}
}

void GameScene::Updata()
{
#ifdef _DEBUG


#endif // _DEBUG
	/*if (input_->TriggerKey(DIK_1)) {
		sound_->Play("ResultBGM",0.05f);
	}
	if (input_->TriggerKey(DIK_2)) {
		sound_->Stop("ResultBGM");
	}*/

	debugCamera.Update(winApi_->hwnd);

	boardPos.UpdateObject3d(/*viewProjection*/debugCamera.GetViewProjection()/*railCamera.viewProjection*/);


	float speed = 0.1f;
	if (input_->PushKey(DIK_LSHIFT)) {
		speed = 0.01f;
	}

	if (input_->PushKey(DIK_W)) {
		gumiShipPos.position += { 0,0, speed };
	}
	if (input_->PushKey(DIK_S)) {
		gumiShipPos.position += { 0,0,-speed };
	}
	if (input_->PushKey(DIK_A)) {
		gumiShipPos.position += { -speed,0,0 };
	}
	if (input_->PushKey(DIK_D)) {
		gumiShipPos.position += { speed,0,0 };
	}

	if (input_->PushKey(DIK_UP)) {
		gumiShipPos.position += { 0, speed, 0 };
	}
	if (input_->PushKey(DIK_DOWN)) {
		gumiShipPos.position += { 0, -speed, 0 };
	}

	sphere_.center = {
			gumiShipPos.position.x,
			gumiShipPos.position.y,
			gumiShipPos.position.z
	};

	XMVECTOR inter;
	bool hit = /*Sphere2PlaneCol(sphere_, plane_)*/
		Sphere2TriangleCol(sphere_,triangle,&inter);
	if (hit) {
		gumiShipObj->ChangeColorObj(DirectX::XMFLOAT4(1.0f, 0, 0, 1));
	}
	else {
		gumiShipObj->ChangeColorObj(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1));
	}

	gumiShipPos.UpdateObject3d(/*viewProjection*/debugCamera.GetViewProjection());
	skyDomePos.UpdateObject3d(debugCamera.GetViewProjection());

	for (int i = 0; i < 3; i++) {
		trianglePos[i].UpdateObject3d(debugCamera.GetViewProjection());
	}
}

void GameScene::Draw()
{
	////////////////
	//3Dオブジェクト//
	////////////////

	Object3d::PreDraw(directX->GetCommandList());



	//player_.Draw(gumishipGraph);
	//enemy_.Draw();

	skyDome->DrawOBJ(&skyDomePos);
	gumiShipObj->DrawOBJ(&gumiShipPos);
	for (int i = 0; i < 3; i++) {
		gumiShipObj->DrawOBJ(&trianglePos[i]);
	}
	///////////////////
	//板状３Dオブジェクト//
	///////////////////
	BoardObject::PreDraw(directX->GetCommandList());

	//boardObject.Draw(&boardPos, khGraph);
	////////////////
	//2Dオブジェクト//
	////////////////

}
