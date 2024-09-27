#include "Rope.h"
#include "mInput.h"
#include "Human/HumanManager.h"
#include "imgui.h"
#include "ModelManager.h"
#include <fstream>
#include <iostream>
#include "mSound.h"
#include "Tutorial.h"
#include "Easing.h"
#include "PlayerManager.h"

void Rope::Initialize(Vector3 pos, ColliderManager* colManagerPtr, bool autoMode, int32_t serial)
{
	for (int i = 0; i < ROPE_LENGTH; i++) {
		//3Dオブジェクト
		ropeObj_[i] = std::move(std::make_unique<Object3d>());
		//モデル読み込み
		ropeObj_[i]->SetModel(ModelManager::GetInstance()->GetModel("Sphere"));
		ropeObj_[i]->SetScale({ 0.5f,0.5f,0.5f });
		//糸
		pos_[i] = { pos.x,pos.y - distance,pos.z };
		vec_[i] = { 0,0,0 };
	}
	//3Dオブジェクト
	catchObj_ = std::move(std::make_unique<Object3d>());
	reliefLight_ = std::move(std::make_unique<Object3d>());
	//モデル読み込み
	catchObj_->SetModel(ModelManager::GetInstance()->GetModel("Area"));
	reliefLight_->SetModel(ModelManager::GetInstance()->GetModel("Light"));

	isUse_ = false;
	switchingTimer_ = 0;
	ropeY_ = -switchingTime_ * switchingSpeed_;
	weight_ = 0;
	isCut_ = false;
	cutTimer = 0;
	catchRadius_ = 4.f;
	catchAngle_ = 0;
	oldPos_ = pos;
	swingPower_ = SwingPower::POWER_ZERO;

	// コライダー|初期化関数実行
	auto callback = std::bind(&Rope::CallBack, this);
	swingCollider_.Initialize("swing_rope", callback, colManagerPtr);
	swingCollider_.sphere_.radius = 10;

	output_.SetfileDir("application/Resources/Config/");
	isAuto_ = autoMode;
	prevIsSwing_ = isSwing_;
	prevSwingPower_ = swingPower_;
	swingCollider_.Data_Add("isSwing", isSwing_);

	serial_ = serial;
	swingCollider_.Data_Add("swingPower", swingPower_);
	lightTimer_.SetLimitTime(switchingTime_ + 60);
	lightTimer_.SetTime(lightTimer_.GetLimitTimer());
	firstLightTimer_.SetLimitTime(30);
	firstLightTimer_.SetTime(firstLightTimer_.GetLimitTimer());
	firstLightTimer_.SetIsEnd(true);
	Color color = Color(0, 0, 0, -1.f);
	reliefLight_->SetColor(color);

	TimerFloat timer;
	timer.SetLimitTime(2);
	timer.SetTime(timer.GetLimitTimer());
	fireEmitter_ = std::make_shared<ContinuousEmitter>();
	fireEmitter_->popCoolTime_ = timer;

	trail_ = std::make_unique<ParticleTrail>(20);
	trail_->SetTexture(TextureManager::GetInstance()->GetTexture("SwordTrail"));
}

