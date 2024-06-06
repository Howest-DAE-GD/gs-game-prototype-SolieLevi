#pragma once
#include <vector>
#include "utils.h"

class Player;

class Enemy
{
public:
	enum class Direction
	{
		left,
		right,
		up,
		down,
		neutral
	};
	Enemy(const Point2f& center, const Point2f& end , int maxDegree, int minDegree);
	void Update(float elapsedSec);
	void Draw() const;
	void turn(float elapsedSec);

	void SetTimer(float timer);
	float GetTimer() const;

	void CheckSeen(Player& player);
	void Calibrate(float scale,float offsetX,float offsetY);
	void Reset();

	void SetSpeed( int speedIncrease );
private:
	Point2f m_Center;
	Color4f m_ConeColor;
	Color4f m_EnemyColor;
	const Color4f m_DisabledColor{ 0.f,0.f,1.f,1.f };
	const Color4f m_DisabledCone{ 1.f,0.f,0.f,0.2f };
	Rectf m_Bounds;

	float m_Angle;
	int m_MaxDegree;
	int m_MinDegree;
	int m_AngleIncrement{ 160 };
	int m_currentIncrement{};

	float m_Timer{ 0.f};
	float m_TimerMax{1.f};

	float m_Size{};

	bool m_Turn{};
	float m_PowerUpTimer{};

	Point2f m_Start;
	Point2f m_End;

	Vector2f m_Velocity;

	Vector2f m_OriginalVelocity;
	std::vector<Point2f> m_VisionCone;
	std::vector<Point2f> m_TransformedVisionCone;

	bool m_GoingToEnd{true};

	bool m_TurnedOnce{};
};

