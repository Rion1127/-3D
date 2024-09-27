#pragma once

#include "StateMachine.h"
#include "GameStatus.h"
#include "NormalHuman.h"

class NormalHuman;

class NormalHuman_Chase :
    public State<HumanState>
{
private:
    NormalHuman* human_;
public:
    NormalHuman_Chase(NormalHuman* human) :State<HumanState>(HumanState::CHASE), human_(human) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
};