void Rope::Update(Vector3 pos)
{
	Vector3 usePos = pos;
	if (usePos.x == -999.f) {
		usePos = oldPos_;
	}
#ifdef DEBUG
	//imgui
	ImGui::Begin("Rope");

	ImGui::SliderFloat("糸に捕まれる半径", &catchRadius_, 0.f, 20.f);
	ImGui::SliderFloat("糸の球の間隔", &distance, 0.f, 10.f);
	ImGui::Text("糸の出し入れ残り時間 = %d", switchingTimer_);
	ImGui::SliderInt("糸の出し入れ時間", &switchingTime_, 0, 200);
	ImGui::SliderFloat("糸の出し入れスピード", &switchingSpeed_, 0.f, 10.f);
	ImGui::SliderFloat("非キャッチ時、糸の加速基本倍率", &nonCatchNormal_, 0.f, 0.5f);
	ImGui::SliderFloat("非キャッチ時、糸の加速追加倍率", &nonCatchAdd_, -0.005f, 0.005f);
	ImGui::SliderFloat("非キャッチ時、糸の減速基本倍率", &nonCatchSubNormal_, 0.8f, 1.f);
	ImGui::SliderFloat("非キャッチ時、糸の減速追加倍率", &nonCatchSubAdd_, -0.1f, 0.1f);
	ImGui::SliderFloat("キャッチ時、糸の加速倍率", &catchNormal_, 0.f, 0.3f);
	ImGui::SliderFloat("キャッチ時、糸の減速倍率", &catchSub_, 0.f, 1.f);
	ImGui::Text("糸の重さ = %d", weight_);
	ImGui::Text("糸の人数 = %d", catchCount_);
	if (isSwing_) {
		ImGui::Text("振り回し判定 = true");
	}
	else {
		ImGui::Text("振り回し判定 = false");
	}
	std::string swingpower = "振り回し強さ判定 = ";
	if (swingPower_ == SwingPower::POWER_ZERO)swingpower += "NONE";
	else if (swingPower_ == SwingPower::WEAK)swingpower += "WEAK";
	else if (swingPower_ == SwingPower::STRONG)swingpower += "STRONG";
	ImGui::Text(swingpower.c_str());
	ImGui::SliderFloat("振り回し判定のしきい値", &swingWeakRange_, 0.f, 100.f);
	ImGui::SliderFloat("デブインパクトを起こすしきい値", &swingStrongRange_, 0.f, 100.f);
	ImGui::SliderInt("振り回し判定の持続時間", &swingTime_, 0, 1000);

	if (ImGui::Button("Save", ImVec2(50, 50))) {
		Save();
	}

	if (ImGui::Button("Load", ImVec2(50, 50))) {
		Load();
	}

	ImGui::Text("seTime : %d", swingSETimer_.GetLimitTimer());

	ImGui::End();

	ImGui::Begin("RopeList");
	ImGui::Text("serial = %d", serial_);
	ImGui::Text("Rope = %f,%f,%f", pos_[ROPE_LENGTH - 1].x, pos_[ROPE_LENGTH - 1].y, pos_[ROPE_LENGTH - 1].z);
	ImGui::End();
#endif // DEBUG
	//重さ検査
	if (weight_ >= MAX_WEIGHT && !isAuto_) {
		isCut_ = true;
		cutTimer = CUT_TIME;
		SoundManager::Play("RopeBreakSE");
	}
	//リセット
	weight_ = 0;
	if (--swingTimer_ < 0) {
		isSwing_ = false;
		swingPower_ = SwingPower::POWER_ZERO;
	}

	if (isCut_) {
		for (int i = 0; i < ROPE_LENGTH; i++) {
			pos_[i].y -= 0.5f;
		}
		HumanManager::GetInstance()->AllDisengage();
		isUse_ = false;
		isCatch_ = false;
		catchCount_ = 0;

		if (--cutTimer < 0) {
			isCut_ = false;
			ropeY_ = -switchingTime_ * switchingSpeed_;
		}
	}
	else {
		if (switchingTimer_ > 0) {
			if (--switchingTimer_ <= 0) {
				isUse_ = (isUse_ + 1) % 2;
				isCatch_ = false;
				catchCount_ = 0;

				if (isAuto_ && !isUse_) {
					isDead_ = true;
				}

				if (!isUse_) {
					ropeY_ = -switchingTime_ * switchingSpeed_;
				}
			}
			else {
				if (isUse_) {
					ropeY_ -= switchingSpeed_;
					isCatch_ = false;
					catchCount_ = 0;
				}
				else {
					ropeY_ += switchingSpeed_;
				}
			}
		}
		else {
			if (!(Tutorial::GetInstance()->GetTutorialNum() > 3 && Tutorial::GetInstance()->GetTutorialNum() < 6) && (!isAuto_ && Controller::GetTriggerButtons(PAD::INPUT_A)) || (!isAuto_ && Key::TriggerKey(DIK_SPACE)) || (isAuto_ && !isUse_) || (isAuto_ && isCatch_) || (PlayerManager::GetInstance()->GetIsFever() && isUse_ && serial_ == 0) || (!PlayerManager::GetInstance()->GetIsFever() && serial_ != 0)) {
				if (!(serial_ == 0 && PlayerManager::GetInstance()->GetIsFever())) {
					switchingTimer_ = switchingTime_;
					//ロープをたらすor引き上げる音
					if (isUse_) {
						lightTimer_.Reset();
						firstLightTimer_.Reset();
						SoundManager::Play("HelpSE", false, 1.5f);
						ParticleManager::GetInstance()->AddParticle("light", fireEmitter_);
						trail_->SetIsVisible(false);
					}
					else {
						SoundManager::Play("RopeFallSE", false, 0.7f);
						trail_->SetIsVisible(true);
					}
				}
			}
		}
		//糸のはじめ
		pos_[0] = { usePos.x,usePos.y - distance - ropeY_,usePos.z };

		//ロープが動いているかどうか
		bool isRopeMove = false;
		//糸の下の物理挙動(誰かが掴んでいるかで挙動が変わる)
		if (!isCatch_) {
			for (int i = 1; i < ROPE_LENGTH; i++) {
				if (PlayerManager::GetInstance()->GetPos().y < pos_[i].y - 4.f || isAuto_) {
					pos_[i] = usePos;
					vec_[i] = { 0,0,0 };
				}
				else if (PlayerManager::GetInstance()->GetPos().y < pos_[i].y + 4.f) {
					vec_[i] = PlayerManager::GetInstance()->GetPos() - pos_[i];
					vec_[i] = vec_[i].normalize() * 0.5f;
				}
				else {
					vec_[i] += (pos_[i - 1] - pos_[i]) * (nonCatchNormal_ + nonCatchAdd_ * i);
					vec_[i].y = 0;

					if (vec_[i].x > 0.5f) {
						vec_[i].x = 0.5f;
						isRopeMove = true;
					}
					if (vec_[i].x < -0.5f) {
						vec_[i].x = -0.5f;
						isRopeMove = true;
					}
					if (vec_[i].z > 0.5f) {
						vec_[i].z = 0.5f;
						isRopeMove = true;
					}
					if (vec_[i].z < -0.5f) {
						vec_[i].z = -0.5f;
						isRopeMove = true;
					}

					if (vec_[i].x < 0.01f && vec_[i].x > -0.01f) {
						vec_[i].x = 0;
					}
					if (vec_[i].z < 0.01f && vec_[i].z > -0.01f) {
						vec_[i].z = 0;
					}

					vec_[i] *= nonCatchSubNormal_ - nonCatchSubAdd_ * i;
				}
				pos_[i] += vec_[i];
				//離れすぎ調整
				{
					Vector3 temp = pos_[i] - pos_[i - 1];
					if (temp.length() > distance) {
						pos_[i] = pos_[i - 1] + temp.normalize() * distance;
					}
				}
				//y調整
				{
					Vector3 temp = pos_[i - 1] - pos_[i];
					temp.y = 0;
					float length = temp.length();
					pos_[i].y = pos_[i - 1].y - (distance - length);
				}
			}
		}
		else {
			//遠心力！
			vec_[ROPE_LENGTH - 1] += (pos_[0] - pos_[ROPE_LENGTH - 1]) * catchNormal_;
			vec_[ROPE_LENGTH - 1].y = 0;

			if (isAuto_) {
				vec_[ROPE_LENGTH - 1] = { 0,0,0 };
			}

			if (vec_[ROPE_LENGTH - 1].x < 0.01f && vec_[ROPE_LENGTH - 1].x > -0.01f) {
				vec_[ROPE_LENGTH - 1].x = 0;
			}
			if (vec_[ROPE_LENGTH - 1].z < 0.01f && vec_[ROPE_LENGTH - 1].z > -0.01f) {
				vec_[ROPE_LENGTH - 1].z = 0;
			}

			vec_[ROPE_LENGTH - 1] *= catchSub_;

			pos_[ROPE_LENGTH - 1] += vec_[ROPE_LENGTH - 1];

			int32_t ropeCount = 0;
			for (int i = 1; i < ROPE_LENGTH - 1; i++) {
				if (PlayerManager::GetInstance()->GetPos().y < pos_[i].y - 3.f) {
					ropeCount++;
				}
			}
			//補間
			Vector3 tempVec = pos_[ROPE_LENGTH - 1] - pos_[0];
			tempVec /= ROPE_LENGTH;

			for (int i = 0; i < ropeCount; i++) {
				pos_[i].x = pos.x;
				pos_[i].z = pos.z;
				isRopeMove = true;
			}
			for (int i = 1 + ropeCount; i < ROPE_LENGTH - 1; i++) {
				pos_[i] = pos_[0] + tempVec * (float)(i - ropeCount);
				isRopeMove = true;
			}

			for (int i = 1; i < ROPE_LENGTH; i++) {
				//y調整
				{
					Vector3 temp = pos_[i - 1] - pos_[i];
					temp.y = 0;
					float length = temp.length();
					pos_[i].y = pos_[i - 1].y - (distance - length);
					isRopeMove = true;
				}
			}

			Vector3 Ppos = PlayerManager::GetInstance()->GetPos();
			Vector3 Rpos = GetEndPos(ROPE_LENGTH - 1);
			Ppos.y = 0;
			Rpos.y = 0;

			if ((Ppos - Rpos).length() > swingWeakRange_) {
				isSwing_ = true;
				swingPower_ = SwingPower::WEAK;
				swingTimer_ = swingTime_;
			}
			if ((Ppos - Rpos).length() > swingStrongRange_) {
				isSwing_ = true;
				swingPower_ = SwingPower::STRONG;
				swingTimer_ = swingTime_;
				if (Tutorial::GetInstance()->GetTutorialNum() < 5) {
					swingPower_ = SwingPower::WEAK;
				}
			}

			if (ropeCatchHumanList_.size() == 0) {
				isCatch_ = false;
				catchCount_ = 0;
			}

		}
		if (prevIsSwing_ != isSwing_) {
			swingCollider_.Data_Remove("isSwing");
			swingCollider_.Data_Add("isSwing", isSwing_);
		}
		if (prevSwingPower_ != swingPower_) {
			swingCollider_.Data_Remove("swingPower");
			swingCollider_.Data_Add("swingPower", swingPower_);

			//SE
			if (swingPower_ == SwingPower::WEAK) {
				SoundManager::Stop("ChargeSE");
			}
			else if (swingPower_ == SwingPower::STRONG) {
				SoundManager::Play("ChargeSE", true, 0.3f);
			}
			else {
				SoundManager::Stop("SwingSE");
				SoundManager::Stop("ChargeSE");
			}
		}
		prevIsSwing_ = isSwing_;
		prevSwingPower_ = swingPower_;
		//つかんでいる時だけ振り回すときの当たり判定を有効にする
		bool isColActive = (isCatch_ && isRopeMove);
		swingCollider_.Set_IsActive((isColActive));
		swingCollider_.sphere_.center = pos_[ROPE_LENGTH - 1];

		//振り回す強さによって音のなる間隔を変える
		if (swingPower_ == SwingPower::WEAK) {
			swingSETimer_.SetLimitTime(50);
		}
		else if (swingPower_ == SwingPower::STRONG) {
			swingSETimer_.SetLimitTime(20);
		}
		if (swingPower_ != SwingPower::POWER_ZERO) {
			swingSETimer_.AddTime();
			if (swingSETimer_.GetIsEnd()) {
				swingSETimer_.Reset();
				SoundManager::Play("SwingSE");
			}
		}
	}
	if (swingPower_ == SwingPower::WEAK) {
		trail_->SetColor(Color::kYellow);
	}
	else if (swingPower_ == SwingPower::STRONG) {
		trail_->SetColor(Color::kRed);
	}
	else if (swingPower_ == SwingPower::POWER_ZERO) {
		trail_->SetColor(Color::kWhite);
	}

	if (serial_ == 0) {
		catchPos_ = pos_[ROPE_LENGTH - 1];
	}
	else {
		catchPos_ = pos_[0];
	}
	catchPos_.y = 0.1f;

	catchAngle_ += 0.01f;

	for (int i = 0; i < ROPE_LENGTH; i++) {
		//オブジェクトの更新
		ropeObj_[i]->SetPos(pos_[i]);

		ropeObj_[i]->Update();
	}
	catchObj_->SetPos(catchPos_);
	catchObj_->SetScale({ catchRadius_ * 2,catchRadius_ * 2,catchRadius_ * 2 });
	catchObj_->SetRot({ 0,catchAngle_,0 });

	//垂らしてない時はプレイヤーの真下に垂らす位置の目印として座標を代入
	if ((isUse_ == false || switchingTimer_ > 0) && !isAuto_) {
		catchObj_->SetPos({
			PlayerManager::GetInstance()->GetPos().x,
			0.1f,
			PlayerManager::GetInstance()->GetPos().z });
	}

	reliefLight_->SetPos({
		PlayerManager::GetInstance()->GetPos().x,
		65.f,
		PlayerManager::GetInstance()->GetPos().z });
	reliefLight_->SetScale(Vector3(20.f,35.f, 20.f));

	fireEmitter_->pos = reliefLight_->GetPos();
	//fireEmitter_->pos.y += 2;

	if (firstLightTimer_.GetIsEnd()) {
		lightTimer_.AddTime();
		float timer = (float)lightTimer_.GetTimer();
		float time = (float)lightTimer_.GetLimitTimer();
		float rate = timer / time;
		float color_a = Easing::Circ::easeIn(0.f, -1.f, rate);
		Color color = Color(0, 0, 0, color_a);
		reliefLight_->SetColor(color);
		if (lightTimer_.GetTimeRate() < 0.4f) {
			fireEmitter_->isActive = true;
		}
		else {
			fireEmitter_->isActive = false;
		}
	}
	else {
		if(isUse_)firstLightTimer_.AddTime();
		float timer = (float)firstLightTimer_.GetTimer();
		float time = (float)firstLightTimer_.GetLimitTimer();
		float rate = timer / time;
		float color_a = Easing::Circ::easeOut(-1.f, 0.f, rate);
		Color color = Color(0, 0, 0, color_a);
		reliefLight_->SetColor(color);
	}

	catchObj_->Update();
	reliefLight_->Update();

	oldPos_ = { pos_[0].x,PlayerManager::GetInstance()->GetPos().y,pos_[0].z };

	trail_->SetPos(pos_[3], pos_[9]);
	trail_->Update();
}

