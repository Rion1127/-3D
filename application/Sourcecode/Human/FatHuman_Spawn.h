#pragma once
#include "StateMachine.h"
#include "GameStatus.h"
#include "DebuHuman.h"

class DebuHuman;

class DebuHuman_Spawn :
    public State<HumanState>
{
private:
    DebuHuman* human_;
public:
    DebuHuman_Spawn(DebuHuman* human) :State<HumanState>(HumanState::SPAWN), human_(human) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
};

