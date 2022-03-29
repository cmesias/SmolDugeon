/*
 * gTextAndFonts.cpp
 *
 *  Created on: Mar 8, 2022
 *      Author: healt
 */



#include "TextNFont.h"

void TextNFont::LoadFonts() {

	// Load fonts
	gFont12 = TTF_OpenFont("resource/fonts/PressStart2P.ttf", 12);
	gFont13 = TTF_OpenFont("resource/fonts/PressStart2P.ttf", 13);
	gFont = TTF_OpenFont("resource/fonts/PressStart2P.ttf", 18);
	gFont20 = TTF_OpenFont("resource/fonts/PressStart2P.ttf", 20);
	gFont24 = TTF_OpenFont("resource/fonts/PressStart2P.ttf", 24);
	gFont26 = TTF_OpenFont("resource/fonts/PressStart2P.ttf", 26);
	gFont36 = TTF_OpenFont("resource/fonts/PressStart2P.ttf", 36);
}

void TextNFont::FreeFonts() {

	// Free textures
	gText.free();

	// Free fonts
	TTF_CloseFont(gFont12);
	TTF_CloseFont(gFont13);
	TTF_CloseFont(gFont);
	TTF_CloseFont(gFont20);
	TTF_CloseFont(gFont24);
	TTF_CloseFont(gFont26);
	TTF_CloseFont(gFont36);
	gFont12 = NULL;
	gFont13 = NULL;
	gFont = NULL;
	gFont20 = NULL;
	gFont24 = NULL;
	gFont26 = NULL;
	gFont36 = NULL;
}
