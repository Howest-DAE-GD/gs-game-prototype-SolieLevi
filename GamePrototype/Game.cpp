#include "pch.h"
#include "Game.h"
#include <iostream>
#include <string>


Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	map.GetVerticesFromSvgFile("level.svg", m_Border);
	for (int idx{}; idx < m_Border.size(); ++idx)
	{
		for (int j{}; j < m_Border[idx].size(); ++j)
		{
			m_Border[idx][j].x = m_Border[idx][j].x * scale - offsetX;
			m_Border[idx][j].y = m_Border[idx][j].y * scale - offsetY;
		}
	}
	m_Enemies.push_back({Point2f{360,1080 - 723},Point2f{360 + 413,1080 - 723},180,0 });
	m_Enemies.push_back({ Point2f{764,1080 - 589 - 283},Point2f{764,1080 - 589},270,90 });
	m_Enemies.push_back({ Point2f{354,1080 - 643},Point2f{354 + 941,1080 - 643},180,0 });
	m_Enemies.push_back({ Point2f{622,1080 - 422},Point2f{622 + 296,1080 - 422},180,0 });
	m_Enemies.push_back({ Point2f{1152,1080 - 726},Point2f{1152 + 426,1080 - 726},180,0 });
	m_Enemies.push_back({ Point2f{351,1080 - 204},Point2f{351 + 302,1080 - 204},180,0 });
	m_Enemies.push_back({ Point2f{1146,1080 - 277},Point2f{1146 + 428,1080 - 277},180,0 });
	m_Enemies.push_back({ Point2f{1024,1080 - 568 - 236},Point2f{1024,1080 - 568},270,90 });
	
	m_Coins.push_back(Coins{ Point2f{363, 1080 - 283} });
	m_Coins.push_back(Coins{ Point2f{1162, 1080 - 345} });
	m_Coins.push_back(Coins{ Point2f{1530, 1080 - 348} });
	m_Coins.push_back(Coins{ Point2f{1260, 1080 - 200} });
	m_Coins.push_back(Coins{ Point2f{1545, 1080 - 852} });
	m_Coins.push_back(Coins{ Point2f{1531, 1080 - 569} });
	m_Coins.push_back(Coins{ Point2f{880, 1080 - 717} });
	m_Coins.push_back(Coins{ Point2f{360, 1080 - 200} });


	for (int idx{}; idx < m_Enemies.size(); ++idx)
	{
		m_Enemies[idx].Calibrate(scale, offsetX, offsetY);
	}
	for (int idx{}; idx < m_Coins.size(); ++idx)
	{
		m_Coins[idx].Calibrate(scale, offsetX, offsetY);
	}
	m_Keycard.Calibrate(scale, offsetX, offsetY);
	m_Door.Calibrate(scale, offsetX, offsetY);
	m_PowerUp.Calibrate(scale, offsetX, offsetY);
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	Inputs(pStates);
	
	if(win == false && m_Paused == false){
		m_Timer += elapsedSec;
		if (m_Player.GetAlive()) {
			for (int idx{}; idx < m_Enemies.size(); ++idx)
			{
				m_Enemies[idx].Update(elapsedSec);
				m_Enemies[idx].CheckSeen(m_Player);
				m_Enemies[idx].SetSpeed(m_SpeedIncrease);
			}
			m_Player.Update(elapsedSec, m_Border, m_Door);
				m_Keycard.Update(elapsedSec, m_Player.GetPos());
		}
		//m_Timer += elapsedSec;
		m_TextTimer += elapsedSec;
		if (m_TextTimer >= 0.6f)
		{
			m_TextTimer = 0.f;
		}
		for (int idx{}; idx < m_Coins.size(); ++idx)
		{
			m_Coins[idx].CheckPickedUp(m_Player);
		}
		m_PowerUp.PickUp(m_Player, m_Enemies);
	}

	if (m_Player.GetPos().x > 1618 * scale - offsetX)
	{
		win = true;
	}


}

