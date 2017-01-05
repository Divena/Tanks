#pragma once
#include <hge.h>
#include <hgevector.h>
#include <hgesprite.h>
#include <list>
#include "C_Block.h"
#include "C_Player.h"
#include "C_Bullet.h"
#include "C_Enemy.h"
#include "Path.h"
#include "menuitem.h"

using namespace std;

const int col_enemy = 3;

const int col_blocks = 250;

struct explosion
{
	hgeAnimation*   Animation;
	hgeVector       Position;
};
struct GameOver {
	bool b_GameOver;
	hgeVector Position;
	float Step;
	hgeSprite *s_GameOver = NULL;
	GameOver(bool gameover, HTEXTURE im) {
		Position = hgeVector(200, -314);
		s_GameOver = new hgeSprite(im, Position.x, Position.y, 388, 314);
		Step = 2;
	};
};

HGE *hge = 0;


HTEXTURE p_Bullet = 0;
HTEXTURE p_GameOver = 0;
HTEXTURE p_Grass = 0;
HTEXTURE p_Block = 0;
HTEXTURE p_Lee = 0;
HTEXTURE p_Enemy = 0;
HTEXTURE p_Base = 0;
HTEXTURE p_Base2 = 0;
HTEXTURE p_Player = 0;
HTEXTURE p_Player2 = 0;
HTEXTURE p_Explosion = 0;
HEFFECT  m_Explosion = 0;

HEFFECT  m_GameOver = 0;
HEFFECT Game1 = 0;
HCHANNEL ch_Game1 = 0;
HEFFECT Game2 = 0;
HCHANNEL ch_Game2 = 0;

HEFFECT Menu = 0;
HCHANNEL ch_Menu = 0;

C_Player*   Player1 = NULL;
C_Player*   Player2 = NULL;
hgeSprite* s_Grass = NULL;
C_Base* Base = NULL;
C_Base* Base2 = NULL;
list<C_Block*> L_Blocks;
list<C_Lee*> L_Lee;
list<C_Bullet*> Bullets;
list<C_Enemy*> L_Enemy;
list<C_Enemy*> L_EnemyClose;
list<explosion> L_Explosions;
bool Shot_true[col_enemy + 2];
C_Path* Path[col_enemy];
bool Game = false;
GameOver *gameover;

HEFFECT	snd;
HTEXTURE tex;
hgeQuad	quad;
hgeQuad	quad2;
hgeQuad	quad1;
hgeGUI *GuiStart;
hgeGUI *GuiPause;

hgeFont	*fnt;
hgeSprite *spr;


void shot(hgeVector Pos, float Angle, int number)
{
	if (Shot_true[number] || number == col_enemy || number == col_enemy + 1) {
		hgeVector vel;
		if (Angle == 0) {
			vel.x = 4; vel.y = 0;
		}
		else if (Angle == M_PI) {
			vel.x = -4; vel.y = 0;
		}
		else if (Angle == -M_PI_2) {
			vel.x = 0; vel.y = -4;
		}
		else {
			vel.x = 0; vel.y = 4;
		}

		C_Bullet* Bullet = new C_Bullet(Pos, vel, p_Bullet, Angle, number);

		Bullets.push_back(Bullet);
		Shot_true[number] = false;
	}
}
void clash(C_Player* play, hgeRect sprite)
{
	if (sprite.Intersect(&play->GetBoundingBox())) {
		play->SetPosition(play->GetPosition_back());
	}
}
void clash(C_Enemy* play, hgeRect sprite)
{
	if (sprite.Intersect(&play->GetBoundingBox())) {
		play->SetPosition(play->GetPosition_back());
		shot(play->GetPosition(), play->GetAngle(), play->GetNumber());
	}
}
void shot_player(C_Enemy* enemy, hgeVector base) {
	hgeVector position = enemy->GetPosition();
	if ((abs(position.x - base.x) < (size_sprite / 2)) || (abs(position.x - base.x - size_sprite) < (size_sprite / 2))) {
		for (auto x = L_Blocks.begin(); x != L_Blocks.end(); x++) {
			if (!(((*x)->GetPosition().x == position.x) && ((((*x)->GetPosition().y < position.y) && ((*x)->GetPosition().y > base.y)) || (((*x)->GetPosition().y > position.y) && ((*x)->GetPosition().y < base.y))))) {
				shot(enemy->GetPosition(), enemy->GetAngle(), enemy->GetNumber());
			}
		}
	}

	if ((abs(position.y - base.y) < (size_sprite / 2)) || (abs(position.y - base.y - size_sprite)< (size_sprite / 2))) {
		for (auto x = L_Blocks.begin(); x != L_Blocks.end(); x++) {
			if (!(((*x)->GetPosition().y == position.y) && ((((*x)->GetPosition().x < position.x) && ((*x)->GetPosition().x > base.x)) || (((*x)->GetPosition().x > position.x) && ((*x)->GetPosition().x < base.x))))) {
				shot(enemy->GetPosition(), enemy->GetAngle(), enemy->GetNumber());
			}
		}
	}

}
void shot_base(C_Enemy* enemy, hgeVector player) {
	hgeVector position = enemy->GetPosition();
	if (position.x == player.x) {
		for (auto x = L_Blocks.begin(); x != L_Blocks.end(); x++) {
			if (!(((*x)->GetPosition().x == position.x) && ((((*x)->GetPosition().y < position.y) &&
				((*x)->GetPosition().y > player.y)) || (((*x)->GetPosition().y > position.y) && ((*x)->GetPosition().y < player.y))))) {
				shot(enemy->GetPosition(), enemy->GetAngle(), enemy->GetNumber());
			}
		}
	}

	if (position.y == player.y) {
		for (auto x = L_Blocks.begin(); x != L_Blocks.end(); x++) {
			if (!(((*x)->GetPosition().y == position.y) && ((((*x)->GetPosition().x < position.x) &&
				((*x)->GetPosition().x > player.x)) || (((*x)->GetPosition().x > position.x) && ((*x)->GetPosition().x < player.x))))) {
				shot(enemy->GetPosition(), enemy->GetAngle(), enemy->GetNumber());
			}
		}
	}

}
void CreateExplosion(hgeVector Position)
{
	explosion exp;
	exp.Animation = new hgeAnimation(p_Explosion, 5, 10, 0, 0, 118, 118);
	exp.Animation->SetHotSpot(59, 59);
	exp.Animation->Play();
	exp.Position = Position;
	L_Explosions.push_back(exp);
}
void Block_Lee(hgeVector position) {

	C_Block* block = NULL;

	block = new C_Block(p_Block, true, true);
	block->SetPosition(hgeVector(position.x + size_sprite, position.y));
	for (auto x = L_Blocks.begin(); x != L_Blocks.end() && !L_Blocks.empty();) {
		if (block->GetBoundingBox().Intersect(&(*x)->GetBoundingBox())) {
			delete (*x);
			x = L_Blocks.erase(x);
		}
		else x++;
	}
	L_Blocks.push_back(block);
	block = new C_Block(p_Block, true, true);
	block->SetPosition(hgeVector(position.x - size_sprite, position.y));
	for (auto x = L_Blocks.begin(); x != L_Blocks.end() && !L_Blocks.empty();) {

		if (block->GetBoundingBox().Intersect(&(*x)->GetBoundingBox())) {
			delete (*x);
			x = L_Blocks.erase(x);
		}
		else x++;
	}
	L_Blocks.push_back(block);

	block = new C_Block(p_Block, true, true);
	block->SetPosition(hgeVector(position.x, position.y + size_sprite));
	for (auto x = L_Blocks.begin(); x != L_Blocks.end() && !L_Blocks.empty();) {
		if (block->GetBoundingBox().Intersect(&(*x)->GetBoundingBox())) {
			delete (*x);
			x = L_Blocks.erase(x);
		}
		else x++;
	}
	L_Blocks.push_back(block);

	block = new C_Block(p_Block, true, true);
	block->SetPosition(hgeVector(position.x, position.y - size_sprite));
	for (auto x = L_Blocks.begin(); x != L_Blocks.end() && !L_Blocks.empty();) {

		if (block->GetBoundingBox().Intersect(&(*x)->GetBoundingBox())) {
			delete (*x);
			x = L_Blocks.erase(x);
		}
		else x++;
	}
	L_Blocks.push_back(block);

};

