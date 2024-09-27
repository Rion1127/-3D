#include "LoadManager.h"

/**
 * @file LoadManager.cpp
 * @brief リソースを読み込みをまとめている
 */

void LoadManager::LoadAllResources()
{
	LoadTexture();
	LoadModel();
	LoadSound();
}

void LoadManager::LoadModel()
{
	
	LoadAssimpModel("application/Resources/Object/moveHuman/human_run.fbx", "Human_run");
	LoadAssimpModel("application/Resources/Object/moveHuman/human_wait.fbx", "Human_wait");
	LoadAssimpModel("application/Resources/Object/moveHuman/human_jump.fbx", "Human_jump");
	LoadAssimpModel("application/Resources/Object/moveHuman/human_knockback.fbx", "Human_knockback");
	LoadAssimpModel("application/Resources/Object/moveHuman/human_wakeup.fbx", "Human_wakeup");
	LoadAssimpModel("application/Resources/Object/moveHuman/human_fever.fbx", "Human_fever");
	
	LoadAssimpModel("application/Resources/Object/moveFatman/fatman_run.fbx", "Fatman_run");
	LoadAssimpModel("application/Resources/Object/moveFatman/fatman_wait.fbx", "Fatman_wait");
	LoadAssimpModel("application/Resources/Object/moveFatman/fatman_jump.fbx", "Fatman_jump");
	LoadAssimpModel("application/Resources/Object/moveFatman/fatman_knockback.fbx", "Fatman_knockback");
	LoadAssimpModel("application/Resources/Object/moveFatman/fatman_wakeup.fbx", "Fatman_wakeup");
	LoadAssimpModel("application/Resources/Object/moveFatman/fatman_fever.fbx", "FatHuman_fever");

	LoadAssimpModel("application/Resources/Object/moveCube/man.fbx", "Man");
	LoadAssimpModel("application/Resources/Object/moveCube/man2.fbx", "Man2");

	LoadObjModel("player", true,true, "Player");
	LoadObjModel("hand", true,true, "Hand");
	LoadObjModel("field", true, false, "Field");
	LoadObjModel("sphere", true, false, "Sphere");
	LoadObjModel("cube", true, false, "Cube");
	LoadObjModel("Area", true, false, "Area");
	LoadObjModel("ring", true, false, "Ring");
	LoadObjModel("flower", true, false, "Flower");
	LoadObjModel("light", true,false, "Light");
}

void LoadManager::LoadTexture()
{
	//二つ目の引数は頭文字を大文字にするように
	LoadGraph("white64x64.png", "White64");
	LoadGraph("white1280x720.png", "White1280x720");
	LoadGraph("white1x1.png", "White");

	LoadGraph("title.png", "Title");
	LoadGraph("clear.png", "Clear");
	LoadGraph("timer_gauge.png", "TimerGauge");
	LoadGraph("fever_timer.png", "FeverGauge");

	LoadGraph("background.png", "Background");
	LoadGraph("number.png", "Number");
	LoadGraph("result.png", "Result");
	LoadGraph("score.png", "Score");
	LoadGraph("start.png", "Start");
	LoadGraph("aButton.png", "AButton");
	LoadGraph("bButton.png", "BButton");
	LoadGraph("lStick.png", "Lstick");
	LoadGraph("Menubutton.png", "Menubutton");

	LoadGraph("tutoreal_01.png", "Tutorial1");
	LoadGraph("tutoreal_02.png", "Tutorial2");
	LoadGraph("tutoreal_03.png", "Tutorial3");
	LoadGraph("tutoreal_04.png", "Tutorial4");
	//タイトルロゴ
	LoadGraph("logo_only_shadow.png", "Logo_only_shadow");

	LoadGraph("1st.png", "1st");
	LoadGraph("2nd.png", "2nd");
	LoadGraph("3rd.png", "3rd");
	LoadGraph("now_score.png", "Now_score");
	LoadGraph("rank.png", "Rank");
	LoadGraph("people_num.png", "PeopleNum");

	LoadGraph("c_rank.png", "C_Rank");
	LoadGraph("b_rank.png", "B_Rank");
	LoadGraph("a_rank.png", "A_Rank");
	LoadGraph("s_rank.png", "S_Rank");

	LoadGraph("fever.png", "Fever");
	LoadGraph("cloud_1.png", "Cloud_1");
	LoadGraph("cloud_2.png", "Cloud_2");
	LoadGraph("big_cloud.png", "Big_cloud");

	LoadGraph("to_title.png", "ToTitle");
	LoadGraph("retry.png", "Retry");
	LoadGraph("ready.png", "Ready");
	LoadGraph("start_call.png", "StartCall");
	LoadGraph("particle.png", "Particle");
	LoadGraph("light_particle.png", "Light_particle");
	LoadGraph("triangle.png", "Triangle");
	LoadGraph("swordTrail.png", "SwordTrail");
	LoadGraph("star4_particle.png", "Star4");
}

void LoadManager::LoadSound()
{
	//二つ目の引数は頭文字を大文字にするように
	//SE

	//BGM
	LoadWave("title_BGM.wav", "TitleBGM", SoundType::BGM);
	LoadWave("gameplay_BGM.wav", "GameplayBGM", SoundType::BGM);
	LoadWave("fever_BGM.wav", "FeverBGM", SoundType::BGM);
	LoadWave("result_BGM.wav", "ResultBGM", SoundType::BGM);

	LoadWave("charge_SE.wav", "ChargeSE", SoundType::SE);			//糸強く振り回してるときに重ねて鳴らす音/
	LoadWave("fat_impact_SE.wav", "FatImpactSE", SoundType::SE);	//デブインパクト/
	LoadWave("grab_SE.wav", "GrabSE", SoundType::SE);				//つかまる音/
	LoadWave("help_SE.wav", "HelpSE", SoundType::SE);				//引き上げるときの音/
	LoadWave("knockback_SE.wav", "KnockbackSE", SoundType::SE);		//ロープで吹き飛ばしたときの音/
	LoadWave("rope_break_SE.wav", "RopeBreakSE", SoundType::SE);	//糸切れる音/
	LoadWave("rope_fall_SE.wav", "RopeFallSE", SoundType::SE);		//糸垂らす音/
	LoadWave("swing_SE.wav", "SwingSE", SoundType::SE);				//糸ぶん回す音/
	LoadWave("count_SE.wav", "CountSE", SoundType::SE);
	LoadWave("select_SE.wav", "SelectSE", SoundType::SE);
	LoadWave("result_end_SE.wav", "ResultEndSE", SoundType::SE);
	LoadWave("result_SE.wav", "ResultSE", SoundType::SE);
	LoadWave("decision_SE.wav", "DecisionSE", SoundType::SE);		//決定音
}
