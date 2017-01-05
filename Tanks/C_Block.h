#pragma once

#include <hge.h>
#include <hgevector.h>
#include <hgesprite.h>
#include <list>



class C_Block
{
public:
	C_Block() {};
	C_Block(HTEXTURE p_block, bool game, bool enemy);
	~C_Block();
	void B_Render();
	hgeRect     GetBoundingBox() { return BoundingBox; };
	hgeVector GetPosition() { return v_Position; };
	bool GetEnemy() { return Enemy; };
	void SetPosition(hgeVector position) {
		v_Position = position;
		s_Block = new hgeSprite(p_Block, v_Position.x, v_Position.y, 32, 32);
		s_Block->SetHotSpot(16, 16);
		s_Block->GetBoundingBox(v_Position.x, v_Position.y, &BoundingBox);
	};
protected:
	hgeSprite* s_Block;
	hgeRect BoundingBox;
	hgeVector v_Position;

private:
	hgeVector f_Position();
	bool Game;
	bool Enemy;
	HTEXTURE p_Block;
};

class C_Lee :public C_Block
{
public:
	C_Lee(HTEXTURE p_lee, hgeVector position);
	~C_Lee() {};
	void Update();
	int GetHelth() { return Helth; };
private: int Helth;
};

class C_Base :public C_Block
{
public:
	C_Base(HTEXTURE p_base, int x, int y);
	~C_Base() {};
};
