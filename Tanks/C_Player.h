#pragma once

#include <hge.h>
#include <hgesprite.h>
#include <hgevector.h>
#include <hgeanim.h>

class C_Player
{
public:
	C_Player() {};
	C_Player(hgeVector position, HTEXTURE image, int number, bool button);
	~C_Player();

	virtual void Update(float delta);
	void Render();

	hgeRect     GetBoundingBox() { return BoundingBox; };
	hgeVector   GetPosition() { return Position; };
	float       GetAngle() { return Angle; };
	void        SetPosition(hgeVector vec) { Position = vec; };
	float       GetSpeed() { return Speed; };
	void        SetSpeed(float speed) { Speed = speed; };
	hgeVector   GetPosition_back() { return Position_back; };
	int         GetNumber() { return Number; };
	void        SetCounter() { Counter++; };
	int         GetCounter() { return Counter; };
	hgeVector   GetStart() { return Start; };
protected:
	hgeVector       Position;
	hgeVector       Position_back;
	HTEXTURE        Texture;
	hgeAnimation*   Sprite;
	hgeRect         BoundingBox;
	float           Speed;
	float           Angle;
	int             Number;
	bool            Game;
private:
	static HGE*     hge;
	hgeVector       Start;
	int             Counter;

};

