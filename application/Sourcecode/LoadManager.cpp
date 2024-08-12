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
}

void LoadManager::LoadTexture()
{
	//二つ目の引数は頭文字を大文字にするように
	LoadGraph("white64x64.png", "White64");
	LoadGraph("white1280x720.png", "White1280x720");
	LoadGraph("white1x1.png", "White");
	LoadGraph("configCameraTex.png", "ConfigCameraTex");
	LoadGraph("title_proto.png", "Title");
	LoadGraph("title.png", "Test");
	LoadGraph("clear_proto.png", "GameClearTex");
	LoadGraph("box_error.png", "Box_error");

	LoadGraph("player/void.png", "Void");
	// プレイヤー
	{
		LoadGraph("player/player_idle.png", "Player_idle");
		LoadGraph("player/player_dash.png", "Player_dash");
		LoadGraph("player/player_jump_up.png", "Player_jump_up");
		LoadGraph("player/player_jump_down.png", "Player_jump_down");
		LoadGraph("player/player_smash.png", "Player_smash");
		LoadGraph("player/player_strike.png", "Player_strike");

		LoadGraph("player/hammer_smash.png", "Hammer_smash");
		LoadGraph("player/hammer_strike.png", "Hammer_strike");
		
		LoadGraph("player/player_spin.png", "Player_spin");
	}
	
	// 敵
	{
		// walker
		LoadGraph("enemy/walker/walker_idle.png", "Walker_idle");
		LoadGraph("enemy/walker/walker_walk.png", "Walker_walk");
		LoadGraph("enemy/walker/walker_hit.png", "Walker_hit");

		// pot
		LoadGraph("enemy/pot/pot_idle.png", "Pot_idle");
		LoadGraph("enemy/pot/pot_hit.png", "Pot_hit");

		// umbrella
		LoadGraph("enemy/umbrella/umbrella_idle.png", "Umbrella_idle");
		LoadGraph("enemy/umbrella/umbrella_jump.png", "Umbrella_jump");
		LoadGraph("enemy/umbrella/umbrella_float.png", "Umbrella_float");
		LoadGraph("enemy/umbrella/umbrella_hit.png", "Umbrella_hit");
		
		// pillbug
		LoadGraph("enemy/pillbug/pillbug_idle.png", "Pillbug_idle");
		LoadGraph("enemy/pillbug/pillbug_hit.png", "Pillbug_hit");

		// cannon_wheel
		LoadGraph("enemy/cannon/cannon_wheel_barrel_idle.png", "Cannon_wheel_barrel_idle");
		LoadGraph("enemy/cannon/cannon_wheel_body_idle.png", "Cannon_wheel_body_idle");
		LoadGraph("enemy/cannon/cannon_wheel_barrel_move.png", "Cannon_wheel_barrel_move");
		LoadGraph("enemy/cannon/cannon_wheel_body_move.png", "Cannon_wheel_body_move");
		LoadGraph("enemy/cannon/cannon_wheel_barrel_fire.png", "Cannon_wheel_barrel_fire");
		LoadGraph("enemy/cannon/cannon_wheel_body_fire.png", "Cannon_wheel_body_fire");
		LoadGraph("enemy/cannon/cannon_wheel_hit.png", "Cannon_wheel_hit");

		// cannon_turret
		LoadGraph("enemy/cannon/cannon_turret_barrel_idle.png", "Cannon_turret_barrel_idle");
		LoadGraph("enemy/cannon/cannon_turret_body_idle.png", "Cannon_turret_body_idle");
		LoadGraph("enemy/cannon/cannon_turret_barrel_fire.png", "Cannon_turret_barrel_fire");
		LoadGraph("enemy/cannon/cannon_turret_body_fire.png", "Cannon_turret_body_fire");
		LoadGraph("enemy/cannon/cannon_turret_hit.png", "Cannon_turret_hit");

		// cannon_bullet
		LoadGraph("enemy/cannon/cannon_bullet.png", "Cannon_bullet");
		
		// boss
		LoadGraph("enemy/boss/boss_body_front_idle.png", "Boss_body_front");
		LoadGraph("enemy/boss/boss_body_side_idle.png", "Boss_body_side");
		LoadGraph("enemy/boss/boss_hand_front_idle.png", "Boss_hand_front");
		LoadGraph("enemy/boss/boss_hand_back_idle.png", "Boss_hand_back");
	}

	// ステージ
	{
		// 床
		LoadGraph("stage/floor.png", "Floor");
		LoadGraph("stage/floor_outline.png", "Floor_outline");
		LoadGraph("stage/floor_front.png", "Floor_front");
		LoadGraph("stage/floor_break_front.png", "Floor_break");
	
		// クリスタル
		LoadGraph("stage/crystal.png", "Crystal");
		
		// ジャンプパッド
		LoadGraph("stage/axel_idle.png", "Jumppad_idle");
		LoadGraph("stage/axel_up.png", "Jumppad_jump");
	
		//BOX
		LoadGraph("stage/iron.png", "Iron");
		LoadGraph("stage/block.png", "Block");

		// ゲート
		LoadGraph("stage/gate_idle.png", "Gate_idle");
		
		// 矢印
		LoadGraph("stage/arrow_direction_idle.png", "Arrow_direction_idle");
		LoadGraph("stage/arrow_video_idle.png", "Arrow_video_idle");

		//レール
		LoadGraph("rail.png", "Rail");
	}

	// UI
	{
		LoadGraph("ui/numbers.png", "Numbers");
		LoadGraph("ui/boss_icon.png", "Boss_icon");
		
		LoadGraph("ui/player_hpgauge.png", "HP_player");
		LoadGraph("ui/player_hpgauge_mask.png", "HP_player_mask");
		LoadGraph("ui/boss_hpgauge.png", "HP_boss");
		LoadGraph("ui/boss_hpgauge_mask.png", "HP_boss_mask");
		
		LoadGraph("ui/ui_manual.png", "Manual");
		LoadGraph("ui/ui_cross.png", "UI_cross");
		
		LoadGraph("ui/pause_tag.png", "Pause_tag");
		LoadGraph("ui/pause_frame.png", "Pause_frame");
		LoadGraph("ui/sound_icon.png", "Sound_icon");
	}
	LoadGraph("ui/key_W.png", "key_W");
	LoadGraph("pauseItem.png", "PauseItem");
	LoadGraph("ui/move_Lstick.png", "LstickUI");
	LoadGraph("ui/pushButton_A.png", "PushButton_A");
	LoadGraph("ui/pushButton_B.png", "PushButton_B");
	LoadGraph("ui/pushButton_RT.png", "PushButton_RT");
	LoadGraph("coin.png", "Coin");
	LoadGraph("number.png", "Number");
	LoadGraph("Hpgauge.png", "Hpgauge");
	LoadGraph("Hpgauge_Enemy.png", "Hpgauge_Enemy");
	LoadGraph("menuFrame.png", "Menu_Frame");
	LoadGraph("soundFrame.png", "SoundFrame");

	//BOX
	LoadGraph("block/block.png", "Box");
	LoadGraph("block/wood.png", "Wood");
	
	//背景
	LoadGraph("bg.png", "BackGround");
	LoadGraph("gameover.png", "Gameover");

	//説明
	LoadGraph("stamp.png", "StampExplain");
	LoadGraph("smash.png", "SmashExplain");
	LoadGraph("move.png", "SmoveExplain");
	LoadGraph("jump.png", "SjumpExplain");
	LoadGraph("Break.png", "SbreakExplain");
	LoadGraph("drop.png", "SdropExplain");


	// エフェクト
	LoadGraph("effect/effect_primitive.png", "Effect_primitive");
	LoadGraph("effect/effect_rect.png", "Effect_rect");

    // imgui
    //-floor thumbnail--------------
    LoadGraph("imgui/floors/thumbnail_floors_normal.png","IMGUI_FLOOR_NORMAL");
    LoadGraph("imgui/floors/thumbnail_floors_platform.png","IMGUI_FLOOR_PLATFORM");
    LoadGraph("imgui/floors/thumbnail_floors_break.png","IMGUI_FLOOR_BREAK");
    LoadGraph("imgui/floors/thumbnail_floors_move.png","IMGUI_FLOOR_MOVE");
    
    //-gate thumbnail---------------
    LoadGraph("imgui/gates/thumbnail_gates_gate.png", "IMGUI_GATE_GATE");

    //-enemy thumbnail--------------
    LoadGraph("imgui/enemies/thumbnail_enemy_cannonTurret.png","IMGUI_ENEMY_CANNON-TURRET");
    LoadGraph("imgui/enemies/thumbnail_enemy_walker.png","IMGUI_ENEMY_WALKER");
    LoadGraph("imgui/enemies/thumbnail_enemy_potI.png","IMGUI_ENEMY_POT_I");
    LoadGraph("imgui/enemies/thumbnail_enemy_potU.png","IMGUI_ENEMY_POT_U");
    LoadGraph("imgui/enemies/thumbnail_enemy_umbrella.png","IMGUI_ENEMY_UMBRELLA");
    LoadGraph("imgui/enemies/thumbnail_enemy_rollsuquare.png","IMGUI_ENEMY_ROLLSQUARE");
    LoadGraph("imgui/enemies/thumbnail_enemy_boss.png","IMGUI_ENEMY_BOSS");
    LoadGraph("imgui/enemies/thumbnail_enemy_cannonWheel.png","IMGUI_ENEMY_CANNON-WHEEL");

    //-item thumbnail---------------
    LoadGraph("imgui/items/thumbnail_item_coin.png", "IMGUI_ITEM_COIN");

    //-gimmick thumbnail------------
    LoadGraph("imgui/gimmicks/thumbnail_gimmick_trampoline.png", "IMGUI_GIMMICK_TRAMPOLINE");
    LoadGraph("imgui/gimmicks/thumbnail_gimmick_seesaw.png", "IMGUI_GIMMICK_SEESAW");
    LoadGraph("imgui/gimmicks/thumbnail_gimmick_arrowSignboard.png", "IMGUI_GIMMICK_ARROWSIGNBOARD");

    //-event thumbnail--------------
    LoadGraph("imgui/events/thumbnail_event_dead.png", "IMGUI_EVENT_DEAD");
    LoadGraph("imgui/events/thumbnail_event_enemyPop.png", "IMGUI_EVENT_ENEMYPOP");
    LoadGraph("imgui/events/thumbnail_event_changeBGM.png", "IMGUI_EVENT_CHANGE_BGM");

    //-others thumbnail-------------
    LoadGraph("imgui/others/thumbnail_others_magnifyingGlass.png", "FOR_IMGUI_MAGNIFYING-GLASS");
    LoadGraph("imgui/others/thumbnail_others_emptyMark.png", "FOR_IMGUI_EMPTY-MARK");
    LoadGraph("imgui/others/thumbnail_others_pin_monochrome.png", "FOR_IMGUI_PIN-MONOCHROME");
    LoadGraph("imgui/others/thumbnail_others_pin_color.png", "FOR_IMGUI_PIN-COLOR");
    LoadGraph("imgui/others/thumbnail_others_crossMark.png", "FOR_IMGUI_CROSS-MARK");
    LoadGraph("imgui/others/thumbnail_others_arrow_left.png", "FOR_IMGUI_ARROW-LEFT");
    LoadGraph("imgui/others/thumbnail_others_arrow_right.png", "FOR_IMGUI_ARROW-RIGHT");
    LoadGraph("imgui/others/thumbnail_others_hand.png", "FOR_IMGUI_HAND");
    LoadGraph("imgui/others/thumbnail_others_player.png", "FOR_IMGUI_PLAYER");
    LoadGraph("imgui/others/thumbnail_others_player.png", "FOR_IMGUI_PLAYER");
    LoadGraph("imgui/others/thumbnail_others_trashcan_close.png", "FOR_IMGUI_TRASHCAN_CLOSE");
    LoadGraph("imgui/others/thumbnail_others_trashcan_open.png", "FOR_IMGUI_TRASHCAN_OPEN");
}

