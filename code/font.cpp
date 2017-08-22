
#define STB_TRUETYPE_IMPLEMENTATION
#include "../stb_truetype.h"

#define DEFAULT_FONT_SIZE 16.0f
//#define FONT_MEGA_TEXTURE_WIDTH 512
//#define FONT_MEGA_TEXTURE_HEIGHT 512
#define FONT_MEGA_TEXTURE_WIDTH 1024
#define FONT_MEGA_TEXTURE_HEIGHT 1024

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
	float size;
	float glyphScale;
	float pixelGlyphScale;
	char file[256];

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

	int loaded;
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
	unsigned char *mem = (unsigned char*)malloc(4 * FONT_MEGA_TEXTURE_WIDTH * FONT_MEGA_TEXTURE_HEIGHT);
	memset(mem, 0, 4 * FONT_MEGA_TEXTURE_WIDTH * FONT_MEGA_TEXTURE_HEIGHT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FONT_MEGA_TEXTURE_WIDTH, FONT_MEGA_TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, mem);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// float4 color = {1.0f, 0.0f, 0.0f, 1.0f};
	// glClearTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);
}

int LoadFont(char *File, float scale) {
	float s = DEFAULT_FONT_SIZE * scale;
	for (int i = 0; i < font_count; ++i) {
		if (strcmp(File, fonts[i].file)==0 &&
			s > fonts[i].size-0.1f && s < fonts[i].size+0.1f) {
			return i;
		}
	}

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
		font->size = size;
		font->pixelGlyphScale = stbtt_ScaleForPixelHeight(&font->STBFontInfo, size);

		strcpy(font->file, File);

		printf("font %s %f\n", File, scale);
		// printf("ascent %i\n", font->Ascent);
		// printf("descent %i\n", font->Descent);
		// printf("linegap %i\n", font->LineGap);

		atomic_swap32(&font->loaded, true);
	} else {
		return -1;
	}

	return font_count++;
}

int LoadLocalFont(char *File, float scale)
{
	FontCache *font = &fonts[font_count];
	// Font.BitmapScale = 1.0f;
	// Font.RenderScale = 1.0f / Font.BitmapScale;

	// FileResult f = load_file(File);
	FileResult f = load_file(File);

	if (f.data) {
		stbtt_InitFont(&font->STBFontInfo, (unsigned char*)f.data, stbtt_GetFontOffsetForIndex((unsigned char*)f.data, 0));
		stbtt_GetFontVMetrics(&font->STBFontInfo, &font->Ascent, &font->Descent, &font->LineGap);
		
		//fontCache->assetID = font->assetID;
		float size = DEFAULT_FONT_SIZE * scale;
		font->glyphScale = stbtt_ScaleForPixelHeight(&font->STBFontInfo, size);
		//size *= renderState->viewportScale;
		font->size = (int)size;
		font->pixelGlyphScale = stbtt_ScaleForPixelHeight(&font->STBFontInfo, size);

		strcpy(font->file, File);

		printf("font %s\n", File);
		// printf("ascent %i\n", font->Ascent);
		// printf("descent %i\n", font->Descent);
		// printf("linegap %i\n", font->LineGap);

		atomic_swap32(&font->loaded, true);
	} else {
		return -1;
	}

	return font_count++;
}