bool FrameFuncPauseMenu();
bool RenderFuncPauseMenu();
void CreateGame() {
	p_GameOver = hge->Texture_Load("tank/123.png");
	p_Grass = hge->Texture_Load("tank/212208.jpg");
	p_Block = hge->Texture_Load("tank/block_01.jpg");
	p_Lee = hge->Texture_Load("tank/block_04.bmp");
	p_Base = hge->Texture_Load("tank/TileSetVX___Global_4.png");
	p_Bullet = hge->Texture_Load("tank/bullet02.png");
	p_Player = hge->Texture_Load("tank/11.png");
	p_Enemy = hge->Texture_Load("tank/12.png");
	p_Explosion = hge->Texture_Load("tank/Explosion-Sprite-Sheet.png");
	m_Explosion = hge->Effect_Load("tank/21410.ogg");
	m_GameOver = hge->Effect_Load("tank/Tanchiki_Dendi_-_End.mp3");
	Game1 = hge->Effect_Load("tank/Blue Stahli - 88 Rounds Per Minute.mp3");

	gameover = new GameOver(true, p_GameOver);

	s_Grass = new hgeSprite(p_Grass, 0, 0, 25 * size_sprite, 20 * size_sprite);

	Player1 = new C_Player(hgeVector(11 * size_sprite, 7 * size_sprite), p_Player, col_enemy, true);
	////////////////////////////////////////////////////////
	Base = new C_Base(p_Base, 11 * size_sprite, 9 * size_sprite);
	///////////////////////////////////////////////
	for (int i(0); i < col_blocks; i++) {
		bool NO = false;
		C_Block* block = new C_Block(p_Block, true, false);

		for (auto i = L_Blocks.begin(); i != L_Blocks.end() && !L_Blocks.empty(); i++) {
			if ((*i)->GetPosition() == block->GetPosition())
				NO = true;
		}
		if (!NO)
			L_Blocks.push_back(block);
	}


	///////////////////////////////////////////////

	/**/ {
		int K = size_sprite;
		C_Lee* lee = 0;
		hgeVector position;
		position.x = 10 * size_sprite;
		position.y = 8 * size_sprite;
		for (int i(0); i < 2; i++)
		{
			for (int j(0); j < 3; j++) {
				position.x += K;
				lee = new C_Lee(p_Lee, position);
				L_Lee.push_back(lee);
			}

			for (int j(0); j < 3; j++) {
				position.y += K;
				lee = new C_Lee(p_Lee, position);
				L_Lee.push_back(lee);
			}
			K *= -1;
		}
	}/**/

	for (int i(0); i < col_enemy;) {
		bool NO = false;
		C_Enemy* enemy;
		enemy = new C_Enemy(p_Enemy, i, true);
		for (auto x = L_Enemy.begin(); x != L_Enemy.end() && !L_Enemy.empty(); x++) {
			if ((*x)->GetPosition() == enemy->GetPosition())
				NO = true;
		}
		if (!NO) {
			L_Enemy.push_back(enemy);
			Shot_true[i] = true;
			Path[i] = new C_Path(&enemy->GetPosition(), &Base->GetPosition(), &L_Blocks);
			i++;

		}

	}

	for (auto i = L_Enemy.begin(); i != L_Enemy.end(); i++) {
		for (auto x = L_Blocks.begin(); x != L_Blocks.end();) {
			if ((*x)->GetPosition() == (*i)->GetPosition()) {
				delete (*x);
				x = L_Blocks.erase(x);
				break;
			}
			else x++;
		}
	}
	ch_Game1 = hge->Effect_Play(Game1);
	hge->Channel_SetVolume(ch_Game1, 40);
};
void CloseGame() {
	hge->Channel_Stop(ch_Game2);
	hge->Texture_Free(p_Grass);
	hge->Texture_Free(p_Block);
	hge->Texture_Free(p_Lee);
	hge->Texture_Free(p_Base);
	hge->Texture_Free(p_Player);
	hge->Texture_Free(p_GameOver);
	hge->Texture_Free(p_Bullet);
	hge->Texture_Free(p_Explosion);
	hge->Effect_Free(m_Explosion);
	hge->Effect_Free(Game1);
	////////////////////////////////

	for (auto i = Bullets.begin(); i != Bullets.end(); /**/) {
		delete (*i);
		i = Bullets.erase(i);
	}
	///////////////////////////////////


	for (auto i = L_Explosions.begin(); i != L_Explosions.end(); /**/)
	{
		delete (*i).Animation;
		i = L_Explosions.erase(i);
	}
	//////////////////////////////////
	for (auto i = L_Blocks.begin(); i != L_Blocks.end();) {
		delete (*i);
		i = L_Blocks.erase(i);
	}
	///////////////////////////////
	for (auto i = L_Lee.begin(); i != L_Lee.end();) {
		delete (*i);
		i = L_Lee.erase(i);
	}
	/////////////////////
	for (auto i = L_Enemy.begin(); i != L_Enemy.end();) {
		delete (*i);
		i = L_Enemy.erase(i);
	}
	/////////////////////
	delete Base;
	delete s_Grass;
	delete Player1;
	delete gameover;
};

