#pragma once
#include <dinput.h>
#include "Vector3.h"
#include <WinUser.h>
#include "WinAPI.h"
//�R���g���[��
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")

//�L�[�{�[�h
class DirectXInput
{
public:
	static DirectXInput* GetInstance();

	void InputIni();
	void InputUpdata();

	bool PushKey(UINT8 key);		//�������ςȂ�
	bool TriggerKey(UINT8 key);		//�������u��
	bool GetKeyReleased(UINT8 key);

private:
	IDirectInputDevice8* keyboard = nullptr;
	//�S�L�[�̓��͏�Ԃ��擾����
	BYTE keys[256] = {};
	//�S�L�[�̓��͏�Ԃ��擾����
	BYTE oldkeys[256] = {};

	WinAPI* winapi_ = nullptr;
};
//�}�E�X
class MouseInput {
public:
	static MouseInput* GetInstance();

	void MouseIni();
	void Updata();

	//�}�E�X�{�^�����
	//[0] MOUSE_LEFT= ���{�^��
	//[1] MOUSE_RIGHT= �E�{�^��
	//[2] MOUSE_WHEEL= �}�E�X�z�C�[��
	bool IsMouseTrigger(BYTE button);
	//[0] MOUSE_LEFT= ���{�^��
	//[1] MOUSE_RIGHT= �E�{�^��
	//[2] MOUSE_WHEEL= �}�E�X�z�C�[��
	bool IsMouseDown(BYTE button);
	//[0] MOUSE_LEFT= ���{�^��
	//[1] MOUSE_RIGHT= �E�{�^��
	//[2] MOUSE_WHEEL= �}�E�X�z�C�[��
	bool IsMouseReleas(BYTE button);
	int IsMouseWheel();
	
	//�}�E�X��1�t���[���Ɉړ������x�N�g�����擾����
	Vector3 GetCursorMove();

	float GetCursorMoveX();
	float GetCursorMoveY();
	float GetCursorMoveZ();
	
	POINT p;
	//���t���[���̃}�E�X�̈ʒu
	Vector3 mPos;
	//�O�t���[���̃}�E�X�̈ʒu
	Vector3 prevmPos;
	//�}�E�X�������������̃x�N�g��
	Vector3 mouseVec;
private:
	//�}�E�X�̍��W���擾����
	void GetCursorPosition();

	IDirectInputDevice8* mouse = nullptr;

	DIMOUSESTATE mouseState;
	DIMOUSESTATE prevmouseState;
	//HWND* hwnd_ = nullptr;
	WinAPI* winapi_ = nullptr;
};
enum {
	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_WHEEL
};
//�R���g���[��
class Controller {
public:
	static Controller* GetInstance();

	void Ini();

	void Update();

	WORD GetButtons(WORD button);
	WORD GetTriggerButtons(WORD button);
	WORD GetReleasButtons(WORD button);
	//false	�E�X�e�B�b�N
	//true	���X�e�B�b�N
	Vector2 GetLStick();
	Vector2 GetRStick();

	BYTE GetRTrigger();
	BYTE GetLTrigger();
private:

	XINPUT_STATE state;
	XINPUT_STATE preState;

	//�o�C�u���[�V����
	XINPUT_VIBRATION vibration;

};
