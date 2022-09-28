#include <cassert>
#define DIRECTINPUT_VERSION		0x0800	//DirectInputのバージョン指定
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include "DirectXInput.h"

//DirectInputの初期化
static IDirectInput8* directInput = nullptr;

#pragma region キーボード

DirectXInput* DirectXInput::GetInstance()
{
	static DirectXInput instance;
	return &instance;
}
void DirectXInput::InputIni(WNDCLASSEX w, HWND hwnd)	//初期化
{
	HRESULT result;
	//DirectInputの初期化
	//static IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(
		w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//キーボードデバイスの生成
	//IDirectInputDevice8* keyboard = nullptr;
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);	//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);		//使っているフラグについて
	assert(SUCCEEDED(result));												//DISCL_FOREGROUND		画面が手前にある場合のみ入力を受け付ける
																			//DISCL_NONEXCLUSIVE	デバイスをこのアプリだけで占有しない
																			//DISCL_NOWINKEY		Windowsキーを無効化する 
	//キーボード情報の取得開始
	keyboard->Acquire();
}

void DirectXInput::InputUpdata()	//アップデート
{
	for (int i = 0; i < 256; ++i)
	{
		oldkeys[i] = keys[i];
	}

	keyboard->GetDeviceState(sizeof(keys), keys);
}
//押しっぱなし
bool DirectXInput::IsKeyDown(UINT8 key)
{
	return keys[key];
}
//押した瞬間
bool DirectXInput::IsKeyTrigger(UINT8 key)
{
	return keys[key] && !oldkeys[key];
}
//離した瞬間
bool DirectXInput::GetKeyReleased(UINT8 key)
{
	return !keys[key] && oldkeys[key];
}
#pragma endregion

#pragma region マウス
#define MOUSE_INPUT 0x80

MouseInput* MouseInput::GetInstance()
{
	static MouseInput instance;
	return &instance;
}

void MouseInput::MouseIni(HWND* hwnd)
{
	HRESULT result;
	assert(SUCCEEDED(hwnd));
	hwnd_ = hwnd;
	//キーボードデバイスの生成
	//IDirectInputDevice8* keyboard = nullptr;
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	assert(SUCCEEDED(result));
	//入力データ形式のセット
	result = mouse->SetDataFormat(&c_dfDIMouse);	//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = mouse->SetCooperativeLevel(
		*hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
	//使っているフラグについて
	//DISCL_FOREGROUND		画面が手前にある場合のみ入力を受け付ける
	//DISCL_NONEXCLUSIVE	デバイスをこのアプリだけで占有しない
	//DISCL_NOWINKEY		Windowsキーを無効化する 

	//マウス情報の取得開始
	mouse->Acquire();
}

void MouseInput::GetCursorPosition()
{
	//スクリーンから見たマウスの座標を取得する
	GetCursorPos(&p);
	//ウィンドウから見たマウスの座標を取得する
	ScreenToClient(*hwnd_, &p);
	//前フレームの状態を代入する
	prevmPos = mPos;
	//現フレームの座標を代入する
	mPos.x = (float)p.x;
	mPos.y = (float)p.y;
	//マウスがどの方向に動いたかのベクトルを取得
	mouseVec = mPos - prevmPos;
	//ベクトル正規化
	//mouseVec.normalize();
}

void MouseInput::Updata()
{
	//前フレームの状態を代入
	prevmouseState = mouseState;
	//マウス情報の取得開始
	//mouse->Acquire();
	mouse->Poll();
	mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
	//座標取得
	GetCursorPosition();
}
//左クリックされたら
bool MouseInput::IsMouseTrigger(BYTE button)
{
	if (!prevmouseState.rgbButtons[button] &&
		mouseState.rgbButtons[button]) {
		return true;
	}
	return false;
}

bool MouseInput::IsMouseDown(BYTE button)
{
	if (prevmouseState.rgbButtons[button]) {
		return true;
	}
	return false;
}

bool MouseInput::IsMouseReleas(BYTE button)
{
	if ((prevmouseState.rgbButtons[button]) &&
		!(mouseState.rgbButtons[button])) {
		return true;
	}
	return false;
}

int MouseInput::IsMouseWheel()
{
	return mouseState.lZ;
}

Vector3 MouseInput::GetCursorMove()
{
	return mouseVec;
}

float MouseInput::GetCursorMoveX()
{
	return mouseVec.x;
}

float MouseInput::GetCursorMoveY()
{
	return mouseVec.y;
}

float MouseInput::GetCursorMoveZ()
{
	return mouseVec.z;
}
#pragma endregion

#pragma region コントローラ
Controller* Controller::GetInstance()
{
	static Controller instance;
	return &instance;
}

void Controller::Ini()
{
	preState = state;
	XInputGetState(
		0,       // DWORD         dwUserIndex
		&state);
}

void Controller::Update()
{
	preState = state;
	XInputGetState(
		0,       // DWORD         dwUserIndex
		&state);
}

WORD Controller::GetButtons(WORD button)
{
	if (state.Gamepad.wButtons == button) {
		return true;
	}

	return false;
}

WORD Controller::GetTriggerButtons(WORD button)
{
	if ((state.Gamepad.wButtons == button) &&
		(preState.Gamepad.wButtons != button))
	{
		return true;
	}

	return false;
}

WORD Controller::GetReleasButtons(WORD button)
{
	if ((state.Gamepad.wButtons != button) &&
		(preState.Gamepad.wButtons == button))
	{
		return true;
	}

	return false;
}

Vector2 Controller::GetLStick()
{
	Vector2 stickPos;
	
	//左スティック
	stickPos.x = state.Gamepad.sThumbLX;
	stickPos.y = state.Gamepad.sThumbLY;
	//デッドゾーンを設定
	if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 2.0f &&
		state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 2.0f))
	{
		stickPos.x = 0;
	}

	if ((state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 2.0f &&
		state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 2.0f))
	{
		stickPos.y = 0;
	}

	return stickPos;
}

Vector2 Controller::GetRStick()
{
	Vector2 stickPos;
	//右スティック
		//returnする変数に値を代入
	stickPos.x = state.Gamepad.sThumbRX;
	stickPos.y = state.Gamepad.sThumbRY;
	//デッドゾーンを設定
	if ((state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		return Vector2(0, 0);
	}
	//デッドゾーンに入らなかったら値を返す
	return stickPos;
}

BYTE Controller::GetRTrigger()
{
	return state.Gamepad.bRightTrigger;
}

BYTE Controller::GetLTrigger()
{
	return state.Gamepad.bLeftTrigger;
}

#pragma endregion