#pragma once
#include "StateMachine.h"
#include "GameStatus.h"
#include "NormalHuman.h"

class NormalHuman;

class NormalHuman_Idle :
    public State<HumanState>
{
private:
    NormalHuman* human_;
public:
    NormalHuman_Idle(NormalHuman* human) :State<HumanState>(HumanState::IDLE), human_(human) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
};

