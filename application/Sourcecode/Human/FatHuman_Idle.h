#pragma once
#include "StateMachine.h"
#include "GameStatus.h"
#include "DebuHuman.h"

class DebuHuman;

class DebuHuman_Idle :
    public State<HumanState>
{
private:
    DebuHuman* human_;
public:
    DebuHuman_Idle(DebuHuman* human) :State<HumanState>(HumanState::IDLE), human_(human) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
};

