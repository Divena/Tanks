#include "menuitem.h"

const int size_sprite = 32;


Ñ_hgeGUIMenuItem::Ñ_hgeGUIMenuItem(int _id, hgeFont *_fnt,
	HEFFECT _snd, float _x, float _y,
	float _delay, char *_title)
{
	id = _id;
	fnt = _fnt;
	snd = _snd;
	delay = _delay;
	title = _title;

	color.SetHWColor(0xFF000000);
	shadow.SetHWColor(0x30000000);
	offset = 0.0f; timer = -1.0f; timer2 = -1.0f;

	bStatic = false; bVisible = true; bEnabled = true;

	float w = fnt->GetStringWidth(title);
	rect.Set(_x - w / 2, _y, _x + w / 2, _y + fnt->GetHeight());
}

void Ñ_hgeGUIMenuItem::Render()
{
	fnt->SetColor(shadow.GetHWColor());
	fnt->Render(rect.x1 + offset + 3, rect.y1 + 3, HGETEXT_LEFT, title);
	fnt->SetColor(color.GetHWColor());
	fnt->Render(rect.x1 - offset, rect.y1 - offset, HGETEXT_LEFT, title);
}

void Ñ_hgeGUIMenuItem::Update(float dt)
{
	if (timer2 != -1.0f)
	{
		timer2 += dt;
		if (timer2 >= delay + 0.1f)
		{
			color = scolor2 + dcolor2;
			shadow = sshadow + dshadow;
			offset = 0.0f;
			timer2 = -1.0f;
		}
		else
		{
			if (timer2 < delay) { color = scolor2; shadow = sshadow; }
			else {
				color = scolor2 + dcolor2*(timer2 - delay) * 10;
				shadow = sshadow + dshadow*(timer2 - delay) * 10;
			}
		}
	}
	else if (timer != -1.0f)
	{
		timer += dt;
		if (timer >= 0.2f)
		{
			color = scolor + dcolor;
			offset = soffset + doffset;
			timer = -1.0f;
		}
		else
		{
			color = scolor + dcolor*timer * 5;
			offset = soffset + doffset*timer * 5;
		}
	}
}

void Ñ_hgeGUIMenuItem::Enter()
{
	hgeColor tcolor2;

	scolor2.SetHWColor(0x00000000);
	tcolor2.SetHWColor(0xFF000000);
	dcolor2 = tcolor2 - scolor2;

	sshadow.SetHWColor(0x00000000);
	tcolor2.SetHWColor(0x30000000);
	dshadow = tcolor2 - sshadow;

	timer2 = 0.0f;
}

void Ñ_hgeGUIMenuItem::Leave()
{
	hgeColor tcolor2;

	scolor2.SetHWColor(0xFF000000);
	tcolor2.SetHWColor(0x00000000);
	dcolor2 = tcolor2 - scolor2;

	sshadow.SetHWColor(0x30000000);
	tcolor2.SetHWColor(0x00000000);
	dshadow = tcolor2 - sshadow;

	timer2 = 0.0f;
}

bool Ñ_hgeGUIMenuItem::IsDone()
{
	if (timer2 == -1.0f) return true;
	else return false;
}

void Ñ_hgeGUIMenuItem::Focus(bool bFocused)
{
	hgeColor tcolor;

	if (bFocused)
	{
		hge->Effect_Play(snd);
		scolor.SetHWColor(0xFF000000);
		tcolor.SetHWColor(0xFF990000);
		soffset = 0;
		doffset = 4;
	}
	else
	{
		scolor.SetHWColor(0xFF990000);
		tcolor.SetHWColor(0xFF000000);
		soffset = 4;
		doffset = -4;
	}

	dcolor = tcolor - scolor;
	timer = 0.0f;
}

void Ñ_hgeGUIMenuItem::MouseOver(bool bOver)
{
	if (bOver) gui->SetFocus(id);
}

bool Ñ_hgeGUIMenuItem::MouseLButton(bool bDown)
{
	if (!bDown)
	{
		offset = 4;
		return true;
	}
	else
	{
		hge->Effect_Play(snd);
		offset = 0;
		return false;
	}
}

bool Ñ_hgeGUIMenuItem::KeyClick(int key, int chr)
{
	if (key == HGEK_ENTER || key == HGEK_SPACE)
	{
		MouseLButton(true);
		return MouseLButton(false);
	}

	return false;
}