FontCache *GetFontCache(char *font_file, float size) {
	float s = DEFAULT_FONT_SIZE * size;
	for (int i = 0; i < font_count; ++i) {
		if (strcmp(font_file, fonts[i].file)==0 &&
			s > fonts[i].size-0.1f && s < fonts[i].size+0.1f) {
			return &fonts[i];
		}
	}

	LunaEvent e = {};
	e.type = EVENT_LOAD_FONT;
	e.draw.file = font_file;
	e.draw.scale = size;
	command_queue.push_event(e);
	return NULL;
	// return &fonts[LoadFont(font_file, size)];
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
	//printf("loading glyph %c\n", GlyphIndex);

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


float Font_GetAdvance(FontCache *font, char a, char b, float size)
{
	float Kern = 0.0f;
	if (b)
	{
		Kern = (float)stbtt_GetCodepointKernAdvance(&font->STBFontInfo, a, b);
	}

	int GlyphAdvance;// = (float)font->glyphs[a].advanceWidth;
	stbtt_GetCodepointHMetrics(&font->STBFontInfo, a, &GlyphAdvance, NULL);
	float Result = (((float)GlyphAdvance * font->glyphScale) + (Kern * font->glyphScale)) * size;
	return Result;
}

float2 GetTextDim(char *font_file, char *str, float size, float widthLimit)
{
	/*asset_font *Font = GetFont(rstate->platform, rstate->Assets, assetID);
	Font_Cache *fontCache = getFontCache(rstate->platform, rstate, assetID, s.y);*/

	// FontCache *Font = &fonts[fontid];
	FontCache *Font = GetFontCache(font_file, size);
	if (!Font) return {};
	if (!atomic_fetch32(&Font->loaded)) return {};

	size = 1.0f;

	float2 dim = {};

	if (Font) {
		// u8 *P = (uint8*)str;
		int Index = 0;
		char LastChar = 0;

		float Advance = 0.0f;
		float StartOffset = (Font->Ascent-Font->Descent) * Font->glyphScale * size /** Font->RenderScale*/;
		float RowOffset = StartOffset;
		float biggest_advance = 0.0f;

		while (*str)
		{
			if (*str == '\n')
			{
				RowOffset += (Font->Ascent-Font->Descent) * Font->glyphScale * size /** Font->RenderScale*/;
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
					while (str[SearchIndex] != ' ' && str[SearchIndex] != '\n' && str[SearchIndex] != 0)
					{
						SearchAdvance += Font_GetAdvance(Font, str[SearchIndex-1], str[SearchIndex], size);
						++SearchIndex;
					}

					if (Advance+SearchAdvance > widthLimit-10.0f)
					{
						RowOffset += (Font->Ascent-Font->Descent) * Font->glyphScale * size;

						// ++RowsRendered;

						Advance = 0.0f;
						LastChar = *str;
						++str;
						++Index;
						continue;
					}
				}
			}

			// if (!Font->glyphs[*str].loaded) {
			// 	LoadFontGlyph(Font, *str);
			// }

			// float Kern = 0.0f;
			// if (*(str+1)) {
			// 	Kern = (float)stbtt_GetCodepointKernAdvance(&Font->STBFontInfo, *str, *(str+1));
			// }

			// float GlyphAdvance = (float)Font->glyphs[*str].advanceWidth;
			// Advance += ((GlyphAdvance * Font->glyphScale) + (Kern * Font->glyphScale)) * size /** Font->RenderScale*/;
			Advance += Font_GetAdvance(Font, *str, *(str+1), size);

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
void draw_font(char *font_file, float size, char *Text, float3 p, float BoundingBoxX = 0.0f)
{
	/*asset_font *Font = GetFont(rstate->platform, rstate->Assets, assetID);
	Font_Cache *fontCache = getFontCache(rstate->platform, rstate, assetID, s.y);*/

	// FontCache *Font = &fonts[fontid];
	FontCache *Font = GetFontCache(font_file, size);
	if (!Font) return;
	if (!atomic_fetch32(&Font->loaded)) return;
	free(font_file);

	size = 1.0f;

	int glyphCount = strlen(Text);
	//Glyph_Render_Command *glyphs = (Glyph_Render_Command*)gjPushMemStack(&RState->memStack, sizeof(Glyph_Render_Command)*glyphCount, true);

	// PushFontCommand(RState, AssetID, p, s * textDimensions, RState->fontTempTexture, WorldSpace);

	uint8 *P = (uint8*)Text;
	uint32 Index = 0;
	uint8 LastChar = 0;
	float Advance = 0.0f;

	uint32 RowsRendered = 0;

	float StartOffset = (Font->Ascent-Font->Descent) * Font->glyphScale * size /** Font->RenderScale*/;
	float RowOffset = StartOffset;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fontMegaTexture.id);
	//glColor4f(c.r, c.g, c.b, c.a);

	while (*P)
	{
		if (*P == '\n')
		{
			RowOffset += (Font->Ascent-Font->Descent) * Font->glyphScale * size /** Font->RenderScale*/;
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
				while (P[SearchIndex] != ' ' && P[SearchIndex] != '\n' && P[SearchIndex] != 0)
				{
					SearchAdvance += Font_GetAdvance(Font, P[SearchIndex-1], P[SearchIndex], size);
					++SearchIndex;
				}
				//SearchAdvance += Font_GetAdvance(Font, P[SearchIndex-1], P[SearchIndex]);

				if (Advance+SearchAdvance > BoundingBoxX-10.0f)
				{
					RowOffset += (Font->Ascent-Font->Descent) * Font->glyphScale * size /** Font->RenderScale*/;
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

		float2 Offset = {Font->glyphs[*P].bitmapOffset.x * size, Font->glyphs[*P].bitmapOffset.y * size} /** Font->RenderScale*/;
		float GlyphSideBearing = (float)Font->glyphs[*P].leftSideBearing * Font->glyphScale * size /** Font->RenderScale*/;
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
		Advance += ((GlyphAdvance * Font->glyphScale) + (Kern * Font->glyphScale)) * size /** Font->RenderScale*/;

		// int32 Adv = Font->Glyphs[*P].AdvanceWidth;

		LastChar = *P;
		++P;
		++Index;
	}

	glDisable(GL_TEXTURE_2D);

	// RowOffset += (Font->Ascent-Font->Descent) * Font->GlyphScale * s.y;
	// if (RenderedHeight)
	// {
	// 	*RenderedHeight = RowOffset;
	// }

	//PushFontCommand(RState, AssetID, p, s, c, glyphs, glyphCount, WorldSpace);
}