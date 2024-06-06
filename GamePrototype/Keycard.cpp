#include "pch.h"
#include "Keycard.h"
#include "Door.h"
#include <iostream>

Keycard::Keycard(int nr)
	: m_Nr{nr}
	, m_Bounds{0.f,0.f,20.f,15.f}
	, m_IsPickedUp{false}
	, m_Target{}
	, m_Used{false}
{
	switch (nr)
	{
	case 0:
		m_Bounds.left = 504.f;
		m_Bounds.bottom = 1080 - 423.f;
		break;
	case 1:
		m_Bounds.left = 744.f;
		m_Bounds.bottom = 1080 - 201.f;
		break;
	case 2:
		m_Bounds.left = 1403.f;
		m_Bounds.bottom = 1080 - 426.f;
		break;
	case 3:
		m_Bounds.left = 1262.f;
		m_Bounds.bottom = 1080 - 350.f;
		break;
	case 4:
		m_Bounds.left = 1274.f;
		m_Bounds.bottom = 1080 - 788.f;
		break;
	}
}

void Keycard::Update(float elapsedSec, Point2f playerCenter)
{

	m_Target = Rectf(playerCenter.x + 20, playerCenter.y + 20,5.f,5.f);
	m_TargetCenter = Point2f{ m_Target.left + m_Target.width / 2,m_Target.bottom + m_Target.height / 2 };

	if(utils::IsOverlapping(m_Target,m_Bounds) == false && m_IsPickedUp == true){
		if (m_TargetCenter.x < m_Bounds.left) m_Bounds.left -= m_Speed * elapsedSec;
		if (m_TargetCenter.x > m_Bounds.left) m_Bounds.left += m_Speed * elapsedSec;

		if (m_TargetCenter.y < m_Bounds.bottom) m_Bounds.bottom -= m_Speed * elapsedSec;
		if (m_TargetCenter.y > m_Bounds.bottom) m_Bounds.bottom += m_Speed * elapsedSec;
	}
}

void Keycard::Draw() const
{
	if(m_Used == false){
		m_Texture.Draw(m_Bounds);
	}
}

float Keycard::GetDistanceFromPlayer(Point2f playerCenter)
{
	return(sqrtf((m_Bounds.left - playerCenter.x) * (m_Bounds.left - playerCenter.x) + (m_Bounds.bottom - playerCenter.y) * (m_Bounds.bottom - playerCenter.y)));
}

void Keycard::SetPickedUp(bool pickedUp)
{
	m_IsPickedUp = pickedUp;
}

void Keycard::SetUsed(bool used)
{
	m_Used = used;
}

void Keycard::Calibrate(float scale, float offsetX, float offsetY)
{
	m_Bounds.left = m_Bounds.left * scale - offsetX;
	m_Bounds.bottom = m_Bounds.bottom * scale - offsetY - 15;
}

void Keycard::Use(Door& door)
{
	float distance{ sqrtf((m_Bounds.left - door.GetPos().left) * (m_Bounds.left - door.GetPos().left) + (m_Bounds.bottom - door.GetPos().bottom) * (m_Bounds.bottom - door.GetPos().bottom)) };
	if (distance <= 20 + m_Bounds.height && distance >= -20 - m_Bounds.height)
	{
		door.Open();
		SetUsed(true);
	}
}

void Keycard::Reset()
{
	m_Nr = rand() % 5;
	switch (m_Nr)
	{
	case 0:
		m_Bounds.left = 504.f;
		m_Bounds.bottom = 1080 - 423.f;
		break;
	case 1:
		m_Bounds.left = 744.f;
		m_Bounds.bottom = 1080 - 201.f;
		break;
	case 2:
		m_Bounds.left = 1403.f;
		m_Bounds.bottom = 1080 - 426.f;
		break;
	case 3:
		m_Bounds.left = 1262.f;
		m_Bounds.bottom = 1080 - 350.f;
		break;
	case 4:
		m_Bounds.left = 1274.f;
		m_Bounds.bottom = 1080 - 788.f;
		break;
	}

	m_Used = false;
	m_IsPickedUp = false;
}
