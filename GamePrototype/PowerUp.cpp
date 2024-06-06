#include "pch.h"
#include "PowerUp.h"
#include "Enemy.h"
#include "utils.h"
#include "Player.h"
#include <vector>


PowerUp::PowerUp(int Nr)
	:m_Bounds{0.f,0.f,20.f,20.f}
	,m_PickedUp{false}
{
	switch (Nr)
	{
	case 0:
		m_Bounds.left = 888;
		m_Bounds.bottom = 1080 - 425;
		break;
	case 1:
		m_Bounds.left = 1154;
		m_Bounds.bottom = 1080 - 572;
		break;
	case 2:
		m_Bounds.left = 490;
		m_Bounds.bottom = 1080 - 790;
		break;
	}
}

void PowerUp::Draw() const
{
	if (!m_PickedUp)
	{
		m_Texture.Draw(m_Bounds);
	}
}

void PowerUp::PickUp(const Player& player, std::vector<Enemy>& enemies)
{
	float distance = sqrtf((m_Bounds.left - player.GetBounds().center.x)* (m_Bounds.left - player.GetBounds().center.x) + (m_Bounds.bottom - player.GetBounds().center.y) * (m_Bounds.bottom - player.GetBounds().center.y));
	if (distance > -20 && distance < 20)
	{
		m_PickedUp = true;
		Activate(enemies);
	}
}

void PowerUp::Activate(std::vector<Enemy>& enemies)
{
	for (int idx{}; idx < enemies.size(); ++idx)
	{
		enemies[idx].SetTimer(7.5f);
	}
}

void PowerUp::Reset()
{
	switch (rand() % 3)
	{
	case 0:
		m_Bounds.left = 888;
		m_Bounds.bottom = 1080 - 425;
		break;
	case 1:
		m_Bounds.left = 1154;
		m_Bounds.bottom = 1080 - 572;
		break;
	case 2:
		m_Bounds.left = 490;
		m_Bounds.bottom = 1080 - 790;
		break;
	}
	m_PickedUp = false;
}

void PowerUp::Calibrate(float scale, float offsetX, float offsetY)
{
	m_Bounds.left = m_Bounds.left * scale - offsetX;
	m_Bounds.bottom = m_Bounds.bottom * scale - offsetY - 15;
}
