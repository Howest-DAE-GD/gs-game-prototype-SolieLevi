#pragma once
#include "BaseGame.h"
#include "player.h"
#include "texture.h"
#include "Enemy.h"
#include "SVGParser.h"
#include "Keycard.h"
#include <vector>
#include "Coins.h"
#include "Door.h"
#include "PowerUp.h"

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:

	float scale{ 0.6f };
	float offsetX{ 300 * scale };
	float offsetY{ 180 * scale };

	float m_Timer{};

	Player m_Player{ Point2f{10.f,20.f},10.f };
	std::vector<Enemy> m_Enemies;
	std::vector<Coins> m_Coins;
	Keycard m_Keycard{ rand() % 5 };
	Door m_Door{};
	PowerUp m_PowerUp{ rand() % 3};

	Texture m_LossText{ "GAME OVER","VCR_OSD_MONO_1.001.ttf",20,Color4f{1.f,0.f,0.f,1.f}};
	Texture m_WinText{ "YOU WON!!!","VCR_OSD_MONO_1.001.ttf",20,Color4f{0.f,1.f,0.f,1.f} };

	Texture m_ResetText{ "R to Reset","VCR_OSD_MONO_1.001.ttf",20,Color4f{1.f,1.f,1.f,1.f} };


	SVGParser map{};
	std::vector<std::vector<Point2f>> m_Border;
	float m_TextTimer{ 0.f };

	bool win{false};
	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void Reset();

	void Inputs(const Uint8* pStates);

	Color4f GetColor(float timer) const;
};