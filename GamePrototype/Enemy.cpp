#include "pch.h"
#include "Enemy.h"
#include "Player.h"
#include "Matrix2x3.h"
#include <iostream>

Enemy::Enemy(const Point2f& center, const Point2f& end, int maxDegree, int minDegree)
	:m_Center{center}
	,m_Angle{float(minDegree)}
	,m_Size{20.f}
	,m_Bounds{center.x ,center.y ,20.f,20.f}
	,m_ConeColor{1.f,0.f,0.f,1.f}
	,m_MaxDegree{maxDegree}
	,m_MinDegree{minDegree}
	,m_EnemyColor{1.f,1.f,1.f,1.f}
	, m_Turn{ true }
	, m_End{ end }
	, m_PowerUpTimer{0.f}

{
	m_VisionCone.push_back(Point2f{0.f,5.f});
	m_VisionCone.push_back(Point2f{ 40.f,20.f });
	m_VisionCone.push_back(Point2f{ 40.f,-20.f });
	m_VisionCone.push_back(Point2f{ 0.f,-5.f });


	if (center.x - end.x == 0)
	{
		m_Velocity.y = 140.f;
	}
	else
	{
		m_Velocity.x = 140.f;
	}
	m_OriginalVelocity = m_Velocity;
}

void Enemy::Update(float elapsedSec)
{
	m_PowerUpTimer -= elapsedSec;
	//matrix gebruike voor rotaties
	if(m_Turn == false)
	{
		m_Center += m_Velocity * elapsedSec;
		m_Bounds.bottom += m_Velocity.y * elapsedSec;
		m_Bounds.left += m_Velocity.x * elapsedSec;
		Matrix2x3 rotation{};
		rotation.SetAsRotate(m_Angle);
		Matrix2x3 translate{};
		translate.SetAsTranslate(Vector2f{ m_Center.x + m_Size / 2,m_Center.y + m_Size / 2 });
		Matrix2x3 TR{ translate * rotation };
		m_TransformedVisionCone = TR.Transform(m_VisionCone);
		if (m_Velocity.x == 0)
		{
			if (m_Center.y >= m_End.y && m_GoingToEnd)
			{
				m_Turn = true;
				m_Velocity = -m_Velocity;
				m_GoingToEnd = false;
			}
			else if (m_Center.y <= m_Start.y && m_GoingToEnd == false)
			{
				m_Turn = true;
				m_Velocity = -m_Velocity;
				m_GoingToEnd = true;
			}
		}
		else
		{
			if (m_Center.x >= m_End.x && m_GoingToEnd)
			{
				m_Turn = true;
				m_Velocity = -m_Velocity;
				m_GoingToEnd = false;
			}
			else if (m_Center.x <= m_Start.x && m_GoingToEnd == false)
			{
				m_Turn = true;
				m_Velocity = -m_Velocity;
				m_GoingToEnd = true;
			}
		}
	}
	else
	{
		turn(elapsedSec);
		Matrix2x3 rotation{};
		rotation.SetAsRotate(m_Angle);
		Matrix2x3 translate{};
		translate.SetAsTranslate(Vector2f{ m_Center.x + m_Size/2,m_Center.y + m_Size / 2 });
		Matrix2x3 TR{ translate * rotation };
		m_TransformedVisionCone = TR.Transform(m_VisionCone);
	}
	m_Timer -= elapsedSec;
}

void Enemy::Draw() const
{
	if (m_PowerUpTimer > 0)utils::SetColor(m_DisabledCone);
	else utils::SetColor(m_ConeColor);
	utils::FillPolygon(m_TransformedVisionCone);

	if (m_PowerUpTimer < 0)
	{
		utils::SetColor(m_EnemyColor);
	}
	else utils::SetColor(m_DisabledColor);
	utils::FillRect(m_Bounds);
}

void Enemy::turn(float elapsedSec)
{
	if(m_TurnedOnce == false) m_currentIncrement = m_AngleIncrement;
	if (m_Timer < 0.f)
	{
		if (m_Angle > m_MaxDegree)
		{
			m_currentIncrement = -m_AngleIncrement;
			m_Turn = false;
			m_TurnedOnce = true;
			m_Angle = float(m_MaxDegree);
			m_Timer = 1.f;
		}
		if (m_Angle < m_MinDegree)
		{
			m_currentIncrement = m_AngleIncrement;
			m_Turn = false;
			m_TurnedOnce = true;
			m_Angle = float(m_MinDegree);
			m_Timer = 1.f;
		}
		m_Angle += m_currentIncrement * elapsedSec;
	}
	else
	{
		m_Turn = false;
	}
}

void Enemy::SetTimer(float timer)
{
	m_PowerUpTimer = timer;
}

float Enemy::GetTimer() const
{
	return m_PowerUpTimer;
}

void Enemy::CheckSeen(Player& player)
{
	if(m_PowerUpTimer < 0){
		if (utils::IsOverlapping(m_TransformedVisionCone, player.GetBounds()))
		{
			player.SetAlive(false);
		}
	}
}

void Enemy::Calibrate(float scale, float offsetX, float offsetY)
{
	m_Center.x = m_Center.x * scale - offsetX;
	m_Center.y = m_Center.y * scale - offsetY - 15;

	m_End.x = m_End.x * scale - offsetX;
	m_End.y = m_End.y * scale - offsetY - 15;

	m_Start = m_Center;

	m_Bounds = Rectf{ m_Center.x,m_Center.y,20.f,20.f };
}

void Enemy::Reset()
{
	m_Center = m_Start;
	m_Angle = float(m_MinDegree);
	m_Bounds = Rectf{m_Start.x,m_Start.y,20.f,20.f};
	m_TurnedOnce = false;
	m_Turn = false;
	m_GoingToEnd = true;
	m_Velocity = m_OriginalVelocity;
	m_Timer = 1.f;
	m_PowerUpTimer = 0.f;
}
