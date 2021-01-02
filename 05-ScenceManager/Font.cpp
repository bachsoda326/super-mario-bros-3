#include "Font.h"

CFont::CFont()
{
	CSprites* sprites = CSprites::GetInstance();

	fonts.clear();
	fonts.insert(make_pair('0', sprites->Get(SPRITE_FONT_ID_0)));
	fonts.insert(make_pair('1', sprites->Get(SPRITE_FONT_ID_1)));
	fonts.insert(make_pair('2', sprites->Get(SPRITE_FONT_ID_2)));
	fonts.insert(make_pair('3', sprites->Get(SPRITE_FONT_ID_3)));
	fonts.insert(make_pair('4', sprites->Get(SPRITE_FONT_ID_4)));
	fonts.insert(make_pair('5', sprites->Get(SPRITE_FONT_ID_5)));
	fonts.insert(make_pair('6', sprites->Get(SPRITE_FONT_ID_6)));
	fonts.insert(make_pair('7', sprites->Get(SPRITE_FONT_ID_7)));
	fonts.insert(make_pair('8', sprites->Get(SPRITE_FONT_ID_8)));
	fonts.insert(make_pair('9', sprites->Get(SPRITE_FONT_ID_9)));
	fonts.insert(make_pair('A', sprites->Get(SPRITE_FONT_ID_A)));
	fonts.insert(make_pair('B', sprites->Get(SPRITE_FONT_ID_B)));
	fonts.insert(make_pair('C', sprites->Get(SPRITE_FONT_ID_C)));
	fonts.insert(make_pair('D', sprites->Get(SPRITE_FONT_ID_D)));
	fonts.insert(make_pair('E', sprites->Get(SPRITE_FONT_ID_E)));
	fonts.insert(make_pair('F', sprites->Get(SPRITE_FONT_ID_F)));
	fonts.insert(make_pair('G', sprites->Get(SPRITE_FONT_ID_G)));
	fonts.insert(make_pair('H', sprites->Get(SPRITE_FONT_ID_H)));
	fonts.insert(make_pair('I', sprites->Get(SPRITE_FONT_ID_I)));
	fonts.insert(make_pair('J', sprites->Get(SPRITE_FONT_ID_J)));
	fonts.insert(make_pair('K', sprites->Get(SPRITE_FONT_ID_K)));
	fonts.insert(make_pair('L', sprites->Get(SPRITE_FONT_ID_L)));
	fonts.insert(make_pair('M', sprites->Get(SPRITE_FONT_ID_M)));
	fonts.insert(make_pair('N', sprites->Get(SPRITE_FONT_ID_N)));
	fonts.insert(make_pair('O', sprites->Get(SPRITE_FONT_ID_O)));
	fonts.insert(make_pair('P', sprites->Get(SPRITE_FONT_ID_P)));
	fonts.insert(make_pair('Q', sprites->Get(SPRITE_FONT_ID_Q)));
	fonts.insert(make_pair('R', sprites->Get(SPRITE_FONT_ID_R)));
	fonts.insert(make_pair('S', sprites->Get(SPRITE_FONT_ID_S)));
	fonts.insert(make_pair('T', sprites->Get(SPRITE_FONT_ID_T)));
	fonts.insert(make_pair('U', sprites->Get(SPRITE_FONT_ID_U)));
	fonts.insert(make_pair('V', sprites->Get(SPRITE_FONT_ID_V)));
	fonts.insert(make_pair('W', sprites->Get(SPRITE_FONT_ID_W)));
	fonts.insert(make_pair('X', sprites->Get(SPRITE_FONT_ID_X)));
	fonts.insert(make_pair('Y', sprites->Get(SPRITE_FONT_ID_Y)));
	fonts.insert(make_pair('Z', sprites->Get(SPRITE_FONT_ID_Z)));
}

LPSPRITE CFont::ToSprite(char c)
{
	if (!('0' <= c && c <= '9') && !('A' <= c && c <= 'Z')) 
		return NULL;
	return fonts.at(c);
}

vector<LPSPRITE> CFont::ToSprites(string s)
{
	vector<LPSPRITE> sprites;
	LPSPRITE sprite;

	for (int i = 0; i < s.size(); i++)
	{
		sprite = ToSprite(s[i]);
		if (sprite != NULL)
			sprites.push_back(sprite);
	}

	return sprites;
}
