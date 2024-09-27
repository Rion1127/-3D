#pragma once
#include "StateMachine.h"
#include "GameStatus.h"
#include "DebuHuman.h"

class DebuHuman;

class DebuHuman_KnockBack :
    public State<HumanState>
{
private:
    DebuHuman* human_;
public:
    DebuHuman_KnockBack(DebuHuman* human) :State<HumanState>(HumanState::KNOCKBACK), human_(human) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
private:

};