void CreateGame2() {
	p_GameOver = hge->Texture_Load("tank/123.png");
	p_Grass = hge->Texture_Load("tank/212208.jpg");
	p_Block = hge->Texture_Load("tank/block_01.jpg");
	p_Lee = hge->Texture_Load("tank/block_04.bmp");
	p_Base = hge->Texture_Load("tank/TileSetVX___Global_4.png");
	p_Base2 = hge->Texture_Load("tank/12345.png");
	p_Bullet = hge->Texture_Load("tank/bullet02.png");
	p_Player = hge->Texture_Load("tank/11.png");
	p_Player2 = hge->Texture_Load("tank/13.png");
	p_Enemy = hge->Texture_Load("tank/12.png");
	p_Explosion = hge->Texture_Load("tank/Explosion-Sprite-Sheet.png");
	m_Explosion = hge->Effect_Load("tank/21410.ogg");
	m_GameOver = hge->Effect_Load("tank/tanks/");
	Game2 = hge->Effect_Load("tank/Blue Stahli - Accelerant.mp3");

	gameover = new GameOver(true, p_GameOver);
	s_Grass = new hgeSprite(p_Grass, 0, 0, 25 * size_sprite, 20 * size_sprite);

	Player1 = new C_Player(hgeVector(4 * size_sprite, 4 * size_sprite), p_Player, col_enemy, true);
	Player2 = new C_Player(hgeVector(24 * size_sprite, 16 * size_sprite), p_Player2, col_enemy + 1, false);
	Shot_true[Player1->GetNumber()] = true;
	Shot_true[Player2->GetNumber()] = true;
	////////////////////////////////////////////////////////
	Base = new C_Base(p_Base, 1 * size_sprite, 1 * size_sprite);
	Base2 = new C_Base(p_Base2, 23 * size_sprite, 18 * size_sprite);
	///////////////////////////////////////////////
	for (int i(0); i < col_blocks; i++) {
		bool NO = false;
		C_Block* block = new C_Block(p_Block, false, false);

		for (auto i = L_Blocks.begin(); i != L_Blocks.end() && !L_Blocks.empty(); i++) {
			if ((*i)->GetPosition() == block->GetPosition())
				NO = true;
		}
		if (!NO)
			L_Blocks.push_back(block);
	}


	///////////////////////////////////////////////

	/**/ {
		int K = size_sprite;
		C_Lee* lee = 0;
		hgeVector position;
		position.x = -1 * size_sprite;
		position.y = 3 * size_sprite;
		for (int i(0); i < 3; i++)
		{
			for (int j(0); j < 4; j++) {
				position.x += K;
				lee = new C_Lee(p_Lee, position);
				L_Lee.push_back(lee);
			}

			for (int j(0); j < 3; j++) {
				position.y -= K;
				lee = new C_Lee(p_Lee, position);
				L_Lee.push_back(lee);
			}
			position.x = 26 * size_sprite;
			position.y = 17 * size_sprite;
			K *= -1;
		}
	}/**/

	for (int i(0); i < col_enemy; ) {
		bool NO = false;
		C_Enemy* enemy;
		enemy = new C_Enemy(p_Enemy, i, false);
		for (auto x = L_EnemyClose.begin(); x != L_EnemyClose.end() && !L_EnemyClose.empty(); x++) {
			if ((*x)->GetPosition() == enemy->GetPosition())
				NO = true;
		}
		if (!NO) {
			Block_Lee(enemy->GetPosition());
			L_EnemyClose.push_back(enemy);
			Shot_true[i] = true;
			i++;
		}

	}

	for (auto i = L_EnemyClose.begin(); i != L_EnemyClose.end(); i++) {
		for (auto x = L_Blocks.begin(); x != L_Blocks.end();) {
			if ((*x)->GetPosition() == (*i)->GetPosition()) {
				delete (*x);
				x = L_Blocks.erase(x);
				break;
			}
			else x++;
		}
	}
	ch_Game2 = hge->Effect_Play(Game2);
	hge->Channel_SetVolume(ch_Game2, 20);
};
void CloseGame2() {
	hge->Channel_Stop(ch_Game2);
	hge->Texture_Free(p_Grass);
	hge->Texture_Free(p_Player);
	hge->Texture_Free(p_Player2);
	hge->Texture_Free(p_Block);
	hge->Texture_Free(p_Lee);
	hge->Texture_Free(p_Base);
	hge->Texture_Free(p_Base2);
	hge->Texture_Free(p_Bullet);
	hge->Texture_Free(p_GameOver);
	hge->Texture_Free(p_Explosion);
	hge->Effect_Free(m_Explosion);
	hge->Effect_Free(Game2);

	////////////////////////////////

	for (auto i = Bullets.begin(); i != Bullets.end(); /**/) {
		delete (*i);
		i = Bullets.erase(i);
	}
	///////////////////////////////////


	for (auto i = L_Explosions.begin(); i != L_Explosions.end(); /**/)
	{
		delete (*i).Animation;
		i = L_Explosions.erase(i);
	}
	//////////////////////////////////
	for (auto i = L_Blocks.begin(); i != L_Blocks.end();) {
		delete (*i);
		i = L_Blocks.erase(i);
	}
	///////////////////////////////
	for (auto i = L_Lee.begin(); i != L_Lee.end();) {
		delete (*i);
		i = L_Lee.erase(i);
	}
	/////////////////////
	for (auto i = L_Enemy.begin(); i != L_Enemy.end();) {
		delete (*i);
		i = L_Enemy.erase(i);
	}

	for (auto i = L_EnemyClose.begin(); i != L_EnemyClose.end();) {
		delete (*i);
		i = L_EnemyClose.erase(i);
	}
	/////////////////////
	delete gameover;
	delete Base;
	delete Base2;
	delete s_Grass;
	delete Player1;
	delete Player2;
};