void Rope::Draw()
{
	trail_->Draw();
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	for (int i = 0; i < ROPE_LENGTH; i++) {
		if (PlayerManager::GetInstance()->GetPos().y >= pos_[i].y - 3.f && pos_[i].y > 0) {
			ropeObj_[i]->Draw();
		}
	}
	if (!(isUse_ && switchingTimer_ > 0)) {
		catchObj_->Draw();
	}
}

void Rope::DrawLight()
{
	reliefLight_->Draw();
}

void Rope::Finalize()
{
	swingCollider_.Finalize();
}

Rope::~Rope()
{
	//swingCollider_.Finalize();
	fireEmitter_->isActive = false;
}

void Rope::CallBack()
{
	//振り回してNormalHumanとぶつかったらつかまっているNormalHumanが消える
	if (swingCollider_.IsTrigger_Name("NormalHuman")) {
		//振り回していない場合は通さない
		if (isSwing_ == false)return;
		if (swingPower_ != SwingPower::STRONG) return;
		SphereCollider* ropeCol = static_cast<SphereCollider*>(swingCollider_.Extract_Collider("NormalHuman"));
		if (ropeCol == nullptr) return;
		//ロープにつかまっているNormalHumanは無効にする
		bool isCatch = ropeCol->Data_Get<bool>("isCatch");
		if (isCatch)return;
		//NormalHumanを消す
		for (auto& human : ropeCatchHumanList_) {
			const auto colName = human->GetColName();
			if (colName == "NormalHuman") {
				human->SetDead();
				human->SetIsAddScore(false);
				break;	//一人でもNormalHumanがいたら検索終了
			}
		}
	}
}

