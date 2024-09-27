#pragma once
#include "StateMachine.h"
#include "GameStatus.h"
#include "NormalHuman.h"

class NormalHuman;

class NormalHuman_Catch :
    public State<HumanState>
{
private:
    NormalHuman* human_;
public:
    NormalHuman_Catch(NormalHuman* human) :State<HumanState>(HumanState::CATCH), human_(human) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
};

