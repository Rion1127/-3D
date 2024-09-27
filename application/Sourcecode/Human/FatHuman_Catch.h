#pragma once
#include "StateMachine.h"
#include "GameStatus.h"
#include "DebuHuman.h"

class DebuHuman;

class DebuHuman_Catch :
    public State<HumanState>
{
private:
    DebuHuman* human_;
public:
    DebuHuman_Catch(DebuHuman* human) :State<HumanState>(HumanState::CATCH), human_(human) {}

    void SetUp() override;
    void Update() override;
    void Draw() override;
    void CleanUp() override;
};

