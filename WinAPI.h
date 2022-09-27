#pragma once
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
class WinAPI
{
public:

	static WinAPI* GetInstance();

	void Ini();

	bool MsgCheck();

	void ReleaseClass();
	
	static LRESULT WindowProcA(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	// ウィンドウサイズ
	const int window_width = 1280; // 横幅
	const int window_height = 720; // 縦幅

	// ウィンドウクラスの設定
	WNDCLASSEX w{};

	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, window_width, window_height };

	HWND hwnd;
private:

};


