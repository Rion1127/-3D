#pragma comment(lib,"winmm.lib")
#include "WinAPI.h"
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hwnd, UINT msg,WPARAM wparam, LPARAM lparam);

// �E�B���h�E�v���V�[�W��
LRESULT WinAPI::WindowProcA(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//ImGui�p�E�B���h�E�v���V�[�W���Ăяo��
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}
	// ���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg) {
		// �E�B���h�E���j�����ꂽ
	case WM_DESTROY:
		// OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}
	// �W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WinAPI* WinAPI::GetInstance()
{
	static WinAPI instance;
	return &instance;
}

void WinAPI::Ini()
{
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProcA;		// �E�B���h�E�v���V�[�W����ݒ�
	w.lpszClassName = L"DirectXGame";			// �E�B���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr);		// �E�B���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW);	// �J�[�\���w��
	// �E�B���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&w);
	
	// �����ŃT�C�Y��␳����
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	hwnd = CreateWindow(w.lpszClassName,	// �N���X��
		L"DirectXGame",						// �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,				// �W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,						// �\��X���W(OS�ɔC����)
		CW_USEDEFAULT,						// �\��Y���W(OS�ɔC����)
		wrc.right - wrc.left,				// �E�B���h�E����
		wrc.bottom - wrc.top,				// �E�B���h�E�c��
		nullptr,							// �e�E�B���h�E�n���h��
		nullptr,							// ���j���[�n���h��
		w.hInstance,						// �Ăяo���A�v���P�[�V�����n���h��
		nullptr);							// �I�v�V����
		// �E�B���h�E��\����Ԃɂ���
	ShowWindow(hwnd, SW_SHOW);

	//�V�X�e���^�C�}�[�̕���\���グ��
	timeBeginPeriod(1);
}

bool WinAPI::MsgCheck()
{
	MSG msg{}; // ���b�Z�[�W

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // ���b�Z�[�W������H
	{
		TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg);  // �E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	if (msg.message == WM_QUIT) // �I�����b�Z�[�W�������烋�[�v�𔲂���
	{
		return true;
	}

	return false;
}

void WinAPI::ReleaseClass()
{
	// �E�B���h�E�N���X��o�^����
	UnregisterClass(w.lpszClassName, w.hInstance);
}



