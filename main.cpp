#include <Windows.h>
#include "Game.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	Game* game = new Game;

	//初期化
	game->Init();
	//更新
	game->Run();

	return 0;
}