void Game::Draw() const
{
	ClearBackground();
	
	Texture timerText{ std::to_string(int(m_Timer) / 60) + " : " + std::to_string(int(m_Timer) % 60) + " : " + std::to_string(int((m_Timer - int(m_Timer) % 60) * 1000)) ,"VCR_OSD_MONO_1.001.ttf",20,Color4f{1.f,1.f,1.f,1.f} };
	Texture coinText{ std::to_string(m_Player.GetCoinCount()) ,"VCR_OSD_MONO_1.001.ttf",20,Color4f{1.f,1.f,1.f,1.f} };

	Texture powerUpTimer{ "POWER UP : " + std::to_string(int(m_Enemies[0].GetTimer())) ,"VCR_OSD_MONO_1.001.ttf",20,GetColor(m_Enemies[0].GetTimer()) };

	if (m_Player.GetAlive() == false || win == true)
	{
		utils::SetColor(Color4f{ 0.2f,0.2f,0.2,0.8f });
		utils::FillRect(Point2f{ 0.f,0.f }, GetViewPort().width, GetViewPort().height);
	}

	m_Player.Draw();
	m_Keycard.Draw();
	coinText.Draw(Point2f{ 50.f,GetViewPort().height - 50.f });
	m_PowerUp.Draw();

	if (m_Enemies[0].GetTimer() > 0)powerUpTimer.Draw(Point2f{ 200.f,GetViewPort().height - 50.f });

	Coins visualCoin{ Point2f{50.f + coinText.GetWidth() + 5.f,GetViewPort().height - 50.f + coinText.GetHeight() / 2} };
	visualCoin.Draw();
	m_Door.Draw();

	for (int idx{}; idx < m_Enemies.size(); ++idx)
	{
		m_Enemies[idx].Draw();
	}
	for (int idx{}; idx < m_Coins.size(); ++idx)
	{
		m_Coins[idx].Draw();
	}
	if (m_Player.GetAlive() == false)
	{
		m_LossText.Draw(Rectf{ GetViewPort().width * 0.4f,GetViewPort().height * 0.5f,GetViewPort().width * 0.2f,GetViewPort().height * 0.1f });
		if (m_TextTimer <= 0.3f)
		{
			m_ResetText.Draw(Rectf{ GetViewPort().width * 0.45f,GetViewPort().height * 0.4f,GetViewPort().width * 0.1f,GetViewPort().height * 0.05f });
		}
	}
	if (win)
	{
		m_WinText.Draw(Rectf{ GetViewPort().width * 0.4f,GetViewPort().height * 0.5f,GetViewPort().width * 0.2f,GetViewPort().height * 0.1f });
	}
	utils::SetColor(Color4f{ 1.f,1.f,1.f,1.f });
	for (int idx{}; idx < m_Border.size(); ++idx)
	{
		for (int j{}; j < m_Border[idx].size() - 1; ++j)
		{
			utils::DrawLine(m_Border[idx][j], m_Border[idx][j + 1]);
		}
	}
	timerText.Draw(Rectf{ GetViewPort().width * 0.45f,GetViewPort().height * 0.9f,GetViewPort().width * 0.12f,GetViewPort().height * 0.05f });


	if (m_Paused)
	{
		utils::SetColor(Color4f{ 0.2f,0.2f,0.2,0.8f });
		utils::FillRect(Point2f{ 0.f,0.f }, GetViewPort().width, GetViewPort().height);

		
		utils::SetColor(Color4f{ 0.5f,0.5f,0.5f,1.f });
		utils::FillRect(GetViewPort().width * 0.4f, GetViewPort().height * 0.2f, GetViewPort().width * 0.2f, GetViewPort().height * 0.6f);
		
		utils::SetColor(Color4f{ 1.f,1.f,1.f,1.f });

		Texture paused{ "PAUSED" ,"VCR_OSD_MONO_1.001.ttf",20,Color4f{1.f,1.f,1.f,1.f} };
		paused.Draw(Rectf{GetViewPort().width * 0.45f, GetViewPort().height * 0.7f, GetViewPort().width * 0.1f, GetViewPort().height * 0.05f});

		Texture speedMinus{ "-" ,"VCR_OSD_MONO_1.001.ttf",20,Color4f{1.f,1.f,1.f,1.f} };
		speedMinus.Draw(Rectf{ GetViewPort().width * 0.45f, GetViewPort().height * 0.5f, GetViewPort().width * 0.025f, GetViewPort().height * 0.05f });
		
		Texture speedIncrease{ std::to_string(m_SpeedIncrease), "VCR_OSD_MONO_1.001.ttf", 20, Color4f{ 1.f,1.f,1.f,1.f } };
		speedIncrease.Draw(Rectf{ GetViewPort().width * 0.475f, GetViewPort().height * 0.5f, GetViewPort().width * 0.05f, GetViewPort().height * 0.05f });

		Texture speedPlus{ "+" ,"VCR_OSD_MONO_1.001.ttf",20,Color4f{1.f,1.f,1.f,1.f} };
		speedPlus.Draw(Rectf{ GetViewPort().width * 0.525f, GetViewPort().height * 0.5f, GetViewPort().width * 0.025f, GetViewPort().height * 0.05f });

		utils::DrawRect(GetViewPort().width * 0.45f, GetViewPort().height * 0.5f, GetViewPort().width * 0.025f, GetViewPort().height * 0.05f);
		utils::DrawRect(GetViewPort().width * 0.475f, GetViewPort().height * 0.5f, GetViewPort().width * 0.05f, GetViewPort().height * 0.05f);
		utils::DrawRect(GetViewPort().width * 0.525f, GetViewPort().height * 0.5f, GetViewPort().width * 0.025f, GetViewPort().height * 0.05f);
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//float distance = sqrtf(m_Keycard.GetDistanceFromPlayer(m_Player.GetPos()) * m_Keycard.GetDistanceFromPlayer(m_Player.GetPos()));
	switch (e.keysym.sym)
	{
	/*case SDLK_a:
	case SDLK_LEFT:
		m_Player.SetDirection(Player::Direction::left);
		break;
	case SDLK_d:
	case SDLK_RIGHT:
		m_Player.SetDirection(Player::Direction::right);
		break;
	case SDLK_w:
	case SDLK_UP:
		m_Player.SetDirection(Player::Direction::up);
		break;
	case SDLK_s:
	case SDLK_DOWN:
		m_Player.SetDirection(Player::Direction::down);
		break;
	case SDLK_f:
		if(distance < 20)m_Keycard.SetPickedUp(true);
		m_Keycard.Use(m_Door);
		break;
	case SDLK_r:
		Reset();
		break;
	default:
		m_Player.SetDirection(Player::Direction::neutral);
		break;*/
	case SDLK_ESCAPE:
		m_Paused = !m_Paused;
		break;
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_a:
	//case SDLK_LEFT:
	//	if(m_Player.GetDirection() == Player::Direction::left) 
	//		m_Player.SetDirection(Player::Direction::neutral);
	//	break;
	//case SDLK_d:
	//case SDLK_RIGHT:
	//	if (m_Player.GetDirection() == Player::Direction::right)
	//		m_Player.SetDirection(Player::Direction::neutral);
	//	break;
	//case SDLK_w:
	//case SDLK_UP:
	//	if (m_Player.GetDirection() == Player::Direction::up)
	//		m_Player.SetDirection(Player::Direction::neutral);
	//	break;
	//case SDLK_s:
	//case SDLK_DOWN:
	//	if (m_Player.GetDirection() == Player::Direction::down)
	//		m_Player.SetDirection(Player::Direction::neutral);
	//	break;
	//default:
	//	//m_Player.SetDirection(Player::Direction::neutral);
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	Point2f mouse{ float(e.x),float(e.y) };
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		if (m_Paused)
		{
			if (utils::IsPointInRect(mouse, Rectf{ GetViewPort().width * 0.45f, GetViewPort().height * 0.5f, GetViewPort().width * 0.025f, GetViewPort().height * 0.05f }) && m_SpeedIncrease > 0)
			{
				--m_SpeedIncrease;
			}
			else if (utils::IsPointInRect(mouse, Rectf{ GetViewPort().width * 0.525f, GetViewPort().height * 0.5f, GetViewPort().width * 0.025f, GetViewPort().height * 0.05f }) && m_SpeedIncrease < 9)
			{
				++m_SpeedIncrease;
			}
		}
		break;
	}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.11f, 0.1f, 0.1f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::Reset()
{
	for (int idx{}; idx < m_Enemies.size(); ++idx)
	{
		m_Enemies[idx].Reset();
	}

	for (int idx{}; idx < m_Coins.size(); ++idx)
	{
		m_Coins[idx].Reset();
	}

	m_Player.Reset(Point2f{ 10.f, 20.f });
	m_Timer = 0.f;

	m_Player.m_CoinCount = 0.f;

	m_Keycard.Reset();
	m_Keycard.Calibrate(scale, offsetX, offsetY);

	m_Door.Reset();

	m_PowerUp.Reset();
	m_PowerUp.Calibrate(scale, offsetX, offsetY);

	m_SpeedIncrease = 5;

	win = false;
}

Color4f Game::GetColor(float timer) const
{
	if (timer > 5) return Color4f{1.f,1.f,1.f,1.f};
	else if (timer > 4.5f) return Color4f{ 1.f,0.f,0.f,1.f };
	else if (timer > 4.f) return Color4f{ 1.f,1.f,1.f,1.f };
	else if (timer > 3.5f) return Color4f{ 1.f,0.f,0.f,1.f };
	else if (timer > 3.f) return Color4f{ 1.f,1.f,1.f,1.f };
	else if (timer > 2.5f) return Color4f{ 1.f,0.f,0.f,1.f };
	else if (timer > 2.f) return Color4f{ 1.f,1.f,1.f,1.f };
	else if (timer > 1.5f) return Color4f{ 1.f,0.f,0.f,1.f };
	else if (timer > 1.f) return Color4f{ 1.f,1.f,1.f,1.f };
	else if (timer > 0.5f) return Color4f{ 1.f,0.f,0.f,1.f };
	else return Color4f{ 1.f,1.f,1.f,1.f };
}

void Game::Inputs(const Uint8* pStates)
{
	m_Player.SetDirection(Player::Direction::neutral);

	float distance = sqrtf(m_Keycard.GetDistanceFromPlayer(m_Player.GetPos()) * m_Keycard.GetDistanceFromPlayer(m_Player.GetPos()));
	bool left{ false }, right{ false }, up{ false }, down{ false };
	if (pStates[SDL_SCANCODE_A] || pStates[SDL_SCANCODE_LEFT])
	{
		m_Player.SetDirection(Player::Direction::left);
		left = true;
	}
	if (pStates[SDL_SCANCODE_D] || pStates[SDL_SCANCODE_RIGHT])
	{
		m_Player.SetDirection(Player::Direction::right);
		right = true;
	}
	if (pStates[SDL_SCANCODE_W] || pStates[SDL_SCANCODE_UP])
	{
		m_Player.SetDirection(Player::Direction::up);
		up = true;
	}
	if (pStates[SDL_SCANCODE_S] || pStates[SDL_SCANCODE_DOWN])
	{
		m_Player.SetDirection(Player::Direction::down);
		down = true;
	}
	if (pStates[SDL_SCANCODE_F])
	{
		if (distance < 20)m_Keycard.SetPickedUp(true);
		m_Keycard.Use(m_Door);
	}
	if (pStates[SDL_SCANCODE_R])
	{
		Reset();
	}
	if (up && left)
	{
		m_Player.SetDirection(Player::Direction::diagonalUpL);
	}
	if (up && right)
	{
		m_Player.SetDirection(Player::Direction::diagonalUpR);
	}
	if (down && left)
	{
		m_Player.SetDirection(Player::Direction::diagonalDownL);
	}
	if (down && right)
	{
		m_Player.SetDirection(Player::Direction::diagonalDownR);
	}
}
