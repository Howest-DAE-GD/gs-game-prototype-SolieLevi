#pragma once
#include "utils.h"
#include "texture.h"

class Door;

class Keycard
{
public:
	Keycard(int nr);

	void Update(float elapsedSec, Point2f playerCenter);
	void Draw()const;

	float GetDistanceFromPlayer(Point2f playerCenter);
	void SetPickedUp(bool pickedUp);
	void SetUsed(bool used);

	void Calibrate(float scale, float offsetX, float offsetY);
	void Use(Door& door);

	void Reset();
private:
	const Color4f m_Color{ 0.f,1.f,0.f,1.f };
	Rectf m_Bounds;

	bool m_IsPickedUp;
	bool m_Used;
	const float m_Speed{ 150.f * 0.8f };
	Rectf m_Target;
	Point2f m_TargetCenter;

	Texture m_Texture{ "keycard.png" };

	int m_Nr;
};

