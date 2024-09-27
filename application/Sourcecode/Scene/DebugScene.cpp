#include "DebugScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "Camera.h"
#include "JsonLoader.h"
#include "Framework.h"
#include "Camera2d.h"
#include "ClearScene.h"
#include <filesystem>
#include "mSound.h"
#include "Framework.h"


#include "Field/FieldManager.h"

DebugScene::~DebugScene()
{
}

void DebugScene::Init()
{
	lightManager_ = std::make_shared<LightManager>();
	Model::SetLight(lightManager_->GetLightGroup());
	AssimpModel::SetLight(lightManager_->GetLightGroup());
	Camera2d::GetInstance()->Init();

	assimpObjects_.Init();
	assimpObjects2_.Init();
	assimpObjects2_.SetAnimationTimer(20);
	timer_.SetLimitTime(60);

	FieldManager::GetInstance()->Initialize();

	//モデル読み込み
	playerObj_ = std::make_unique<Object3d>();
	playerObj_->SetModel(ModelManager::GetInstance()->GetModel("Field"));

	colliderObj_ = std::make_unique<Object3d>();
	colliderObj_->SetModel(ModelManager::GetInstance()->GetModel("Light"));
	auto callback = std::bind(&DebugScene::CallBack, this);
	// コライダー|初期化関数実行
	sphereCol_.Initialize("swing_rope", callback, &colliderManager_);

	//feverLight_.Init(lightManager_->GetLightGroup());
	//feverLight_.SetIsFever(true);

	knockBack_.Init();
	knockTimer_.SetLimitTime(120);
	knockVec_ = Vector3(0.8f, 1.f, 0);
	knockPower_ = 1.f;

	HumanManager::GetInstance()->Initialize(&colliderManager_,nullptr,nullptr);

	fireEmitter_ = std::make_shared<ContinuousEmitter>();
	TimerFloat timer;
	timer.SetLimitTime(1);
	timer.SetTime(timer.GetLimitTimer());
	fireEmitter_->popCoolTime_ = timer;
	fireEmitter_->pos = { 0,10,0 };
	ParticleManager::GetInstance()->AddParticle("fireBall", fireEmitter_);
}

void DebugScene::Update()
{
#ifdef _DEBUG
	//カメラ更新
	debugCamera_.Update();
	Camera::scurrent_ = debugCamera_.GetCamera();
	Camera::scurrent_->Update(CameraMode::LookAT);
#endif // _DEBUG
	


	colliderManager_.Update();
	
	assimpObjects_.SetPos(Vector3(-5, 0, 0));
	assimpObjects2_.SetPos(Vector3(5, 0, 0));

	assimpObjects_.Update();
	timer_.AddTime();
	if (timer_.GetIsEnd()) {
		assimpObjects2_.Update();
	}
	//feverLight_.Update();

	lightManager_->Update();
	
	knockTimer_.AddTime();
	if (knockTimer_.GetIsEnd()) {
		playerObj_->SetPos(Vector3(0, 0, 0));
		Vector3 knockVec = knockVec_.normalize() * knockPower_;
		knockBack_.SetKnockVeclocity(knockVec);
		knockTimer_.Reset();
	}
	if (Key::TriggerKey(DIK_K))knockBack_.SetKnockVeclocity(Vector3(1,1.5f,0));
	//床に触れていたらグラビティをリセットする
	auto pos = playerObj_->GetPos();
	
	//座標移動
	pos += knockBack_.GetKnockVelocity();
	knockBack_.Update();
	if (pos.y <= 2) {
		if (knockBack_.GetGravity() < 0) {
			knockBack_.GravityReset();
		}
	}
	//ロープの先端の当たり判定移動処理
	if (Key::PushKey(DIK_D)) { sphereCol_.sphere_.center.x += 0.3f; }
	if (Key::PushKey(DIK_A)) { sphereCol_.sphere_.center.x -= 0.3f; }
	if (Key::PushKey(DIK_W)) { sphereCol_.sphere_.center.z += 0.3f; }
	if (Key::PushKey(DIK_S)) { sphereCol_.sphere_.center.z -= 0.3f; }
	if (Key::PushKey(DIK_I)) { sphereCol_.sphere_.center.y += 0.3f; }
	if (Key::PushKey(DIK_K)) { sphereCol_.sphere_.center.y -= 0.3f; }
	colliderObj_->SetPos(Vector3(0, 10, 0));
	colliderObj_->SetRot(Vector3(Radian(45),Radian(-90), 0));
	//float radius = sphereCol_.sphere_.radius;
	//Vector3 scale = { radius ,radius ,radius };
	//colliderObj_->SetScale(scale);
	colliderObj_->SetScale(Vector3(2, 5, 2));
	playerObj_->SetColor(Color(255, 255, 255, 255));
	playerObj_->SetPos({0,0,0});
	playerObj_->SetShadowAtten(Vector3(0, 0, 0));
	playerObj_->Update();
	colliderObj_->Update();
	FieldManager::GetInstance()->Update();
	//HumanManager::GetInstance()->Update();

	

	

	ParticleManager::GetInstance()->Update();
}

void DebugScene::Draw()
{
	/////////////////
	//3Dオブジェクト//
	/////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	//feverLight_.Draw();
	//playerObj_->Draw();
	
	FieldManager::GetInstance()->Draw();
	//HumanManager::GetInstance()->Draw();
	colliderObj_->Draw();
	PipelineManager::PreDraw("Object3D_NotCull", TRIANGLELIST);
	/////////////////
	//assimpオブジェクト//
	/////////////////
	PipelineManager::PreDraw("assimp", TRIANGLELIST);
	/*assimpObjects_.Draw();
	if (timer_.GetIsEnd()) {
		assimpObjects2_.Draw();
	}*/
	
	ParticleManager::GetInstance()->Draw();

	knockBack_.DrawImGui();
	ImGui::Begin("knockVec");

	float* knockVec[3] = { &knockVec_.x,&knockVec_.y,&knockVec_.z };
	ImGui::DragFloat3("knockVec", *knockVec,0.1f);

	float* knockpower = &knockPower_;
	ImGui::DragFloat("knockpower", knockpower, 0.1f);

	ImGui::End();
}

void DebugScene::DrawRenderTexture()
{
}

void DebugScene::DrawPostEffect()
{
}

void DebugScene::CallBack()
{
	
}