void LoadManager::LoadSound()
{
	//二つ目の引数は頭文字を大文字にするように
	//SE
	LoadWave("swingSE.wav", "SwingSE", SoundType::SE);				//ハンマーを振る音
	LoadWave("jumpSE.wav", "JumpSE", SoundType::SE);				//ジャンプ音
	LoadWave("dashSE.wav", "DashSE", SoundType::SE);				//ダッシュ音
	LoadWave("gameClearSE.wav", "ClearSE", SoundType::SE);			//クリア音
	LoadWave("damageSE.wav", "DamageSE", SoundType::SE);			//ダメージ音
	LoadWave("enterTheDoorSE.wav", "EnterTheDoorSE", SoundType::SE);//ゲートに入る時の音
	LoadWave("blockColSE.wav", "BlockColSE", SoundType::SE);		//ブロックがぶつかった時の音
	LoadWave("hammerSmashSE.wav", "HammerSmashSE", SoundType::SE);	//横振りのハンマーが当たった時の音時の音
	LoadWave("hammerStrikeSE.wav", "HammerStrikeSE", SoundType::SE);//縦振りのハンマーが当たった時の音時の音
	LoadWave("stampSE.wav", "StampSE", SoundType::SE);				//急降下
	LoadWave("shockWave.wav", "ShockWave", SoundType::SE);			//ショックウェーブ
	LoadWave("shotSE.wav", "ShotSE", SoundType::SE);				//敵の弾
	LoadWave("trampolineSE.wav", "TrampolineSE", SoundType::SE);	//トランポリン
	LoadWave("coinSE.wav", "CoinSE", SoundType::SE);	//コイン
	LoadWave("canSmashSE.wav", "CanSmashSE", SoundType::SE);	//柔らかめのハンマーのSE
	LoadWave("canNotSmashSE.wav", "CanNotSmashSE", SoundType::SE);	//固めのハンマーのSE
	LoadWave("playerDamageSE.wav", "PlayerDamageSE", SoundType::SE);	//固めのハンマーのSE

	LoadWave("pauseMoveSE.wav", "PauseMoveSE", SoundType::SE);	//メニュー項目移動のSE
	LoadWave("pauseSE.wav", "PauseSE", SoundType::SE);	//メニューを開くのSE
	LoadWave("redoSE.wav", "RedoSE", SoundType::SE);	//メニューの戻る時のSE
	LoadWave("pauseSelectSE.wav", "PauseSelectSE", SoundType::SE);	//メニューの戻る時のSE
	LoadWave("playerDead.wav", "PlayerDead", SoundType::SE);	//メニューの戻る時のSE

	LoadWave("metalBoxHit.wav", "MetalBoxHit", SoundType::SE);	//木箱のSE
	LoadWave("breakBoxHit.wav", "BreakBoxHit", SoundType::SE);	//metalBoxのSE

	//敵関連
	LoadWave("springJump.wav", "EnemySpringJump", SoundType::SE);	//ばね敵の跳ねる音
	LoadWave("bossMove.wav", "BossMove", SoundType::SE);	//ボスが動いているときの音
	LoadWave("fistPunch.wav", "bossBumpOfFist", SoundType::SE);	//拳がぶつかる音 仮
	LoadWave("bossWarpSE2.wav", "bossTeleport", SoundType::SE);	//ワープ音



	//BGM
	LoadWave("gameSceneBGM.wav", "GameSceneBGM", SoundType::BGM);	//ゲームのメインBGM
	LoadWave("bossBattleBGM.wav", "BossBattleBGM", SoundType::BGM);	//ボス戦のBGM
}
