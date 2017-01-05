#include "Path.h"


C_Path::C_Path(hgeVector* position_start, hgeVector* position_finish, std::list<C_Block*> *l_block)
{
	Finish = (*position_finish) + hgeVector(size_sprite, size_sprite);
	Start = *position_start;
	Finish_Path = NULL;

	if (!(*l_block).empty()) {
		int l = (*l_block).size();
		int i = 0;
		for (auto x = (*l_block).begin(); i < l; i++) {
			L_block.push_back((*x)->GetPosition());
			x++;
		}
	}

	sqare* start = new sqare(NULL, Start, &Finish);
	L_open.push_back(start);
	if (find_path()) {
		Start_Path();
	}
	else {
		Finish_Path = NULL;
	}

}

void C_Path::update_path(std::list<C_Block*> *l_block) {

	if (Finish_Path != NULL) {
		Start = Finish_Path->Position;
	}

	Finish_Path = NULL;
	if (!(*l_block).empty()) {
		int l = (*l_block).size();
		L_block.clear();
		int i = 0;
		for (auto x = (*l_block).begin(); i < (l * 2 / 3); i++) {
			L_block.push_back((*x)->GetPosition());
			x++;
		}
	}
	L_open.clear();
	L_close.clear();
	sqare* start = new sqare(NULL, Start, &Finish);
	L_open.push_back(start);
	if (find_path()) {
		Start_Path();
	}
	else {
		Finish_Path = NULL;
	}
}

C_Path::~C_Path()
{
	for (auto i = L_open.begin(); i != L_open.end();) {
		delete (*i);
		i = L_open.erase(i);
	}

	for (auto i = L_close.begin(); i != L_close.end();) {
		delete (*i);
		i = L_close.erase(i);
	}
}

bool C_Path::clash_block(hgeVector position) {
	if (!L_block.empty()) {
		int l = L_block.size();
		int i = 0;
		for (auto x = L_block.begin(); i < l; i++) {
			if ((*x) == position) {
				return false;
			}
			x++;
		}
	}

	if (!L_open.empty())
		for (auto x = L_open.begin(); x != L_open.end(); x++) {
			if ((*x)->Position == position) {
				return false;
			}
		}
	if (!L_close.empty())
		for (auto x = L_close.begin(); x != L_close.end(); x++) {
			if ((*x)->Position == position) {
				return false;
			}
		}
	return true;
};

bool C_Path::find_path() {
	sqare* home = NULL;
	float p;
	sqare* i;
	hgeVector v;

	while ((!exam()) && !L_open.empty())
	{
		home = *(L_open.begin());
		for (auto x = L_open.begin(); x != L_open.end(); x++) {
			if ((home->F) > ((*x)->F)) {
				home = (*x);
			}
		}
		for (auto x = L_open.begin(); x != L_open.end();) {
			if (home == (*x)) {
				x = L_open.erase(x);
			}
			else x++;
		}

		L_close.push_back(home);

		p = home->Position.x;
		if ((home->Position.x <= 25 * size_sprite) && (home->Position.x >= 0 * size_sprite)
			&& (home->Position.y <= 20 * size_sprite) && (home->Position.y >= 0 * size_sprite)) {
			v.x = (home->Position.x) + size_sprite;
			v.y = home->Position.y;
			if (clash_block(v)) {
				i = new sqare(home, v, &Finish);
				L_open.push_back(i);
			}
			v.x = (home->Position.x) - size_sprite;
			v.y = home->Position.y;
			if (clash_block(v)) {
				i = new sqare(home, v, &Finish);
				L_open.push_back(i);
			}
			v.x = home->Position.x;
			v.y = (home->Position.y) - size_sprite;
			if (clash_block(v)) {
				i = new sqare(home, v, &Finish);
				L_open.push_back(i);
			}
			v.x = home->Position.x;
			v.y = (home->Position.y) + size_sprite;
			if (clash_block(v)) {
				i = new sqare(home, v, &Finish);
				L_open.push_back(i);
			}
		}
	}
	if (!L_open.empty()) {
		return true;
	}
	else {
		return false;
	}
}

void C_Path::Start_Path() {
	sqare* i = Finish_Path;
	sqare* j = new sqare(NULL, i->Position, &Start);
	while (i->Home != NULL) {
		i = i->Home;
		sqare* k = new sqare(j, i->Position, &Start);
		j = k;
	}
	Finish_Path = j;
	delete i;
}

bool C_Path::exam() {
	for (auto i = L_open.begin(); i != L_open.end();i++) {
		if ((*i)->Position == Finish) {
			Finish_Path = (*i);
			return true;
		}
	}
	return false;
}

void C_Path::Next() {
	if ((Finish_Path != NULL)) {
		if (Finish_Path->Home != NULL) {
			sqare* i = Finish_Path;
			Finish_Path = Finish_Path->Home;
			delete i;
		}
	}
}