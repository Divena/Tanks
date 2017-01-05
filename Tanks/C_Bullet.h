#pragma once

#include <hge.h>
#include <hgesprite.h>
#include <hgevector.h>
#include <hgeanim.h>

class C_Bullet
{
public:
	C_Bullet(hgeVector Position, hgeVector Velocity, HTEXTURE &Texture, float angle, int number);
	~C_Bullet();

	void Update();
	void Render();
	hgeRect     GetBoundingBox() { return BoundingBox; };
	hgeVector   GetPosition() { return Position; };
	int         GetNumber() { return Number; };

private:
	static HGE*     hge;
	hgeVector       Position;
	hgeVector       Velocity;
	hgeAnimation*      Sprite;
	hgeRect         BoundingBox;
	float           Angle;
	int             Number;
};

