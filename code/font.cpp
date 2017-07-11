
#define STB_TRUETYPE_IMPLEMENTATION
#include "../stb_truetype.h"

#define DEFAULT_FONT_SIZE 16.0f
#define FONT_MEGA_TEXTURE_WIDTH 512
#define FONT_MEGA_TEXTURE_HEIGHT 512

//struct asset_font
//{
//	stbtt_fontinfo STBFontInfo;
//	//asset_id assetID;
//	int Ascent;
//	int Descent;
//	int LineGap;
//};

struct FontCache {
	//asset_id assetID; // Probably can remove this, erm actually not
	int size;
	float glyphScale;
	float pixelGlyphScale;

	stbtt_fontinfo STBFontInfo;
	int Ascent;
	int Descent;
	int LineGap;

	struct {
		bool loaded;

		float2 bitmapOffset;
		float2 bitmapSize;
		int advanceWidth;
		int leftSideBearing;
		float2 megaTextureOffset;
		float2 megaTextureSize;
	} glyphs[0xFF];
};

struct Font_Mega_Texture {
	GLuint id;
	float2 writeOffset;
	float currentRowOffset;
};

FontCache fonts[256] = {};
int font_count = 0;

Font_Mega_Texture fontMegaTexture;

void init_font_system() {
	glGenTextures(1, &fontMegaTexture.id);
	glBindTexture(GL_TEXTURE_2D, fontMegaTexture.id);
	//unsigned char *data = (unsigned char*)gjPushMemStack(&RState->memStack, 1024*1024*4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FONT_MEGA_TEXTURE_WIDTH, FONT_MEGA_TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

int LoadFont(char *File, float scale)
{
	FontCache *font = &fonts[font_count];
	// Font.BitmapScale = 1.0f;
	// Font.RenderScale = 1.0f / Font.BitmapScale;

	// FileResult f = load_file(File);
	FileResult f = load_universal_file(File);

	if (f.data) {
		stbtt_InitFont(&font->STBFontInfo, (unsigned char*)f.data, stbtt_GetFontOffsetForIndex((unsigned char*)f.data, 0));
		stbtt_GetFontVMetrics(&font->STBFontInfo, &font->Ascent, &font->Descent, &font->LineGap);
		
		//fontCache->assetID = font->assetID;
		float size = DEFAULT_FONT_SIZE * scale;
		font->glyphScale = stbtt_ScaleForPixelHeight(&font->STBFontInfo, size);
		//size *= renderState->viewportScale;
		font->size = (int)size;
		font->pixelGlyphScale = stbtt_ScaleForPixelHeight(&font->STBFontInfo, size);
	} else {
		return -1;
	}

	return font_count++;
}

//Font_Cache *cacheFont(render_state *renderState, asset_font *font, float scale) {
//	Font_Cache *fontCache = null;
//	fiz(ArraySize(renderState->fontCache)) {
//		if (renderState->fontCache[i].assetID == UNKNOWN_ASSET) {
//			fontCache = &renderState->fontCache[i];
//			break;
//		}
//	}
//
//	if (fontCache) {
//		fontCache->assetID = font->assetID;
//		float size = DEFAULT_FONT_POINT_SIZE * scale;
//		fontCache->glyphScale = stbtt_ScaleForPixelHeight(&font->STBFontInfo, size);
//		size *= renderState->viewportScale;
//		fontCache->size = (int)size;
//		fontCache->pixelGlyphScale = stbtt_ScaleForPixelHeight(&font->STBFontInfo, size);
//	}
//
//	return fontCache;
//}

void LoadFontGlyph(FontCache *font, char GlyphIndex) {
	auto *glyph = &font->glyphs[GlyphIndex];
	glyph->loaded = true;

	stbtt_GetCodepointHMetrics(&font->STBFontInfo, GlyphIndex, &glyph->advanceWidth, &glyph->leftSideBearing);

	int2 BitmapSize;
	int2 BitmapOffset;
	// TODO: Use subpixel rasterizer
	unsigned char *FontBitmap = stbtt_GetCodepointBitmap(&font->STBFontInfo, 0, font->pixelGlyphScale, GlyphIndex,
		&BitmapSize.x, &BitmapSize.y, &BitmapOffset.x, &BitmapOffset.y);

	glyph->bitmapSize = {(float)BitmapSize.x, (float)BitmapSize.y};
	glyph->bitmapOffset = {(float)BitmapOffset.x, (float)BitmapOffset.y};
	// Glyph->Size = BitmapSize;
	// Glyph->Offset = BitmapOffset;

	size_t BitmapMemorySize = BitmapSize.x*BitmapSize.y*4;
	uint8 *Bitmap = (uint8*)malloc(BitmapMemorySize);//PushMemory(&renderState->Assets->memStack, BitmapMemorySize);
	for (uint32 BI = 0;
		BI < BitmapMemorySize;
		BI += 4)
	{
		Bitmap[BI] = 0xFF;
		Bitmap[BI+1] = 0xFF;
		Bitmap[BI+2] = 0xFF;
		Bitmap[BI+3] = FontBitmap[BI/4];
	}

	stbtt_FreeBitmap(FontBitmap, 0);

#if 0
	glGenTextures(1, &Glyph->tid);
	glBindTexture(GL_TEXTURE_2D, Glyph->tid);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		Glyph->Size.x,
		Glyph->Size.y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		(uint8*)Bitmap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#endif

	if (fontMegaTexture.writeOffset.x + BitmapSize.x + 2 > FONT_MEGA_TEXTURE_WIDTH) {
		fontMegaTexture.writeOffset.x = 0.0f;
		fontMegaTexture.writeOffset.y = fontMegaTexture.currentRowOffset + 1;
	}

	float2 writePos = {fontMegaTexture.writeOffset.x + 1, fontMegaTexture.writeOffset.y + 1};

	glBindTexture(GL_TEXTURE_2D, fontMegaTexture.id);
	glTexSubImage2D(GL_TEXTURE_2D, 0,
		writePos.x, writePos.y,
		BitmapSize.x, BitmapSize.y, GL_RGBA, GL_UNSIGNED_BYTE, Bitmap);

	glyph->megaTextureOffset = {writePos.x / FONT_MEGA_TEXTURE_WIDTH,
		writePos.y / FONT_MEGA_TEXTURE_HEIGHT};
	glyph->megaTextureSize = {glyph->bitmapSize.x / FONT_MEGA_TEXTURE_WIDTH,
		glyph->bitmapSize.y / FONT_MEGA_TEXTURE_HEIGHT};

	fontMegaTexture.writeOffset.x += BitmapSize.x + 1;
	if (fontMegaTexture.writeOffset.y + BitmapSize.y > fontMegaTexture.currentRowOffset) {
		fontMegaTexture.currentRowOffset = fontMegaTexture.writeOffset.y + BitmapSize.y;
	}

	//PopMemory(&renderState->Assets->memStack, BitmapMemorySize);
	free(Bitmap);

	/*glyph->bitmapSize /= renderState->viewportScale;
	glyph->bitmapOffset /= renderState->viewportScale;*/
}


float Font_GetAdvance(FontCache *font, char a, char b, float2 s)
{
	float Kern = 0.0f;
	if (b)
	{
		Kern = (float)stbtt_GetCodepointKernAdvance(&font->STBFontInfo, a, b);
	}

	float GlyphAdvance = (float)font->glyphs[a].advanceWidth;
	float Result = ((GlyphAdvance * font->glyphScale) + (Kern * font->glyphScale)) * s.x;
	return Result;
}

float2 GetTextDim(int fontid, char *str, float2 s, float widthLimit)
{
	/*asset_font *Font = GetFont(rstate->platform, rstate->Assets, assetID);
	Font_Cache *fontCache = getFontCache(rstate->platform, rstate, assetID, s.y);*/
	if (fontid == -1) return{};
	FontCache *Font = &fonts[fontid];

	float2 dim = {};

	if (Font) {
		// u8 *P = (uint8*)str;
		int Index = 0;
		char LastChar = 0;

		float Advance = 0.0f;
		float StartOffset = (Font->Ascent-Font->Descent) * Font->glyphScale * s.y /** Font->RenderScale*/;
		float RowOffset = StartOffset;
		float biggest_advance = 0.0f;

		while (*str)
		{
			if (*str == '\n')
			{
				RowOffset += (Font->Ascent-Font->Descent) * Font->glyphScale * s.y /** Font->RenderScale*/;
				// ++RowsRendered;

				LastChar = *str;
				++str;
				++Index;
				Advance = 0.0f;
				continue;
			}

			if (widthLimit > 0.0f)
			{
				if (*str == ' ')
				{
					uint32 SearchIndex = 1;
					float SearchAdvance = 0.0f;
					while (str[SearchIndex] != ' ' && str[SearchIndex] != '\n' && str[SearchIndex] != NULL)
					{
						SearchAdvance += Font_GetAdvance(Font, str[SearchIndex-1], str[SearchIndex], s);
						++SearchIndex;
					}

					if (Advance+SearchAdvance > widthLimit-10.0f)
					{
						RowOffset += (Font->Ascent-Font->Descent) * Font->glyphScale * s.y /** Font->RenderScale*/;
						// ++RowsRendered;

						Advance = 0.0f;
						LastChar = *str;
						++str;
						++Index;
						continue;
					}
				}
			}

			if (!Font->glyphs[*str].loaded) {
				LoadFontGlyph(Font, *str);
			}

			float Kern = 0.0f;
			if (*(str+1)) {
				Kern = (float)stbtt_GetCodepointKernAdvance(&Font->STBFontInfo, *str, *(str+1));
			}

			float GlyphAdvance = (float)Font->glyphs[*str].advanceWidth;
			Advance += ((GlyphAdvance * Font->glyphScale) + (Kern * Font->glyphScale)) * s.x /** Font->RenderScale*/;
			if (Advance > biggest_advance) biggest_advance = Advance;

			LastChar = *str;
			++str;
			++Index;
		}

		dim.x = biggest_advance;
		dim.y = RowOffset;
	}

	return dim;
}

// int fontid, char *str, float2 s, float widthLimit
void _PushFont(int fontid, char *Text, float3 p, float2 s, float4 c, float BoundingBoxX = 0.0f, float *RenderedHeight = NULL)
{
	/*asset_font *Font = GetFont(rstate->platform, rstate->Assets, assetID);
	Font_Cache *fontCache = getFontCache(rstate->platform, rstate, assetID, s.y);*/
	if (fontid == -1) return;
	FontCache *Font = &fonts[fontid];

	s = {1.0f, 1.0f};

	int glyphCount = strlen(Text);
	//Glyph_Render_Command *glyphs = (Glyph_Render_Command*)gjPushMemStack(&RState->memStack, sizeof(Glyph_Render_Command)*glyphCount, true);

	// PushFontCommand(RState, AssetID, p, s * textDimensions, RState->fontTempTexture, WorldSpace);

	uint8 *P = (uint8*)Text;
	uint32 Index = 0;
	uint8 LastChar = 0;
	float Advance = 0.0f;

	uint32 RowsRendered = 0;

	float StartOffset = (Font->Ascent-Font->Descent) * Font->glyphScale * s.y /** Font->RenderScale*/;
	float RowOffset = StartOffset;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fontMegaTexture.id);
	//glColor4f(c.r, c.g, c.b, c.a);

	while (*P)
	{
		if (*P == '\n')
		{
			RowOffset += (Font->Ascent-Font->Descent) * Font->glyphScale * s.y /** Font->RenderScale*/;
			++RowsRendered;

			LastChar = *P;
			++P;
			++Index;
			Advance = 0.0f;
			continue;
		}

		if (BoundingBoxX > 0.0f)
		{
			if (*P == ' ')
			{
				uint32 SearchIndex = 1;
				float SearchAdvance = 0.0f;
				while (P[SearchIndex] != ' ' && P[SearchIndex] != '\n' && P[SearchIndex] != NULL)
				{
					SearchAdvance += Font_GetAdvance(Font, P[SearchIndex-1], P[SearchIndex], s);
					++SearchIndex;
				}
				//SearchAdvance += Font_GetAdvance(Font, P[SearchIndex-1], P[SearchIndex]);

				if (Advance+SearchAdvance > BoundingBoxX-10.0f)
				{
					RowOffset += (Font->Ascent-Font->Descent) * Font->glyphScale * s.y /** Font->RenderScale*/;
					++RowsRendered;

					Advance = 0.0f;
					LastChar = *P;
					++P;
					++Index;
					continue;
				}
			}
		}

		auto *glyph = &Font->glyphs[*P];
		if (!glyph->loaded) {
			LoadFontGlyph(Font, *P);
		}

		// xoff/yoff are the offset it pixel space from the glyph origin to the top-left of the bitmap
		// leftSideBearing is the offset from the current horizontal position to the left edge of the character
		// advanceWidth is the offset from the current horizontal position to the next horizontal position
		// these are expressed in unscaled coordinates

		float2 Offset = {Font->glyphs[*P].bitmapOffset.x * s.x, Font->glyphs[*P].bitmapOffset.y * s.y} /** Font->RenderScale*/;
		float GlyphSideBearing = (float)Font->glyphs[*P].leftSideBearing * Font->glyphScale * s.x /** Font->RenderScale*/;
		//Advance += GlyphSideBearing ;
		float XPos = p.x+Advance;
		//XPos += XOffset;
		XPos += GlyphSideBearing;

		//auto Glyph = fontCache->glyphs[*P];
		//glyphs[Index].textureOffset = Glyph.megaTextureOffset;
		//glyphs[Index].textureSize = Glyph.megaTextureSize;
		//glyphs[Index].pos = {Advance+GlyphSideBearing, Offset.y + RowOffset};
		//glyphs[Index].size = /*Glyph.Size*/Glyph.bitmapSize;

		{
			glPushMatrix();
			float3 pos = p;
			pos.x += Advance+GlyphSideBearing;
			pos.y += Offset.y + RowOffset;
			glTranslatef(pos.x, pos.y, pos.z);
			glBegin(GL_QUADS);
			{
				glTexCoord2f(glyph->megaTextureOffset.x, glyph->megaTextureOffset.y); glVertex3f(0.0f, 0.0f, 0.0f);
				glTexCoord2f(glyph->megaTextureOffset.x+glyph->megaTextureSize.x, glyph->megaTextureOffset.y); glVertex3f(glyph->bitmapSize.x, 0.0f, 0.0f);
				glTexCoord2f(glyph->megaTextureOffset.x+glyph->megaTextureSize.x, glyph->megaTextureOffset.y+glyph->megaTextureSize.y); glVertex3f(glyph->bitmapSize.x, glyph->bitmapSize.y, 0.0f);
				glTexCoord2f(glyph->megaTextureOffset.x, glyph->megaTextureOffset.y+glyph->megaTextureSize.y); glVertex3f(0.0f, glyph->bitmapSize.y, 0.0f);
			}
			glEnd();
			glPopMatrix();
		}

		// DrawGlyphToTexture(RState, {Advance+GlyphSideBearing, Offset.y + RowOffset, p.z}, Glyph.tid, Glyph.Size);
		// PushFontGlyph(RState, AssetID, *P, {XPos, p.y+Offset.y + RowOffset, p.z}, s, c, WorldSpace);

		float Kern = 0.0f;
		if (*(P+1))
		{
			Kern = (float)stbtt_GetCodepointKernAdvance(&Font->STBFontInfo, *P, *(P+1));
		}

		float GlyphAdvance = (float)Font->glyphs[*P].advanceWidth;
		Advance += ((GlyphAdvance * Font->glyphScale) + (Kern * Font->glyphScale)) * s.x /** Font->RenderScale*/;

		// int32 Adv = Font->Glyphs[*P].AdvanceWidth;

		LastChar = *P;
		++P;
		++Index;
	}

	glDisable(GL_TEXTURE_2D);

	// RowOffset += (Font->Ascent-Font->Descent) * Font->GlyphScale * s.y;
	if (RenderedHeight)
	{
		*RenderedHeight = RowOffset;
	}

	//PushFontCommand(RState, AssetID, p, s, c, glyphs, glyphCount, WorldSpace);
}

int lua_draw_font(lua_State* l) {
	int fontid = lua_tonumber(l, 1);
	char *str = (char*)lua_tostring(l, 2);
	float x = lua_tonumber(l, 3);
	float y = lua_tonumber(l, 4);
	float width = lua_tonumber(l, 5);

	_PushFont(fontid, str, {x, y, 0}, {1, 1}, {1, 1, 1, 1}, width);

	return 0;
}

int lua_font_dimensions(lua_State* l) {
	int fontid = lua_tonumber(l, 1);
	char *str = (char*)lua_tostring(l, 2);
	float width = lua_tonumber(l, 3);

	float2 dim = GetTextDim(fontid, str, {1, 1}, width);
	lua_pushnumber(l, dim.x);
	lua_pushnumber(l, dim.y);

	return 2;
}

int lua_load_font(lua_State* l) {
	char *file = (char*)lua_tostring(l, 1);
	float size = lua_tonumber(l, 2);

	int font = LoadFont(file, size);
	lua_pushnumber(l, font);

	return 1;
}