void CreateStartMenu() {
	quad.tex = hge->Texture_Load("tank/p1_307200007204371.jpg");
	quad1.tex = hge->Texture_Load("tank/1289423842_gr_vect_txt-5.jpg");
	quad2.tex = hge->Texture_Load("tank/logo_s.png");
	tex = hge->Texture_Load("tank/cursor.png");
	snd = hge->Effect_Load("tank/menu.wav");
	Menu = hge->Effect_Load("tank/Blue Stahli - Overklock.mp3");
	fnt = new hgeFont("tank/font1.fnt");

	quad.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	for (int i = 0; i<4; i++)
	{
		quad.v[i].z = 0.5f;
		quad.v[i].col = 0xFFFFFFFF;
	}
	quad.v[0].tx = 0.05f; quad.v[0].ty = 0.05f;
	quad.v[1].tx = 0.95f; quad.v[1].ty = 0.05f;
	quad.v[2].tx = 0.95f; quad.v[2].ty = 0.95f;
	quad.v[3].tx = 0.05f; quad.v[3].ty = 0.95f;
	quad.v[0].x = 0; quad.v[0].y = 0;
	quad.v[1].x = 800; quad.v[1].y = 0;
	quad.v[2].x = 800; quad.v[2].y = 640;
	quad.v[3].x = 0; quad.v[3].y = 640;



	quad2.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	for (int i = 0; i<4; i++)
	{
		quad2.v[i].z = 0.5f;
		quad2.v[i].col = 0xFFFFFFFF;
	}
	quad2.v[0].tx = 0; quad2.v[0].ty = 0;
	quad2.v[1].tx = 1; quad2.v[1].ty = 0;
	quad2.v[2].tx = 1; quad2.v[2].ty = 1;
	quad2.v[3].tx = 0; quad2.v[3].ty = 1;
	quad2.v[0].x = 50; quad2.v[0].y = 50;
	quad2.v[1].x = quad2.v[0].x + 176; quad2.v[1].y = quad2.v[0].y;
	quad2.v[2].x = quad2.v[1].x; quad2.v[2].y = quad2.v[0].y + 76;
	quad2.v[3].x = quad2.v[0].x; quad2.v[3].y = quad2.v[2].y;

	quad1.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;

	for (int i = 0; i<4; i++)
	{
		quad1.v[i].z = 0.5f;
		quad1.v[i].col = 0x99FFFFFF;
	}

	quad1.v[0].tx = 0; quad1.v[0].ty = 0;
	quad1.v[1].tx = 1; quad1.v[1].ty = 0;
	quad1.v[2].tx = 1; quad1.v[2].ty = 1;
	quad1.v[3].tx = 0; quad1.v[3].ty = 1;
	quad1.v[0].x = 260; quad1.v[0].y = 140;
	quad1.v[1].x = 525; quad1.v[1].y = 140;
	quad1.v[2].x = 525; quad1.v[2].y = 440;
	quad1.v[3].x = 260; quad1.v[3].y = 440;



	spr = new hgeSprite(tex, 0, 0, 32, 32);

	GuiStart = new hgeGUI();

	GuiStart->AddCtrl(new Ñ_hgeGUIMenuItem(1, fnt, snd, 400, 200, 0.0f, "1 Player"));
	GuiStart->AddCtrl(new Ñ_hgeGUIMenuItem(2, fnt, snd, 400, 240, 0.1f, "1x1 (2 players)"));
	GuiStart->AddCtrl(new Ñ_hgeGUIMenuItem(3, fnt, snd, 400, 320, 0.4f, "Exit"));

	GuiStart->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
	GuiStart->SetCursor(spr);
	GuiStart->SetFocus(1);
	GuiStart->Enter();
	ch_Menu = hge->Effect_Play(Menu);
	hge->Channel_SetVolume(ch_Menu, 20);
}
void CloseStartMenu() {
	hge->Effect_Free(Menu);
	delete GuiStart;
	delete fnt;
	delete spr;
	hge->Effect_Free(snd);
	hge->Texture_Free(tex);
	hge->Texture_Free(quad.tex);
}

void CreateMenuPause() {
	GuiPause = new hgeGUI();

	GuiPause->AddCtrl(new Ñ_hgeGUIMenuItem(1, fnt, snd, 400, 200, 0.0f, "New Game"));
	GuiPause->AddCtrl(new Ñ_hgeGUIMenuItem(2, fnt, snd, 400, 240, 0.1f, "Continue"));

	GuiPause->AddCtrl(new Ñ_hgeGUIMenuItem(3, fnt, snd, 400, 360, 0.4f, "back"));

	GuiPause->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
	GuiPause->SetCursor(spr);
	GuiPause->SetFocus(1);
	GuiPause->Enter();
}
void CloseMenuPause() {
	delete GuiPause;
}

