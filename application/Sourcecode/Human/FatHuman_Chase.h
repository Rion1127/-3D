#pragma once

#include "StateMachine.h"
#include "GameStatus.h"
#include "DebuHuman.h"

class DebuHuman;

class DebuHuman_Chase :
    public State<HumanState>
{
private:
    DebuHuman* human_;
public:
    DebuHuman_Chase(DebuHuman* human) :State<HumanState>(HumanState::CHASE), human_(human) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
};

