#include "C_Enemy.h"

HGE* C_Enemy::hge = 0;
const int size_sprite = 32;
C_Enemy::C_Enemy(HTEXTURE image, int number, bool game)
{
	Game = game;
	Position = Random_Position();
	hge = hgeCreate(HGE_VERSION);
	Texture = image;
	Sprite = new hgeAnimation(Texture, 4, 4, 0, 0, 31, 28);
	Sprite->SetHotSpot(15.5, 14);
	Sprite->Play();
	Angle = 0;
	Speed = 0.5;
	Number = number;
}

C_Enemy::~C_Enemy()
{
	hge->Release();
}


hgeVector C_Enemy::Random_Position()
{
	hgeVector tm;
	if (Game) {
		bool NO = true;
		while (NO) {
			NO = false;
			HGE* hge = hgeCreate(HGE_VERSION);
			tm.x = (float)hge->Random_Int(size_sprite, size_sprite * 25);
			tm.y = (float)hge->Random_Int(size_sprite, size_sprite * 20);
			tm.x -= (int)tm.x % size_sprite;
			tm.y -= (int)tm.y % size_sprite;
			if (tm.x > (2 * size_sprite) && tm.x < (23 * size_sprite) && tm.y >(2 * size_sprite) && tm.y < (18 * size_sprite))
				NO = true;
		}
	}
	else {
		bool NO = true;
		while (NO) {
			NO = false;
			HGE* hge = hgeCreate(HGE_VERSION);
			tm.x = (float)hge->Random_Int(size_sprite, size_sprite * 25);
			tm.y = (float)hge->Random_Int(size_sprite, size_sprite * 20);
			tm.x -= (int)tm.x % size_sprite;
			tm.y -= (int)tm.y % size_sprite;
			if ((tm.x < (7 * size_sprite) && tm.y < (7 * size_sprite)) || (tm.x >(18 * size_sprite) && tm.y >(13 * size_sprite)))
				NO = true;
		}
	}
	return tm;
}

void C_Enemy::Update(float delta, hgeVector position)
{
	Position_back = Position;
	if ((Position.x - position.x) > 0)
	{
		Position.x -= Speed;
		Angle = M_PI;

	}
	else if ((Position.x - position.x) < 0)
	{
		Position.x += Speed;
		Angle = 0;

	}
	else if ((Position.y - position.y) > 0)
	{
		Position.y -= Speed;
		Angle = -M_PI_2;
	}
	else if ((Position.y - position.y) < 0)
	{
		Position.y += Speed;
		Angle = M_PI_2;
	}

	Sprite->Update(delta);
	Sprite->GetBoundingBox(Position.x, Position.y, &BoundingBox);
}