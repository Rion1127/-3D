#pragma once
#include "StateMachine.h"
#include "GameStatus.h"
#include "NormalHuman.h"
#include "Timer.h"

class NormalHuman;

class NormalHuman_WakeUp :
    public State<HumanState>
{
private:
    NormalHuman* human_;
public:
    NormalHuman_WakeUp(NormalHuman* human) :State<HumanState>(HumanState::WAKEUP), human_(human) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
private:
    Timer wakeUpTimer_;
};

