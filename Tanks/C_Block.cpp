#include "C_Block.h"

const int size_sprite = 32;
C_Block::C_Block(HTEXTURE p_block, bool game, bool enemy) : Game(game), Enemy(enemy)
{
	p_Block = p_block;
	v_Position = f_Position();
	s_Block = new hgeSprite(p_block, v_Position.x, v_Position.y, 32, 32);
	s_Block->SetHotSpot(16, 16);
	s_Block->GetBoundingBox(v_Position.x, v_Position.y, &BoundingBox);
}
C_Block::~C_Block()
{
	delete s_Block;
}



hgeVector C_Block::f_Position()
{
	hgeVector tm;
	HGE* hge = hgeCreate(HGE_VERSION);

	if (Game) {
		bool NO = true;
		while (NO)
		{
			NO = false;
			tm.x = (float)hge->Random_Int(size_sprite, size_sprite * 25);
			tm.y = (float)hge->Random_Int(size_sprite, size_sprite * 20);
			tm.x -= (int)tm.x % size_sprite;
			tm.y -= (int)tm.y % size_sprite;
			if ((tm.x > 8 * size_sprite) && (tm.x<15 * size_sprite) && (tm.y > 6 * size_sprite) && (tm.y < 13 * size_sprite))
				NO = true;
		}
	}
	else {
		bool NO = true;
		while (NO)
		{
			NO = false;
			tm.x = (float)hge->Random_Int(size_sprite, size_sprite * 25);
			tm.y = (float)hge->Random_Int(size_sprite, size_sprite * 20);
			tm.x -= (int)tm.x % size_sprite;
			tm.y -= (int)tm.y % size_sprite;
			if (((tm.x < 5 * size_sprite) && (tm.y < 5 * size_sprite)) || ((tm.x > 20 * size_sprite) && (tm.y > 15 * size_sprite)))
				NO = true;
		}
	}
	hge->Release();
	return tm;

};

void C_Block::B_Render()
{
	s_Block->Render(v_Position.x, v_Position.y);
};

C_Lee::C_Lee(HTEXTURE p_lee, hgeVector position)
{
	v_Position = position;
	s_Block = new hgeSprite(p_lee, position.x, position.y, 32, 32);
	s_Block->SetHotSpot(16, 16);
	s_Block->SetTextureRect(0, 0, 32, 32);
	s_Block->GetBoundingBox(v_Position.x, v_Position.y, &BoundingBox);
	Helth = 0;

};

C_Base::C_Base(HTEXTURE p_base, int x, int y)
{
	v_Position.x = (float)x;
	v_Position.y = (float)y;
	s_Block = new hgeSprite(p_base, v_Position.x, v_Position.y, 64, 64);
	s_Block->SetHotSpot(16, 16);
	s_Block->GetBoundingBox(v_Position.x, v_Position.y, &BoundingBox);
};

void C_Lee::Update()
{
	Helth++;
	s_Block->SetTextureRect((Helth * 32), 0, 32, 32);
}