void Rope::Save()
{
	output_.Begin("rope_Config");

	output_.Output("catchRadius", catchRadius_);
	output_.Output("distance", distance);
	output_.Output("switchingTimer", switchingTimer_);
	output_.Output("switchingTime", switchingTime_);
	output_.Output("switchingSpeed", switchingSpeed_);
	output_.Output("nonCatchNormal", nonCatchNormal_);
	output_.Output("nonCatchAdd", nonCatchAdd_);
	output_.Output("nonCatchSubNormal", nonCatchSubNormal_);
	output_.Output("nonCatchSubAdd", nonCatchSubAdd_);
	output_.Output("catchNormal", catchNormal_);
	output_.Output("catchSub", catchSub_);
	output_.Output("weight", weight_);
	output_.Output("catchCount", catchCount_);
	output_.Output("swingWeakRange_", swingWeakRange_);
	output_.Output("swingStrongRange_", swingStrongRange_);
	output_.Output("swingTime", swingTime_);

	output_.End();
}

void Rope::Load()
{
	//読み込み
	std::ifstream file("application/Resources/Config/rope_Config.txt");  // 読み込むファイルのパスを指定
	std::string line;

	while (std::getline(file, line)) {  // 1行ずつ読み込む
		std::cout << line << std::endl;

		std::stringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');
		if (key == "catchRadius") {
			line_stream >> catchRadius_;
		}
		else if (key == "distance") {
			line_stream >> distance;
		}
		else if (key == "switchingTimer") {
			line_stream >> switchingTimer_;
		}
		else if (key == "switchingTime") {
			line_stream >> switchingTime_;
		}
		else if (key == "switchingSpeed") {
			line_stream >> switchingSpeed_;
		}
		else if (key == "nonCatchNormal") {
			line_stream >> nonCatchNormal_;
		}
		else if (key == "nonCatchAdd") {
			line_stream >> nonCatchAdd_;
		}
		else if (key == "nonCatchSubNormal") {
			line_stream >> nonCatchSubNormal_;
		}
		else if (key == "nonCatchSubAdd") {
			line_stream >> nonCatchSubAdd_;
		}
		else if (key == "catchNormal") {
			line_stream >> catchNormal_;
		}
		else if (key == "catchSub") {
			line_stream >> catchSub_;
		}
		else if (key == "weight") {
			line_stream >> weight_;
		}
		else if (key == "catchCount") {
			line_stream >> catchCount_;
		}
		else if (key == "swingStrongRange_") {
			line_stream >> swingStrongRange_;
		}
	}
}
