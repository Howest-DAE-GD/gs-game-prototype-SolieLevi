#pragma once

#include <vector>
#include "utils.h"

class Enemy;
class Door;

class Player
{
public:
	enum class Direction 
	{
		left,
		right,
		up,
		down,
		neutral,
		diagonalUpL,
		diagonalUpR,
		diagonalDownL,
		diagonalDownR
	};

	Player(const Point2f& center, float size);

	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& map, Door door);
	void Draw() const;

	Circlef GetBounds() const;
	Point2f GetPos();
	Direction GetDirection();
	bool GetAlive() const;

	void SetAlive(bool isAlive);
	void SetDirection(const Direction& direction);

	void SetPosition(const Point2f& pos);
	void SetPosition(float x, float y);

	void Reset(const Point2f& pos);

	int m_CoinCount{};

	int GetCoinCount() const;

private:
	Circlef m_CirclePlayer;
	Rectf m_Bounds;

	Color4f m_Color;
	Color4f m_DeadColor;

	const float m_Speed{150.f};
	float m_CurrentSpeed{ 150.f };

	Direction m_Direction;

	bool m_IsAlive;

	void CheckCollision(float elapsedSec, const std::vector<std::vector<Point2f>>& map, Door door);
	
	utils::HitInfo hit;

};