bool RenderFuncGame()
{
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	s_Grass->Render(0, 0);

	for (auto i = L_Blocks.begin(); i != L_Blocks.end(); i++) {
		(*i)->B_Render();
	}

	for (auto i = L_Lee.begin(); i != L_Lee.end(); i++) {
		(*i)->B_Render();
	}
	Base->B_Render();
	Player1->Render();

	for (auto i = L_Enemy.begin(); i != L_Enemy.end(); i++) {
		(*i)->Render();
	}

	for (auto i = Bullets.begin(); i != Bullets.end(); i++) {
		(*i)->Render();
	}

	for (auto i = L_Explosions.begin(); i != L_Explosions.end(); i++)
	{
		(*i).Animation->Render((*i).Position.x, (*i).Position.y);
	}
	gameover->s_GameOver->Render(gameover->Position.x, gameover->Position.y);
	hge->Gfx_EndScene();
	return false;
}
bool FrameFuncGame()
{
	float delta = hge->Timer_GetDelta();

	/////////////////////////////////////////////
	if (gameover->b_GameOver) {

		if (hge->Input_KeyDown(HGEK_SPACE)) {
			shot(Player1->GetPosition(), Player1->GetAngle(), Player1->GetNumber());
		}
		////////////////////////////////////////////

		for (auto i = Bullets.begin(); i != Bullets.end(); /**/) {
			(*i)->Update();
			if ((*i)->GetPosition().x > 25 * size_sprite || (*i)->GetPosition().x < 0 || (*i)->GetPosition().y < 0 || (*i)->GetPosition().y > 20 * size_sprite) {
				Shot_true[(*i)->GetNumber()] = true;
				delete (*i);
				i = Bullets.erase(i);
			}
			else i++;
		}
		/////////////////////////////////////////

		Player1->Update(delta);
		/////////////////////////////////////
		if (!L_Enemy.empty()) {
			for (auto x = L_Enemy.begin(); x != L_Enemy.end(); x++) {
				sqare* path = Path[(*x)->GetNumber()]->Get_Path();
				if (path != NULL) {
					if ((*x)->GetPosition() == path->Position) {
						Path[(*x)->GetNumber()]->Next();
					}
					(*x)->Update(delta, path->Position);
				}
				else {
					shot((*x)->GetPosition(), (*x)->GetAngle(), (*x)->GetNumber());
					Path[(*x)->GetNumber()]->update_path(&L_Blocks);
				}
				shot_base((*x), Base->GetPosition());
				shot_player((*x), Player1->GetPosition());
			}

		}
		///////////////////////////////////
		for (auto i = L_Explosions.begin(); i != L_Explosions.end(); /**/)
		{
			if ((*i).Animation->GetFrame() == 4)
			{
				delete (*i).Animation;
				i = L_Explosions.erase(i);
			}
			else
			{
				(*i).Animation->Update(delta);
				i++;
			}
			hge->Effect_PlayEx(m_Explosion, 100, 0);
		}
		///////////////////////////////////
		if (!L_Blocks.empty()) {
			for (auto x = L_Blocks.begin(); x != L_Blocks.end(); x++) {
				clash(Player1, (*x)->GetBoundingBox());
			}

		}
		///////////////////////////
		for (auto i = Bullets.begin(); i != Bullets.end() && !Bullets.empty();) {
			if ((Player1->GetBoundingBox().Intersect(&(*i)->GetBoundingBox())) && (*i)->GetNumber() != Player1->GetNumber()) {
				Shot_true[(*i)->GetNumber()] = true;
				delete (*i);
				i = Bullets.erase(i);
				CreateExplosion(Player1->GetPosition());
				Player1->SetPosition(Player1->GetStart());
				break;
			}
			else i++;
		}
		/////////////////////////////////
		if (!L_Lee.empty()) {
			for (auto x = L_Lee.begin(); x != L_Lee.end(); x++) {
				clash(Player1, (*x)->GetBoundingBox());
			}
		}
		/////////////////////////////////////
		clash(Player1, Base->GetBoundingBox());
		//////////////////////////////////////////////////
		for (auto x = L_Blocks.begin(); x != L_Blocks.end() && !L_Blocks.empty(); x++) {
			for (auto i = L_Enemy.begin(); i != L_Enemy.end() && !L_Enemy.empty(); i++) {
				clash((*i), (*x)->GetBoundingBox());
			}
		}

		//////////////////////////////////////////////
		for (auto x = L_Lee.begin(); x != L_Lee.end() && !L_Lee.empty(); x++) {
			for (auto i = L_Enemy.begin(); i != L_Enemy.end() && !L_Enemy.empty(); i++) {
				clash((*i), (*x)->GetBoundingBox());
			}
		}


		///////////////////////////////////////////
		for (auto i = L_Enemy.begin(); i != L_Enemy.end() && !L_Enemy.empty(); i++) {
			if ((*i)->GetBoundingBox().Intersect(&Player1->GetBoundingBox())) {
				CreateExplosion((*i)->GetPosition());
				CreateExplosion(Player1->GetPosition());
				Player1->SetPosition(Player1->GetStart());
				(*i)->SetPosition((*i)->Random_Position());
				Path[(*i)->GetNumber()]->update_path(&L_Blocks);
			}
		}

		//////////////////////////////////////////////
		for (auto i = L_Enemy.begin(); i != L_Enemy.end() && !L_Enemy.empty(); i++) {
			clash((*i), Base->GetBoundingBox());
		}
		///////////////////////////////////////////////////////
		for (auto x = L_Blocks.begin(); x != L_Blocks.end() && !L_Blocks.empty();) {
			bool incr = false;
			for (auto i = Bullets.begin(); i != Bullets.end() && !Bullets.empty();) {
				if ((*x)->GetBoundingBox().Intersect(&(*i)->GetBoundingBox())) {
					Shot_true[(*i)->GetNumber()] = true;
					delete (*i);
					i = Bullets.erase(i);

					delete (*x);
					x = L_Blocks.erase(x);
					incr = true;
					break;
				}
				else i++;

			}

			if (!incr) x++;
		}
		/////////////////////////////////////////////////
		for (auto i = Bullets.begin(); i != Bullets.end() && !Bullets.empty(); i++) {
			if ((*i)->GetBoundingBox().Intersect(&Base->GetBoundingBox())) {
				gameover->b_GameOver = false;
				hge->Channel_Stop(ch_Game1);
				hge->Effect_Play(m_GameOver);
			}
		}
		//////////////////////////////////////////////////
		for (auto x = L_Enemy.begin(); x != L_Enemy.end() && !L_Enemy.empty();) {
			bool incr = false;
			for (auto i = Bullets.begin(); i != Bullets.end() && !Bullets.empty();) {
				if ((*x)->GetBoundingBox().Intersect(&(*i)->GetBoundingBox()) && (*i)->GetNumber() != (*x)->GetNumber()) {
					Shot_true[(*i)->GetNumber()] = true;
					if ((*i)->GetNumber() == Player1->GetNumber()) {
						Player1->SetCounter();
					}
					delete (*i);
					i = Bullets.erase(i);
					CreateExplosion((*x)->GetPosition());
					(*x)->SetPosition((*x)->Random_Position());
					Path[(*x)->GetNumber()]->update_path(&L_Blocks);
					incr = true;

					break;
				}
				else i++;

			}

			if (!incr) x++;
		}
		////////////////////////////////////////////////// 
		for (auto x = L_Lee.begin(); x != L_Lee.end() && !L_Lee.empty();) {
			bool incr = false;
			for (auto i = Bullets.begin(); i != Bullets.end() && !Bullets.empty();) {
				if ((*x)->GetBoundingBox().Intersect(&(*i)->GetBoundingBox())) {

					Shot_true[(*i)->GetNumber()] = true;
					delete (*i);
					i = Bullets.erase(i);

					if ((*x)->GetHelth() == 2) {
						delete (*x);
						x = L_Lee.erase(x);
						incr = true;
					}
					else
						(*x)->Update();
					break;
				}
				else i++;

			}
			if (!incr) x++;
		}
	}
	else {
		if (gameover->Position.y < 200) {
			gameover->Position.y += gameover->Step;
		}
	}
	///////////////////////////////////////////

	if (hge->Input_GetKeyState(HGEK_ESCAPE)) {
		CreateMenuPause();
		hge->System_SetState(HGE_FRAMEFUNC, FrameFuncPauseMenu);
		hge->System_SetState(HGE_RENDERFUNC, RenderFuncPauseMenu);
	};
	return false;
}

