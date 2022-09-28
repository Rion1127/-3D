#pragma once
#include <dinput.h>
#include "Vector3.h"
#include <WinUser.h>
//コントローラ
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")

//キーボード
class DirectXInput
{
public:
	static DirectXInput* GetInstance();

	void InputIni(WNDCLASSEX w, HWND hwnd);
	void InputUpdata();

	bool IsKeyDown(UINT8 key);		//押しっぱなし
	bool IsKeyTrigger(UINT8 key);		//押した瞬間
	bool GetKeyReleased(UINT8 key);

private:
	IDirectInputDevice8* keyboard = nullptr;
	//全キーの入力状態を取得する
	BYTE keys[256] = {};
	//全キーの入力状態を取得する
	BYTE oldkeys[256] = {};
};
//マウス
class MouseInput {
public:
	static MouseInput* GetInstance();

	void MouseIni(HWND* hwnd);
	void Updata();

	//マウスボタン情報
	//[0] MOUSE_LEFT= 左ボタン
	//[1] MOUSE_RIGHT= 右ボタン
	//[2] MOUSE_WHEEL= マウスホイール
	bool IsMouseTrigger(BYTE button);
	//[0] MOUSE_LEFT= 左ボタン
	//[1] MOUSE_RIGHT= 右ボタン
	//[2] MOUSE_WHEEL= マウスホイール
	bool IsMouseDown(BYTE button);
	//[0] MOUSE_LEFT= 左ボタン
	//[1] MOUSE_RIGHT= 右ボタン
	//[2] MOUSE_WHEEL= マウスホイール
	bool IsMouseReleas(BYTE button);
	int IsMouseWheel();
	
	//マウスが1フレームに移動したベクトルを取得する
	Vector3 GetCursorMove();

	float GetCursorMoveX();
	float GetCursorMoveY();
	float GetCursorMoveZ();
	
	POINT p;
	//現フレームのマウスの位置
	Vector3 mPos;
	//前フレームのマウスの位置
	Vector3 prevmPos;
	//マウスが動いた方向のベクトル
	Vector3 mouseVec;
private:
	//マウスの座標を取得する
	void GetCursorPosition();

	IDirectInputDevice8* mouse = nullptr;

	DIMOUSESTATE mouseState;
	DIMOUSESTATE prevmouseState;
	HWND* hwnd_ = nullptr;
};
enum {
	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_WHEEL
};
//コントローラ
class Controller {
public:
	static Controller* GetInstance();

	void Ini();

	void Update();

	WORD GetButtons(WORD button);
	WORD GetTriggerButtons(WORD button);
	WORD GetReleasButtons(WORD button);
	//false	右スティック
	//true	左スティック
	Vector2 GetLStick();
	Vector2 GetRStick();

	BYTE GetRTrigger();
	BYTE GetLTrigger();
private:

	XINPUT_STATE state;
	XINPUT_STATE preState;

	//バイブレーション
	XINPUT_VIBRATION vibration;

};

