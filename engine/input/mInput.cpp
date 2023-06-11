#include <cassert>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include "mInput.h"

//DirectInput�̏�����
static IDirectInput8* directInput = nullptr;

#pragma region �L�[�{�[�h

DirectXInput* DirectXInput::GetInstance()
{
	static DirectXInput instance;
	return &instance;
}
void DirectXInput::InputIni()	//������
{
	HRESULT result;

	winapi_ = WinAPI::GetInstance();

	//DirectInput�̏�����
	result = DirectInput8Create(
		winapi_->w_.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//�L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);	//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = keyboard_->SetCooperativeLevel(
		winapi_->hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
	//�g���Ă���t���O�ɂ���
	//DISCL_FOREGROUND		��ʂ���O�ɂ���ꍇ�̂ݓ��͂��󂯕t����
	//DISCL_NONEXCLUSIVE	�f�o�C�X�����̃A�v�������Ő�L���Ȃ�
	//DISCL_NOWINKEY		Windows�L�[�𖳌������� 
	
	//�L�[�{�[�h���̎擾�J�n
	keyboard_->Acquire();
}

void DirectXInput::InputUpdata()	//�A�b�v�f�[�g
{
	for (uint32_t i = 0; i < 256; ++i)
	{
		oldkeys_[i] = keys_[i];
	}
	//�L�[�{�[�h���̎擾�J�n
	keyboard_->Acquire();
	keyboard_->GetDeviceState(sizeof(keys_), keys_);
}
//�������ςȂ�
bool DirectXInput::PushKey(UINT8 key)
{
	return keys_[key];
}
//�������u��
bool DirectXInput::TriggerKey(UINT8 key)
{
	return keys_[key] && !oldkeys_[key];
}
//�������u��
bool DirectXInput::GetKeyReleased(UINT8 key)
{
	return !keys_[key] && oldkeys_[key];
}
#pragma endregion

#pragma region �}�E�X
#define MOUSE_INPUT 0x80

MouseInput* MouseInput::GetInstance()
{
	static MouseInput instance;
	return &instance;
}

void MouseInput::MouseIni()
{
	HRESULT result;
	winapi_ = WinAPI::GetInstance();
	
	//�L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(result));
	//���̓f�[�^�`���̃Z�b�g
	result = mouse_->SetDataFormat(&c_dfDIMouse);	//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = mouse_->SetCooperativeLevel(
		winapi_->hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
	//�g���Ă���t���O�ɂ���
	//DISCL_FOREGROUND		��ʂ���O�ɂ���ꍇ�̂ݓ��͂��󂯕t����
	//DISCL_NONEXCLUSIVE	�f�o�C�X�����̃A�v�������Ő�L���Ȃ�
	//DISCL_NOWINKEY		Windows�L�[�𖳌������� 

	//�}�E�X���̎擾�J�n
	mouse_->Acquire();
}

void MouseInput::GetCursorPosition()
{
	//�X�N���[�����猩���}�E�X�̍��W���擾����
	GetCursorPos(&p_);
	//�E�B���h�E���猩���}�E�X�̍��W���擾����
	ScreenToClient(winapi_->hwnd_, &p_);
	//�O�t���[���̏�Ԃ�������
	prevmPos_ = mPos_;
	//���t���[���̍��W��������
	mPos_.x = (float)p_.x;
	mPos_.y = (float)p_.y;
	//�}�E�X���ǂ̕����ɓ��������̃x�N�g�����擾
	mouseVec_ = mPos_ - prevmPos_;
	//�x�N�g�����K��
	//mouseVec.normalize();
}
#include <string>
void MouseInput::Updata()
{
	HRESULT result;
	
	//�}�E�X���̎擾�J�n
	mouse_->Acquire();	//�����ɒu�������Ƃŉ���

	//std::string str = "OK\n";
	//�O�t���[���̏�Ԃ���
	prevmouseState_ = mouseState_;
	//�}�E�X���̎擾�J�n
	result = mouse_->Poll();
	if (result == DIERR_INPUTLOST) {
		//str = "NG\n";
	}
	
	result = mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState_);
	//�E�B���h�E�̊O���N���b�N��������͏��𖳌��ɂ���
	if (FAILED(result)) {
		mouseState_.lX = 0;
		mouseState_.lY = 0;
		mouseState_.lZ = 0;
		mouseState_.rgbButtons[0] = 0;
		mouseState_.rgbButtons[1] = 0;
		mouseState_.rgbButtons[2] = 0;
		mouseState_.rgbButtons[3] = 0;
	}
	//OutputDebugStringA(str.c_str());
	//���W�擾
	GetCursorPosition();
}
//���N���b�N���ꂽ��
bool MouseInput::IsMouseTrigger(BYTE button)
{
	if (!prevmouseState_.rgbButtons[button] &&
		mouseState_.rgbButtons[button]) {
		return true;
	}
	return false;
}

