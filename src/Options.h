/*
 * Options.h
 *
 *  Created on: May 24, 2017
 *      Author: Carl
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_


#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "LTexture.h"
#include "LWindow.h"

class Options {

public:	// Exclusive for this game

	// Mouse cursor
	LTexture gCursor;


public:	// functions

	// Save a level to a directory
	/*void SaveLevel(LWindow &gWindow, SDL_Renderer *gRenderer, bool &mainLoop,
				   std::string SpawnCoordinatesData,
				   std::string TileSaveData,
				   std::string ItemSaveData,
				   std::string MonsterSaveData);*/

};

#endif /* OPTIONS_H_ */