bool RenderFuncGame2()
{
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);

	s_Grass->Render(0, 0);

	for (auto i = L_Blocks.begin(); i != L_Blocks.end(); i++) {
		(*i)->B_Render();
	}
	for (auto i = L_EnemyClose.begin(); i != L_EnemyClose.end(); i++) {
		(*i)->Render();
	}
	for (auto i = L_Lee.begin(); i != L_Lee.end(); i++) {
		(*i)->B_Render();
	}
	Base->B_Render();
	Player1->Render();
	Base2->B_Render();
	Player2->Render();

	for (auto i = L_Enemy.begin(); i != L_Enemy.end(); i++) {
		(*i)->Render();
	}

	for (auto i = Bullets.begin(); i != Bullets.end(); i++) {
		(*i)->Render();
	}

	for (auto i = L_Explosions.begin(); i != L_Explosions.end(); i++)
	{
		(*i).Animation->Render((*i).Position.x, (*i).Position.y);
	}
	gameover->s_GameOver->Render(gameover->Position.x, gameover->Position.y);

	hge->Gfx_EndScene();
	return false;
}
bool FrameFuncGame2()
{
	float delta = hge->Timer_GetDelta();

	/////////////////////////////////////////////
	if (gameover->b_GameOver) {
		if (hge->Input_KeyDown(HGEK_SPACE)) {
			shot(Player2->GetPosition(), Player2->GetAngle(), Player2->GetNumber());
		}
		if (hge->Input_KeyDown(HGEK_SHIFT)) {
			shot(Player1->GetPosition(), Player1->GetAngle(), Player1->GetNumber());
		}

		////////////////////////////////////////////
		if (!Bullets.empty()) {
			for (auto i = Bullets.begin(); i != Bullets.end(); /**/) {
				(*i)->Update();
				if ((*i)->GetPosition().x > 25 * size_sprite || (*i)->GetPosition().x < 0 || (*i)->GetPosition().y < 0 || (*i)->GetPosition().y > 20 * size_sprite) {
					Shot_true[(*i)->GetNumber()] = true;
					delete (*i);
					i = Bullets.erase(i);
				}
				else i++;
			}
		}
		/////////////////////////////////////////
		Player1->Update(delta);
		Player2->Update(delta);
		/////////////////////////////////////
		if (!L_Enemy.empty()) {
			for (auto x = L_Enemy.begin(); x != L_Enemy.end(); x++) {
				sqare* path = Path[(*x)->GetNumber()]->Get_Path();
				if (path != NULL) {
					if ((*x)->GetPosition() == path->Position) {
						Path[(*x)->GetNumber()]->Next();
					}
					(*x)->Update(delta, path->Position);
				}
				else {
					shot((*x)->GetPosition(), (*x)->GetAngle(), (*x)->GetNumber());
					Path[(*x)->GetNumber()]->update_path(&L_Blocks);
				}
				if (Path[(*x)->GetNumber()]->GetFinish() == (Base->GetPosition() + hgeVector(size_sprite, size_sprite))) {
					shot_base((*x), Base->GetPosition());
					shot_player((*x), Player1->GetPosition());
				}
				else {
					shot_base((*x), Base2->GetPosition());
					shot_player((*x), Player2->GetPosition());
				}
			}

		}
		///////////////////////////////////
		for (auto i = L_Explosions.begin(); i != L_Explosions.end(); /**/)
		{
			if ((*i).Animation->GetFrame() == 4)
			{
				delete (*i).Animation;
				i = L_Explosions.erase(i);
			}
			else
			{
				(*i).Animation->Update(delta);
				i++;
			}
			hge->Effect_PlayEx(m_Explosion, 100, 0);
		}
		///////////////////////////////////
		if (!L_Blocks.empty()) {
			for (auto x = L_Blocks.begin(); x != L_Blocks.end(); x++) {
				clash(Player1, (*x)->GetBoundingBox());
			}

		}
		if (!L_Blocks.empty()) {
			for (auto x = L_Blocks.begin(); x != L_Blocks.end(); x++) {
				clash(Player2, (*x)->GetBoundingBox());
			}

		}
		/////////////////////////////
		if ((Player1->GetBoundingBox().Intersect(&Player2->GetBoundingBox()))) {
			CreateExplosion(Player1->GetPosition());
			CreateExplosion(Player2->GetPosition());
			Player1->SetPosition(Player1->GetStart());
			Player2->SetPosition(Player2->GetStart());
		}
		///////////////////////////
		for (auto i = Bullets.begin(); i != Bullets.end() && !Bullets.empty();) {
			if ((Player1->GetBoundingBox().Intersect(&(*i)->GetBoundingBox())) && (*i)->GetNumber() != Player1->GetNumber()) {
				Shot_true[(*i)->GetNumber()] = true;
				delete (*i);
				i = Bullets.erase(i);
				CreateExplosion(Player1->GetPosition());
				Player1->SetPosition(Player1->GetStart());
				break;
			}
			else i++;
		}
		/////////////////////////////////
		for (auto i = Bullets.begin(); i != Bullets.end() && !Bullets.empty(); i++) {
			if (((*i)->GetBoundingBox().Intersect(&Base->GetBoundingBox())) ||
				(*i)->GetBoundingBox().Intersect(&Base2->GetBoundingBox())) {
				gameover->b_GameOver = false;
				hge->Channel_Stop(ch_Game2);
				hge->Effect_Play(m_GameOver);
			}
		}
		/////////////////////////////////
		for (auto i = Bullets.begin(); i != Bullets.end() && !Bullets.empty();) {
			if ((Player2->GetBoundingBox().Intersect(&(*i)->GetBoundingBox())) && (*i)->GetNumber() != Player2->GetNumber()) {
				Shot_true[(*i)->GetNumber()] = true;
				delete (*i);
				i = Bullets.erase(i);
				CreateExplosion(Player2->GetPosition());
				Player2->SetPosition(Player2->GetStart());
				break;
			}
			else i++;
		}
		/////////////////////////////////
		if (!L_Lee.empty()) {
			for (auto x = L_Lee.begin(); x != L_Lee.end(); x++) {
				clash(Player1, (*x)->GetBoundingBox());
			}
		}
		/////////////////////////////////////
		if (!L_Lee.empty()) {
			for (auto x = L_Lee.begin(); x != L_Lee.end(); x++) {
				clash(Player2, (*x)->GetBoundingBox());
			}
		}
		/////////////////////////////////////
		clash(Player1, Base->GetBoundingBox());
		clash(Player1, Base2->GetBoundingBox());
		clash(Player2, Base->GetBoundingBox());
		clash(Player2, Base2->GetBoundingBox());
		//////////////////////////////////////////////////
		for (auto x = L_Blocks.begin(); x != L_Blocks.end() && !L_Blocks.empty(); x++) {
			for (auto i = L_Enemy.begin(); i != L_Enemy.end() && !L_Enemy.empty(); i++) {
				clash((*i), (*x)->GetBoundingBox());
			}
		}

		//////////////////////////////////////////////
		for (auto x = L_Lee.begin(); x != L_Lee.end() && !L_Lee.empty(); x++) {
			for (auto i = L_Enemy.begin(); i != L_Enemy.end() && !L_Enemy.empty(); i++) {
				clash((*i), (*x)->GetBoundingBox());
			}
		}


		///////////////////////////////////////////
		for (auto i = L_Enemy.begin(); i != L_Enemy.end() && !L_Enemy.empty();) {
			if ((*i)->GetBoundingBox().Intersect(&Player1->GetBoundingBox())) {
				CreateExplosion((*i)->GetPosition());
				CreateExplosion(Player1->GetPosition());
				Player1->SetPosition(Player1->GetStart());
				(*i)->SetPosition((*i)->Random_Position());
				L_EnemyClose.push_back(*i);
				Block_Lee((*i)->GetPosition());
				i = L_Enemy.erase(i);
			}
			else i++;
		}
		/////////////////////////////////////////////////
		for (auto i = L_Enemy.begin(); i != L_Enemy.end() && !L_Enemy.empty();) {
			if ((*i)->GetBoundingBox().Intersect(&Player2->GetBoundingBox())) {
				CreateExplosion((*i)->GetPosition());
				CreateExplosion(Player2->GetPosition());
				Player2->SetPosition(Player2->GetStart());
				(*i)->SetPosition((*i)->Random_Position());
				L_EnemyClose.push_back(*i);
				Block_Lee((*i)->GetPosition());
				i = L_Enemy.erase(i);
			}
			else i++;
		}
		//////////////////////////////////////////////
		for (auto i = L_Enemy.begin(); i != L_Enemy.end() && !L_Enemy.empty(); i++) {
			clash((*i), Base->GetBoundingBox());
		}
		for (auto i = L_Enemy.begin(); i != L_Enemy.end() && !L_Enemy.empty(); i++) {
			clash((*i), Base2->GetBoundingBox());
		}
		///////////////////////////////////////////////////////
		for (auto x = L_Blocks.begin(); x != L_Blocks.end() && !L_Blocks.empty();) {
			bool incr = false;
			for (auto i = Bullets.begin(); i != Bullets.end() && !Bullets.empty();) {
				if ((*x)->GetBoundingBox().Intersect(&(*i)->GetBoundingBox())) {
					Shot_true[(*i)->GetNumber()] = true;


					if (((*x)->GetEnemy()) && !L_EnemyClose.empty()) {
						for (auto t = L_EnemyClose.begin(); t != L_EnemyClose.end();) {
							if ((hgeVector((*t)->GetPosition().x + size_sprite, (*t)->GetPosition().y) == (*x)->GetPosition()) ||
								(hgeVector((*t)->GetPosition().x - size_sprite, (*t)->GetPosition().y) == (*x)->GetPosition()) ||
								(hgeVector((*t)->GetPosition().x, (*t)->GetPosition().y + size_sprite) == (*x)->GetPosition()) ||
								(hgeVector((*t)->GetPosition().x, (*t)->GetPosition().y - size_sprite) == (*x)->GetPosition()))
							{
								L_Enemy.push_back(*t);
								if ((*i)->GetNumber() == Player1->GetNumber()) {
									Path[(*t)->GetNumber()] = new C_Path(&(*t)->GetPosition(), &Base2->GetPosition(), &L_Blocks);
								}
								else if ((*i)->GetNumber() == Player2->GetNumber()) {
									Path[(*t)->GetNumber()] = new C_Path(&(*t)->GetPosition(), &Base->GetPosition(), &L_Blocks);
								}
								else if (sqrt(abs((*t)->GetPosition().x - Base->GetPosition().x) + abs((*t)->GetPosition().x - Base->GetPosition().x)) >
									sqrt(abs((*t)->GetPosition().x - Base2->GetPosition().x) + abs((*t)->GetPosition().x - Base2->GetPosition().x))) {
									Path[(*t)->GetNumber()] = new C_Path(&(*t)->GetPosition(), &Base2->GetPosition(), &L_Blocks);
								}
								else {
									Path[(*t)->GetNumber()] = new C_Path(&(*t)->GetPosition(), &Base->GetPosition(), &L_Blocks);
								}
								t = L_EnemyClose.erase(t);
							}
							else t++;
						}
					}
					delete (*i);
					i = Bullets.erase(i);
					delete (*x);
					x = L_Blocks.erase(x);
					incr = true;

					break;
				}
				else i++;

			}

			if (!incr) x++;
		}
		//////////////////////////////////////////////////
		for (auto x = L_Enemy.begin(); x != L_Enemy.end() && !L_Enemy.empty();) {
			bool incr = false;
			for (auto i = Bullets.begin(); i != Bullets.end() && !Bullets.empty();) {
				if ((*x)->GetBoundingBox().Intersect(&(*i)->GetBoundingBox()) && (*i)->GetNumber() != (*x)->GetNumber()) {
					Shot_true[(*i)->GetNumber()] = true;
					delete (*i);
					i = Bullets.erase(i);
					CreateExplosion((*x)->GetPosition());
					(*x)->SetPosition((*x)->Random_Position());
					L_EnemyClose.push_back(*x);
					Block_Lee((*x)->GetPosition());
					x = L_Enemy.erase(x);
					incr = true;

					break;
				}
				else i++;

			}

			if (!incr) x++;
		}
		////////////////////////////////////////////////// 
		for (auto x = L_Lee.begin(); x != L_Lee.end() && !L_Lee.empty();) {
			bool incr = false;
			for (auto i = Bullets.begin(); i != Bullets.end() && !Bullets.empty();) {
				if ((*x)->GetBoundingBox().Intersect(&(*i)->GetBoundingBox())) {

					Shot_true[(*i)->GetNumber()] = true;
					delete (*i);
					i = Bullets.erase(i);

					if ((*x)->GetHelth() == 2) {
						delete (*x);
						x = L_Lee.erase(x);
						incr = true;
					}
					else
						(*x)->Update();
					break;
				}
				else i++;

			}
			if (!incr) x++;
		}
	}
	else {
		if (gameover->Position.y < 200) {
			gameover->Position.y += gameover->Step;
		}
	}
	///////////////////////////////////////////

	if (hge->Input_GetKeyState(HGEK_ESCAPE)) {
		CreateMenuPause();
		hge->System_SetState(HGE_FRAMEFUNC, FrameFuncPauseMenu);
		hge->System_SetState(HGE_RENDERFUNC, RenderFuncPauseMenu);
	};
	return false;
}

