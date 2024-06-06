#pragma once
class Door
{
public:
	Door();

	Rectf GetPos();

	void Open();
	void Draw() const;
	
	void Reset();

	void Calibrate(float scale, float offsetX, float offsetY);

	bool GetOpen();
private:
	const Color4f m_Color{ 1.f,0.f,0.f,1.f };
	Rectf m_Bounds;

	bool m_IsOpen;

};

