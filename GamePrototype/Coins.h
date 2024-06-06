#pragma once
#include "Player.h"

class Coins
{
public:
	Coins(const Point2f& pos);
	void Draw() const;

	void CheckPickedUp(Player& player);
	void Calibrate(float scale, float offsetX, float offsetY);

	void Reset();

private:
	Color4f m_Color{ 1.f,1.f,0.f,1.f };
	bool m_IsActive{ true };
	Point2f m_Center;
	const float size{ 5.f };

};

