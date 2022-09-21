#pragma once
#include "Player.h"
#include "Enemy.h"
void CheckAllCollisions(Player& player, Enemy& enemy_);

bool RayCollision(WorldTransform ray, WorldTransform obj);

