#include "C_Player.h"

HGE* C_Player::hge = 0;
const int size_sprite = 32;

C_Player::C_Player(hgeVector position, HTEXTURE image, int number, bool button) :Game(button), Start(position)
{
	Position = position;
	hge = hgeCreate(HGE_VERSION);
	Texture = image;
	Sprite = new hgeAnimation(Texture, 4, 4, 0, 0, 30, 28);
	Sprite->SetHotSpot(15, 14);
	Sprite->Play();
	Angle = 0;
	Speed = 1.0;
	Number = -1;
	Number = number;

}


C_Player::~C_Player()
{
	delete Sprite;
	hge->Release();
}

void C_Player::Update(float delta)
{
	Position_back = Position;
	if (Game) {
		if (hge->Input_GetKeyState(HGEK_LEFT) && Position.x > 16)
		{
			Position.x -= Speed;
			Angle = M_PI;

		};
		if (hge->Input_GetKeyState(HGEK_RIGHT) && Position.x < (size_sprite * 25 - size_sprite / 2))
		{
			Position.x += Speed;
			Angle = 0;
		};
		if (hge->Input_GetKeyState(HGEK_UP) && Position.y > size_sprite / 2)
		{
			Position.y -= Speed;
			Angle = -M_PI_2;

		};
		if (hge->Input_GetKeyState(HGEK_DOWN) && Position.y < size_sprite * 20 - size_sprite / 2)
		{
			Position.y += Speed;
			Angle = M_PI_2;
		};
	}
	else {
		if (hge->Input_GetKeyState(HGEK_A) && Position.x > 16)
		{
			Position.x -= Speed;
			Angle = M_PI;

		};
		if (hge->Input_GetKeyState(HGEK_D) && Position.x < (size_sprite * 25 - size_sprite / 2))
		{
			Position.x += Speed;
			Angle = 0;
		};
		if (hge->Input_GetKeyState(HGEK_W) && Position.y > size_sprite / 2)
		{
			Position.y -= Speed;
			Angle = -M_PI_2;

		};
		if (hge->Input_GetKeyState(HGEK_S) && Position.y < size_sprite * 20 - size_sprite / 2)
		{
			Position.y += Speed;
			Angle = M_PI_2;
		};
	}

	Sprite->Update(delta);
	Sprite->GetBoundingBox(Position.x, Position.y, &BoundingBox);
}

void C_Player::Render()
{
	Sprite->RenderEx(Position.x, Position.y, Angle);
}