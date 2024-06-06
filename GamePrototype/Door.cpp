#include "pch.h"
#include "Door.h"
#include "utils.h"

Door::Door()
	: m_Bounds{ 1597.f,1080 - 181.f,28.f,59.f }
	, m_IsOpen{false}
{
}

Rectf Door::GetPos()
{
	return m_Bounds;
}

void Door::Open()
{
	m_IsOpen = true;
}

void Door::Draw() const
{
	if (!m_IsOpen)
	{
		utils::SetColor(m_Color);
		utils::FillRect(m_Bounds);
	}
}

void Door::Reset()
{
	m_IsOpen = false;
}

void Door::Calibrate(float scale, float offsetX, float offsetY)
{
	m_Bounds.left = m_Bounds.left * scale - offsetX + 3;
	m_Bounds.bottom = m_Bounds.bottom * scale - offsetY - 35;
	m_Bounds.height *= scale;
	m_Bounds.width *= scale;
}

bool Door::GetOpen()
{
	return m_IsOpen;
}
