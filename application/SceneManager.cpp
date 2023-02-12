#include "SceneManager.h"
#include "GameScene.h"
#include "WinAPI.h"
#include "Texture.h"
#include "Easing.h"
#include "ParticleScene.h"


void SceneManager::Ini()
{
	//�r���h�J�n���̃V�[��
	Transition<ParticleScene>();
	sceneChangeSprite.Ini();
	sceneChangePos = {
		(float)-(WinAPI::GetInstance()->win_width / 2) ,
		(float)(WinAPI::GetInstance()->win_height / 2) ,
	};
	sceneChangeSprite.SetPos(sceneChangePos);

	sceneChangeGraph = TextureManager::GetInstance()->LoadGraph("white1�~1.png");
}

void SceneManager::Update()
{
	currentScene->Update();

//	//�V�[�����؂�ւ��Ƃ�
//	if (currentScene->GetIsNext() == 1) {
//#pragma region �V�[���؂�ւ�
//		float changeTime_ = 0.5f;
//		// ���݂̎���
//		int nowCount = GetNowCount();
//
//		// �o�ߎ���[s]
//		float nowTime = (float)((nowCount - changeStartCount_) / 1000.0f);
//
//		// �o�ߎ��Ԃ̊���
//		float timeRate = nowTime / changeTime_;
//
//		timeRate = Clamp(timeRate, 1.0f, 0.0f);
//
//		sceneChangePos.x =
//			Easing::Back::easeOut(
//				(float)-(WinAPI::GetInstance()->window_width / 2),
//				(float)(WinAPI::GetInstance()->window_width / 2),
//				timeRate);
//
//		sceneChangeSprite.SetPos(sceneChangePos);
//#pragma endregion
//		//�C�[�W���O���I�������V�[����؂�ւ���
//		if (timeRate >= 1.0f) {
//			isNext_ = false;
//			//�R���e�B�j���[����V�[�����^�C�g���ƃQ�[���I�[�o�[�ȊO�̂Ƃ��������
//			if (sceneNum_ != SceneNum::TITLE_ &&
//				sceneNum_ != SceneNum::GAMEOVER_) {
//				continueScene = sceneNum_;
//			}
//			Transition<GameScene>();
//		}
//	}
//	//�V�[�����ς������
//	else if (currentScene->GetIsNext() == 2) {
//#pragma region �V�[���؂�ւ�
//		float changeTime_ = 0.5f;
//		// ���݂̎���
//		int nowCount = GetNowCount();
//
//
//		if (isDifferent == true) {
//			different = nowCount - changeStartCount_;
//			isDifferent = false;
//		}
//		// �o�ߎ���[s](590�͏������Œx��鎞�ԕ������Ă���p���[�R�[�h��ŏC��)
//		float nowTime = (float)((nowCount - changeStartCount_ - different) / 1000.0f);
//
//		// �o�ߎ��Ԃ̊���
//		float timeRate = nowTime / changeTime_;
//
//		timeRate = Clamp(timeRate, 1.0f, 0.0f);
//
//		sceneChangePos.x =
//			Easing::Back::easeIn(
//				(float)(WinAPI::GetInstance()->window_width / 2),
//				(float)(WinAPI::GetInstance()->window_width * 1.5f),
//				timeRate);
//
//		sceneChangeSprite.SetPos(sceneChangePos);
//#pragma endregion
//		if (timeRate >= 1.0f) {
//			isNext_ = false;
//			currentScene->SetIsNext(0);
//		}
//	}
//	//�Q�[���I�[�o�[�̎�
//	else if (currentScene->GetIsNext() == 3) {
//		float changeTime_ = 0.5f;
//
//		// ���݂̎���
//		int nowCount = GetNowCount();
//
//		// �o�ߎ���[s]
//		float nowTime = (float)((nowCount - changeStartCount_) / 1000.0f);
//
//		// �o�ߎ��Ԃ̊���
//		float timeRate = nowTime / changeTime_;
//
//		timeRate = Clamp(timeRate, 1.0f, 0.0f);
//
//		sceneChangePos.x =
//			Easing::Back::easeOut(
//				(float)-(WinAPI::GetInstance()->window_width / 2),
//				(float)(WinAPI::GetInstance()->window_width / 2),
//				timeRate);
//
//		sceneChangeSprite.SetPos(sceneChangePos);
//		//�C�[�W���O���I�������V�[����؂�ւ���
//		if (timeRate >= 1.0f) {
//			isNext_ = false;
//
//
//			Transition<GameScene>();
//
//		}
//	}
}

void SceneManager::Draw()
{

	//�Q�[���`��
	currentScene->Draw();

	//�V�[���؂�ւ�
	sceneChangeSprite.PreDraw();
	if (currentScene->GetIsNext()) {
		sceneChangeSprite.Draw(sceneChangeGraph);
	}
}

std::unique_ptr<EmptyScene> SceneManager::currentScene = nullptr;

Sprite SceneManager::sceneChangeSprite;
Vector2  SceneManager::sceneChangePos;
uint32_t SceneManager::sceneChangeGraph;
int SceneManager::changeStartCount_;
int SceneManager::sceneNum_;
bool SceneManager::isNext_;
int SceneManager::continueScene = SceneNum::STAGE1_;
bool SceneManager::isDifferent = true;
int SceneManager::different = 0;