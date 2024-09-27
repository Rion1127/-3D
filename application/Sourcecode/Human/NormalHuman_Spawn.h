#pragma once
#include "StateMachine.h"
#include "GameStatus.h"
#include "NormalHuman.h"

class NormalHuman;

class NormalHuman_Spawn :
    public State<HumanState>
{
private:
    NormalHuman* human_;
public:
    NormalHuman_Spawn(NormalHuman* human) :State<HumanState>(HumanState::SPAWN), human_(human) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
};

