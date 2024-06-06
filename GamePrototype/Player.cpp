#include "pch.h"
#include "Player.h"
#include "utils.h"
#include "Door.h"
#include <iostream>

Player::Player(const Point2f& center, float size)
	:m_Bounds{center.x-size/2,center.y-size/2,size,size}
	,m_CirclePlayer{center,size}
	,m_Color{0.f,0.7f,0.2f,1.f}
	,m_DeadColor{1.f,0.f,0.f,1.f}
	,m_IsAlive{true}
	,m_Direction{Direction::neutral}
{
}

void Player::SetDirection(const Direction& direction)
{
	m_Direction = direction;
}

void Player::CheckCollision(float elapsedSec, const std::vector<std::vector<Point2f>>& map, Door door)
{
	Point2f p1{ m_CirclePlayer.center.x + m_CirclePlayer.radius + 2,m_CirclePlayer.center.y + 2 };
	Point2f p2{ m_CirclePlayer.center.x + m_CirclePlayer.radius - 2,m_CirclePlayer.center.y - 2 };

	Point2f p3{ m_CirclePlayer.center.x - m_CirclePlayer.radius + 2,m_CirclePlayer.center.y + 2 };
	Point2f p4{ m_CirclePlayer.center.x - m_CirclePlayer.radius - 2,m_CirclePlayer.center.y - 2 };

	Point2f p5{ m_CirclePlayer.center.x + 2,m_CirclePlayer.center.y + m_CirclePlayer.radius + 2 };
	Point2f p6{ m_CirclePlayer.center.x - 2,m_CirclePlayer.center.y + m_CirclePlayer.radius - 2 };

	Point2f p7{ m_CirclePlayer.center.x + 2,m_CirclePlayer.center.y - m_CirclePlayer.radius + 2 };
	Point2f p8{ m_CirclePlayer.center.x - 2,m_CirclePlayer.center.y - m_CirclePlayer.radius - 2 };

	m_CurrentSpeed = m_Speed;
	for (int idx{}; idx < map.size(); ++idx) {

		if (utils::Raycast(map[idx], p1,p2, hit))
		{
			m_CurrentSpeed = 0.f;
			SetPosition(hit.intersectPoint.x - m_CirclePlayer.radius - 3, m_CirclePlayer.center.y);
		}
		else if (utils::Raycast(map[idx], p3,p4 , hit))
		{
			m_CurrentSpeed = 0.f;
			SetPosition(hit.intersectPoint.x + m_CirclePlayer.radius + 3, m_CirclePlayer.center.y);
		}
		else if (utils::Raycast(map[idx], p5,p6, hit))
		{
			m_CurrentSpeed = 0.f;
			SetPosition(m_CirclePlayer.center.x, hit.intersectPoint.y - m_CirclePlayer.radius - 3);
		}
		else if (utils::Raycast(map[idx], p7,p8, hit))
		{
			m_CurrentSpeed = 0.f;
			SetPosition(m_CirclePlayer.center.x, hit.intersectPoint.y + m_CirclePlayer.radius + 3);
		}
	}
	if ( door.GetOpen() == false && utils::IsOverlapping(door.GetPos(), m_CirclePlayer))
	{
		m_CurrentSpeed = 0.f;
		SetPosition(door.GetPos().left - 3 - m_CirclePlayer.radius, m_CirclePlayer.center.y);
	}
}

void Player::SetPosition(float x, float y)
{
	m_CirclePlayer.center = Point2f{ x,y };
}

void Player::Reset(const Point2f& pos)
{
	m_CirclePlayer.center = pos;
	m_Bounds = Rectf{ m_CirclePlayer.center.x - m_CirclePlayer.radius / 2, m_CirclePlayer.center.y - m_CirclePlayer.radius / 2, m_CirclePlayer.radius, m_CirclePlayer.radius };
	m_IsAlive = true;
	m_Direction = Direction::neutral;
}

int Player::GetCoinCount() const
{
	return m_CoinCount;
}

void Player::SetPosition(const Point2f& pos)
{
	SetPosition(pos.x, pos.y);
}

void Player::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& map,Door door)
{
	CheckCollision(elapsedSec, map,door);
	if(m_IsAlive){
		switch (m_Direction)
		{
		case Player::Direction::left:
			m_CirclePlayer.center.x -= m_CurrentSpeed * elapsedSec;
			m_Bounds.left -= m_CurrentSpeed * elapsedSec;
			break;
		case Player::Direction::right:
			m_CirclePlayer.center.x += m_CurrentSpeed * elapsedSec;
			m_Bounds.left += m_CurrentSpeed * elapsedSec;
			break;
		case Player::Direction::up:
			m_CirclePlayer.center.y += m_CurrentSpeed * elapsedSec;
			m_Bounds.bottom += m_CurrentSpeed * elapsedSec;
			break;
		case Player::Direction::down:
			m_CirclePlayer.center.y -= m_CurrentSpeed * elapsedSec;
			m_Bounds.bottom -= m_CurrentSpeed * elapsedSec;
			break;


		case Player::Direction::diagonalUpL:
			m_CirclePlayer.center.x -= m_CurrentSpeed * elapsedSec;
			m_Bounds.left -= m_CurrentSpeed * elapsedSec;

			m_CirclePlayer.center.y += m_CurrentSpeed * elapsedSec;
			m_Bounds.bottom += m_CurrentSpeed * elapsedSec;

			break;
		case Player::Direction::diagonalUpR:
			m_CirclePlayer.center.x += m_CurrentSpeed * elapsedSec;
			m_Bounds.left += m_CurrentSpeed * elapsedSec;

			m_CirclePlayer.center.y += m_CurrentSpeed * elapsedSec;
			m_Bounds.bottom += m_CurrentSpeed * elapsedSec;
			break;
		case Player::Direction::diagonalDownL:
			m_CirclePlayer.center.x -= m_CurrentSpeed * elapsedSec;
			m_Bounds.left -= m_CurrentSpeed * elapsedSec;

			m_CirclePlayer.center.y -= m_CurrentSpeed * elapsedSec;
			m_Bounds.bottom -= m_CurrentSpeed * elapsedSec;
			break;
		case Player::Direction::diagonalDownR:
			m_CirclePlayer.center.x += m_CurrentSpeed * elapsedSec;
			m_Bounds.left += m_CurrentSpeed * elapsedSec;

			m_CirclePlayer.center.y -= m_CurrentSpeed * elapsedSec;
			m_Bounds.bottom -= m_CurrentSpeed * elapsedSec;
			break;
		}
	}
}

void Player::Draw() const
{
	

	if (m_IsAlive)
	{
		utils::SetColor(m_Color);
	}
	else
	{
		utils::SetColor(m_DeadColor);
	}
	utils::FillEllipse(m_CirclePlayer.center, m_CirclePlayer.radius, m_CirclePlayer.radius);
}

Circlef Player::GetBounds() const
{
	return m_CirclePlayer;
}

Point2f Player::GetPos()
{
	return m_CirclePlayer.center;
}

Player::Direction Player::GetDirection()
{
	return m_Direction;
}

bool Player::GetAlive() const
{
	return m_IsAlive;
}

void Player::SetAlive(bool isAlive)
{
	m_IsAlive = isAlive;
}