bool FrameFuncStartMenu()
{
	float dt = hge->Timer_GetDelta();
	int id;
	static int lastid = 0;

	if (hge->Input_GetKeyState(HGEK_ESCAPE)) { lastid = 3; GuiStart->Leave(); }

	id = GuiStart->Update(dt);
	if (id == -1)
	{
		switch (lastid)
		{
		case 1: {
			Game = false;
			GuiStart->SetFocus(true);
			GuiStart->Enter();
			CreateGame();
			hge->System_SetState(HGE_FRAMEFUNC, FrameFuncGame);
			hge->System_SetState(HGE_RENDERFUNC, RenderFuncGame);
			hge->Channel_Stop(ch_Menu);
			break; }
		case 2: {
			Game = true;
			GuiStart->SetFocus(true);
			GuiStart->Enter();
			CreateGame2();
			hge->System_SetState(HGE_FRAMEFUNC, FrameFuncGame2);
			hge->System_SetState(HGE_RENDERFUNC, RenderFuncGame2);
			hge->Channel_Stop(ch_Menu);
			break;
		}

		case 3: return true;
		}
	}
	else if (id) { lastid = id; GuiStart->Leave(); }

	return false;
}
bool RenderFuncStartMenu()
{
	hge->Gfx_BeginScene();
	hge->Gfx_RenderQuad(&quad);
	hge->Gfx_RenderQuad(&quad1);
	hge->Gfx_RenderQuad(&quad2);
	GuiStart->Render();
	fnt->SetColor(0xFFFFFFFF);
	hge->Gfx_EndScene();

	return false;
}

