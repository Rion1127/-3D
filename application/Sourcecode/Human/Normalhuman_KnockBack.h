#pragma once
#include "StateMachine.h"
#include "GameStatus.h"
#include "NormalHuman.h"

class NormalHuman;

class Normalhuman_KnockBack :
    public State<HumanState>
{
private:
    NormalHuman* human_;
public:
    Normalhuman_KnockBack(NormalHuman* human) :State<HumanState>(HumanState::KNOCKBACK), human_(human) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
private:

};

