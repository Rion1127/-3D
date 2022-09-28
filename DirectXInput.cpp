#include <cassert>
#define DIRECTINPUT_VERSION		0x0800	//DirectInput�̃o�[�W�����w��
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include "DirectXInput.h"

//DirectInput�̏�����
static IDirectInput8* directInput = nullptr;

#pragma region �L�[�{�[�h

DirectXInput* DirectXInput::GetInstance()
{
	static DirectXInput instance;
	return &instance;
}
void DirectXInput::InputIni(WNDCLASSEX w, HWND hwnd)	//������
{
	HRESULT result;
	//DirectInput�̏�����
	//static IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(
		w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//�L�[�{�[�h�f�o�C�X�̐���
	//IDirectInputDevice8* keyboard = nullptr;
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);	//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);		//�g���Ă���t���O�ɂ���
	assert(SUCCEEDED(result));												//DISCL_FOREGROUND		��ʂ���O�ɂ���ꍇ�̂ݓ��͂��󂯕t����
																			//DISCL_NONEXCLUSIVE	�f�o�C�X�����̃A�v�������Ő�L���Ȃ�
																			//DISCL_NOWINKEY		Windows�L�[�𖳌������� 
	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();
}

void DirectXInput::InputUpdata()	//�A�b�v�f�[�g
{
	for (int i = 0; i < 256; ++i)
	{
		oldkeys[i] = keys[i];
	}

	keyboard->GetDeviceState(sizeof(keys), keys);
}
//�������ςȂ�
bool DirectXInput::IsKeyDown(UINT8 key)
{
	return keys[key];
}
//�������u��
bool DirectXInput::IsKeyTrigger(UINT8 key)
{
	return keys[key] && !oldkeys[key];
}
//�������u��
bool DirectXInput::GetKeyReleased(UINT8 key)
{
	return !keys[key] && oldkeys[key];
}
#pragma endregion

#pragma region �}�E�X
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
	//�L�[�{�[�h�f�o�C�X�̐���
	//IDirectInputDevice8* keyboard = nullptr;
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	assert(SUCCEEDED(result));
	//���̓f�[�^�`���̃Z�b�g
	result = mouse->SetDataFormat(&c_dfDIMouse);	//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = mouse->SetCooperativeLevel(
		*hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
	//�g���Ă���t���O�ɂ���
	//DISCL_FOREGROUND		��ʂ���O�ɂ���ꍇ�̂ݓ��͂��󂯕t����
	//DISCL_NONEXCLUSIVE	�f�o�C�X�����̃A�v�������Ő�L���Ȃ�
	//DISCL_NOWINKEY		Windows�L�[�𖳌������� 

	//�}�E�X���̎擾�J�n
	mouse->Acquire();
}

void MouseInput::GetCursorPosition()
{
	//�X�N���[�����猩���}�E�X�̍��W���擾����
	GetCursorPos(&p);
	//�E�B���h�E���猩���}�E�X�̍��W���擾����
	ScreenToClient(*hwnd_, &p);
	//�O�t���[���̏�Ԃ�������
	prevmPos = mPos;
	//���t���[���̍��W��������
	mPos.x = (float)p.x;
	mPos.y = (float)p.y;
	//�}�E�X���ǂ̕����ɓ��������̃x�N�g�����擾
	mouseVec = mPos - prevmPos;
	//�x�N�g�����K��
	//mouseVec.normalize();
}

void MouseInput::Updata()
{
	//�O�t���[���̏�Ԃ���
	prevmouseState = mouseState;
	//�}�E�X���̎擾�J�n
	//mouse->Acquire();
	mouse->Poll();
	mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
	//���W�擾
	GetCursorPosition();
}
//���N���b�N���ꂽ��
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

#pragma region �R���g���[��
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
	
	//���X�e�B�b�N
	stickPos.x = state.Gamepad.sThumbLX;
	stickPos.y = state.Gamepad.sThumbLY;
	//�f�b�h�]�[����ݒ�
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
	//�E�X�e�B�b�N
		//return����ϐ��ɒl����
	stickPos.x = state.Gamepad.sThumbRX;
	stickPos.y = state.Gamepad.sThumbRY;
	//�f�b�h�]�[����ݒ�
	if ((state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		return Vector2(0, 0);
	}
	//�f�b�h�]�[���ɓ���Ȃ�������l��Ԃ�
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