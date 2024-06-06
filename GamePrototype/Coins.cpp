#include "pch.h"
#include "Coins.h"
#include "utils.h"

Coins::Coins(const Point2f& pos)
	:m_Center{pos}
{
}

void Coins::Draw() const
{
	if(m_IsActive)
	{
		utils::SetColor(m_Color);
		utils::FillEllipse(m_Center, size, size);
	}
}

void Coins::CheckPickedUp(Player& player)
{
	if (utils::IsOverlapping(player.GetBounds(), Circlef{ m_Center,size }) && m_IsActive)
	{
		m_IsActive = false;
		++player.m_CoinCount;
	}
}

void Coins::Calibrate(float scale, float offsetX, float offsetY)
{
	m_Center.x = m_Center.x * scale - offsetX;
	m_Center.y = m_Center.y * scale - offsetY - 15;
}

void Coins::Reset()
{
	m_IsActive = true;
}
