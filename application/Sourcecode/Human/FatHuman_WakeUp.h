#pragma once
#include "StateMachine.h"
#include "GameStatus.h"
#include "DebuHuman.h"
#include "Timer.h"

class DebuHuman;

class DebuHuman_WakeUp :
    public State<HumanState>
{
private:
    DebuHuman* human_;
public:
    DebuHuman_WakeUp(DebuHuman* human) :State<HumanState>(HumanState::WAKEUP), human_(human) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
private:
    Timer wakeUpTimer_;
};

