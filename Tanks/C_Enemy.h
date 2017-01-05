#pragma once
#include "C_Player.h"
#include <math.h>


class C_Enemy : public C_Player
{
public:
	C_Enemy(HTEXTURE image, int number, bool game);
	~C_Enemy();
	virtual void Update(float delta, hgeVector position);
	hgeVector Random_Position();
private:
	static HGE*     hge;
};