bool MouseInput::IsMouseDown(BYTE button)
{
	if (prevmouseState_.rgbButtons[button]) {
		return true;
	}
	return false;
}

bool MouseInput::IsMouseReleas(BYTE button)
{
	if ((prevmouseState_.rgbButtons[button]) &&
		!(mouseState_.rgbButtons[button])) {
		return true;
	}
	return false;
}

int32_t MouseInput::IsMouseWheel()
{
	return mouseState_.lZ;
}

Vector3 MouseInput::GetCursorMove()
{
	return mouseVec_;
}

float MouseInput::GetCursorMoveX()
{
	return mouseVec_.x;
}

float MouseInput::GetCursorMoveY()
{
	return mouseVec_.y;
}

float MouseInput::GetCursorMoveZ()
{
	return mouseVec_.z;
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
	Update();
}

void Controller::Update()
{
	DWORD result = S_OK;
	preState_ = state_;
	result = XInputGetState(
		0,       // DWORD         dwUserIndex
		&state_);

	if (result == ERROR_SUCCESS) {
		isConnect_ = true;
	}
	else {
		isConnect_ = false;
	}
}

WORD Controller::GetButtons(WORD button)
{
	if (state_.Gamepad.wButtons == button) {
		return true;
	}

	return false;
}

WORD Controller::GetTriggerButtons(WORD button)
{
	if ((state_.Gamepad.wButtons == button) &&
		(preState_.Gamepad.wButtons != button))
	{
		return true;
	}

	return false;
}

WORD Controller::GetReleasButtons(WORD button)
{
	if ((state_.Gamepad.wButtons != button) &&
		(preState_.Gamepad.wButtons == button))
	{
		return true;
	}

	return false;
}

Vector2 Controller::GetLStick()
{
	Vector2 stickPos;
	
	//���X�e�B�b�N
	stickPos.x = state_.Gamepad.sThumbLX;
	stickPos.y = state_.Gamepad.sThumbLY;
	//�f�b�h�]�[����ݒ�
	if ((state_.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE/* * 2.0f*/ &&
		state_.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE/* * 2.0f*/))
	{
		stickPos.x = 0;
	}

	if ((state_.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE/* * 2.0f*/ &&
		state_.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE/* * 2.0f*/))
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
	stickPos.x = state_.Gamepad.sThumbRX;
	stickPos.y = state_.Gamepad.sThumbRY;
	//�f�b�h�]�[����ݒ�
	if ((state_.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		state_.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(state_.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			state_.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		return Vector2(0, 0);
	}
	//�f�b�h�]�[���ɓ���Ȃ�������l��Ԃ�
	return stickPos;
}

BYTE Controller::GetRTrigger()
{
	return state_.Gamepad.bRightTrigger;
}

BYTE Controller::GetLTrigger()
{
	return state_.Gamepad.bLeftTrigger;
}

#pragma endregion