bool FrameFuncPauseMenu() {
	float dt = hge->Timer_GetDelta();
	int id;
	static int lastid = 0;

	id = GuiPause->Update(dt);
	if (id == -1)
	{
		switch (lastid)
		{
		case 1: {
			GuiPause->SetFocus(true);
			GuiPause->Enter();
			if (!Game) {
				CloseGame();
				CreateGame();
				hge->System_SetState(HGE_FRAMEFUNC, FrameFuncGame);
				hge->System_SetState(HGE_RENDERFUNC, RenderFuncGame);
				break;
			}
			else {
				CloseGame2();
				CreateGame2();
				hge->System_SetState(HGE_FRAMEFUNC, FrameFuncGame2);
				hge->System_SetState(HGE_RENDERFUNC, RenderFuncGame2);
				break;
			}
		}
		case 2: {
			GuiPause->SetFocus(true);
			GuiPause->Enter();
			if (!Game) {
				hge->System_SetState(HGE_FRAMEFUNC, FrameFuncGame);
				hge->System_SetState(HGE_RENDERFUNC, RenderFuncGame);
				break;
			}
			else {
				hge->System_SetState(HGE_FRAMEFUNC, FrameFuncGame2);
				hge->System_SetState(HGE_RENDERFUNC, RenderFuncGame2);
				break;
			}
		}
		case 3: {
			if (!Game) {
				CloseGame();
			}
			else {
				CloseGame2();
			}
			CloseMenuPause();
			hge->System_SetState(HGE_FRAMEFUNC, FrameFuncStartMenu);
			hge->System_SetState(HGE_RENDERFUNC, RenderFuncStartMenu);
			ch_Menu = hge->Effect_Play(Menu);
			hge->Channel_SetVolume(ch_Menu, 20);
			break;
		};
		}
	}
	else if (id) { lastid = id; GuiPause->Leave(); }

	return false;
};
bool RenderFuncPauseMenu() {
	hge->Gfx_BeginScene();
	hge->Gfx_RenderQuad(&quad);
	hge->Gfx_RenderQuad(&quad1);
	hge->Gfx_RenderQuad(&quad2);
	GuiPause->Render();
	fnt->SetColor(0xFFFFFFFF);
	hge->Gfx_EndScene();

	return false;
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);
	hge->System_SetState(HGE_FRAMEFUNC, FrameFuncStartMenu);
	hge->System_SetState(HGE_RENDERFUNC, RenderFuncStartMenu);
	hge->System_SetState(HGE_TITLE, "Tanks");
	hge->System_SetState(HGE_FPS, HGEFPS_VSYNC);
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_USESOUND, true);
	hge->System_SetState(HGE_SCREENWIDTH, 800);
	hge->System_SetState(HGE_SCREENHEIGHT, 640);

	if (hge->System_Initiate())
	{
		CreateStartMenu();

		hge->System_Start();

		CloseStartMenu();
	}
	else {

	}

	hge->System_Shutdown();
	hge->Release();
	return 0;
}