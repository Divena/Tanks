#pragma once

#include <hge.h>
#include <hgevector.h>
#include <hgesprite.h>
#include <list>
#include <math.h>
#include "C_Block.h"

const int size_sprite = 32;

struct sqare {
	hgeVector Position;
	sqare* Home;
	int F;

	sqare(sqare* home, hgeVector position, hgeVector* position_finish) :Home(home), Position(position) {
		int H = (int)sqrt(abs(position.x - position_finish->x) + abs(position.y - position_finish->y));
		F = H;
	};
};

class C_Path
{
public:
	C_Path(hgeVector* position_start, hgeVector* position_finish, std::list<C_Block*> *l_block);
	~C_Path();
	sqare* Get_Path() { return Finish_Path; }
	hgeVector GetFinish() { return Finish; };
	void Next();
	void update_path(std::list<C_Block*> *l_block);
private:
	std::list<sqare*> L_close;
	std::list<sqare*> L_open;
	std::list<hgeVector> L_block;
	hgeVector Finish;
	hgeVector Start;
	bool clash_block(hgeVector position);
	bool find_path();
	bool exam();
	void Start_Path();
	sqare* Finish_Path;
};

