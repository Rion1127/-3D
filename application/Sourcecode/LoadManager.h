#pragma once
#include "Texture.h"
#include "mSound.h"
#include "ModelManager.h"

/**
 * @file LoadManager.h
 * @brief リソースを読み込みをまとめている
 */

class LoadManager
{
private:

public:
	void LoadAllResources();

	void LoadModel();
	void LoadTexture();
	void LoadSound();
	/// <summary>
	/// assimpモデル読み込み
	/// </summary>
	/// <param name="fileName"></param>
	/// <param name="name"></param>
	inline void LoadAssimpModel(const std::string& fileName, const std::string& keyName) {
		ModelManager::GetInstance()->LoadAssimpModel(fileName, keyName);
	}
	/// <summary>
	/// objモデル読み込み
	/// </summary>
	/// <param name="fileName"></param>
	/// <param name="smoothing"></param>
	/// <param name="isShadow"></param>
	/// <param name="keyName"></param>
	inline void LoadObjModel(const std::string& fileName, bool smoothing, bool isShadow, const std::string& keyName) {
		ModelManager::GetInstance()->LoadModel(fileName, smoothing, isShadow, keyName);
	}

	/// <summary>
	/// 画像読み込み
	/// </summary>
	/// <param name="fileName">画像のファイル名を入力</param>
	/// <param name="name">画像を呼び出すときのキーワードを設定</param>
	inline void LoadGraph(const std::string& fileName, const std::string& name) {
		TextureManager::GetInstance()->LoadGraph(fileName, name);
	}
	/// <summary>
	/// 音源読み込み(.waveのみ)
	/// </summary>
	/// <param name="fileName">音源のファイル名を入力</param>
	/// <param name="name">音源を呼び出すときのキーワードを設定</param>
	inline void LoadWave(const std::string& fileName, const std::string& name,const SoundType& soundType) {
		SoundManager::LoadWave(fileName, name, soundType);
	}
};

