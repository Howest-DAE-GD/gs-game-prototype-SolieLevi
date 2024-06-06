#pragma once
#include "utils.h"
#include "Texture.h"

class Player;
class Enemy;
class PowerUp
{
public:
	PowerUp(int Nr);
	void Draw() const;
	void PickUp(const Player& player, std::vector<Enemy>& enemies);
	void Activate(std::vector<Enemy>& enemies);

	void Reset();
	void Calibrate(float scale, float offsetX, float offsetY);
private:
	bool m_PickedUp;
	Rectf m_Bounds;

	Texture m_Texture{ "powerup.png" };
};

