#pragma once

#include "C_Bullet.h"

HGE* C_Bullet::hge = 0;

C_Bullet::C_Bullet(hgeVector position, hgeVector velocity, HTEXTURE &Texture, float angle, int number) : Position(position), Velocity(velocity), Angle(angle), Number(number)
{
	hge = hgeCreate(HGE_VERSION);
	Sprite = new hgeAnimation(Texture, 1, 1, 0, 0, 6, 3);
	Sprite->SetHotSpot(3, 1.5);
	Sprite->Play();
}


C_Bullet::~C_Bullet()
{
	delete Sprite;

	hge->Release();
}

void C_Bullet::Update()
{

	Position.x += Velocity.x;
	Position.y += Velocity.y;

	Sprite->GetBoundingBox(Position.x, Position.y, &BoundingBox);
}

void C_Bullet::Render()
{
	Sprite->RenderEx(Position.x, Position.y, Angle);
}


