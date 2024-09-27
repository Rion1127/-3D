#pragma once
#include "StateMachine.h"
#include "AssimpObject3D.h"
#include "GameStatus.h"

template<typename T>
class AssimpObjects {
public:
	AssimpObjects(void) = default;
	~AssimpObjects(void) = default;
	/// <summary>
	/// 指定の状態へ移行します。
	/// </summary>
	/// <param name="nextStateId"> 移行先の状態ID </param>
	void ChangeState(T nextStateId) {
		//配列が存在しない場合は返す
		if (!objList_.contains(nextStateId)) {
			return;
		}
		state_ = nextStateId;
		std::unique_ptr<AssimpObject3D> obj = std::make_unique<AssimpObject3D>();
		obj->SetModel(objList_[nextStateId]);
		obj->Init(isActiveShadow);
		currentObj_ = std::move(obj);
	}
	/// <summary>
	/// 現在の状態のIDを返します。
	/// </summary>
	/// <returns> 現在の状態のID </returns>
	const T GetCurrentStateName() const { return state_; }
public:
	T state_;
	/// <summary>
	/// モデルリスト
	/// </summary>
	std::unordered_map<T, AssimpModel*> objList_;
	/// <summary>
	/// 現在の状態
	/// </summary>
	std::unique_ptr<AssimpObject3D> currentObj_;

	bool isActiveShadow = true;
};
