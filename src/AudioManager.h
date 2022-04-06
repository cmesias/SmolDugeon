/*
 * AudioManager.h
 *
 *  Created on: Apr 6, 2022
 *      Author: healt
 */

#ifndef AUDIOMANAGER_H_
#define AUDIOMANAGER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "Helper.h"
#include "LTexture.h"
#include "LWindow.h"
#include "Game/TextNFont.h"
#include "Game/Button.h"
#include "Shortcuts.h"


class AudioManager: public Shortcuts {

public:
	Helper helper;
	TextNFont textNFont;

	enum AudioManagerResult { Back, Exit, Nothing, StartGame, ShowingMenu };
	AudioManagerResult audioManagerResult = Nothing;

	void mousePressed(SDL_Event event);
	AudioManager::AudioManagerResult mouseReleased(SDL_Event event);

public:	// Exclusive for this game
	// Players.cpp
	Mix_Chunk *sCast 			= NULL;
	Mix_Chunk *sSlash 			= NULL;
	Mix_Chunk *sDownStab		= NULL;
	Mix_Chunk *sParry			= NULL;
	Mix_Chunk *sStep			= NULL;
	Mix_Chunk *sDash			= NULL;

	void LoadAudio();
	void FreeAudio();

public:	// Menu

	LTexture gCursor;
	SDL_Event e;
	int mx, my, mex, mey;
	bool leftClick;
	bool pauseLoop;

	/* 0: Audio Settings
	 */
	int settingsIndex = 0;

	// We'll use this to store all the buttons
	/*
	 * 	no Index
	 * 		0: Show Audio button
	 * 		1: Show Video button
	 * 		2: back button
	 * 		3: exit button
	 *
	 * 	0 Index
	 * 		4: Master Volume
	 * 		5: Music Volume
	 * 	    6: SFX Volume
	 *
	 * 	1 Index
	 * 		7: Resolution
	 * 		8: Anti Aliasing
	 * 		9: VSYNC
	 * 		10: Fullscreen
	 *
	 * 	2 Index
	 * 		11: Apply
	 * 		12: Keep
	 * 		13: Revert
	 */
	std::vector<Button*> vButtons;
	unsigned int buttonsSize;
	int btnSpacingH = 80;
	int btnTextXOffset = -10;
	int btnTextYOffset = -40;
	int applyButtonYOffset = 40;
	int keepXOffset = 50;
	int revertXOffset = 40;
	bool promptToKeep = false;
	int promptTimer = 0;

	Button audioBtn;
	Button videoBtn;
	Button backBtn;
	Button exitBtn;

	Button masterVolBtn;	int valueOffset = 290;
	Button musicVolBtn;
	Button sfxVolBtn;

	Button resolutionBtn;
	Button antiAliasBtn;
	Button vsyncBtn;
	Button fullscrenBtn;

	Button applyBtn;
	Button keepBtn;
	Button revertBtn;

	// Save/Load CFG

	// Default directory for audio and video config files
	const std::string defDir = "data/cfg/";

	// Load audio/video menu
	void start(LWindow &gWindow, SDL_Renderer *gRenderer);

	void SaveAudioCFG();

	void LoadAudioCFG();

	void SaveVideoCFG();

	void LoadVideoCFG();

	// TODO [ ] - finish asdding sliders for each button for the user to change the values

	/*
	 *	These values can be changed
	 *  0 masterVolume;
	 *  1 musicVolume;
	 *  2 sfxVolume;
	 *  3 RESOLUTION;
	 *  4 ANTI_ALIAS;
	 *  5 VSYNC;
	 *  6 FULLSCREEN;
	 */
	int option[7];

	int getValue(int findIndex) {
		return option[findIndex];
	}

	int getMasterVolume() {
		return option[0];
	}

	int getMsicVolume() {
		return option[1];
	}

	int getSfxVolume() {
		return option[2];
	}

	int getResolutionValue() {
		return option[3];
	}

	int getAntiAliasValue() {
		return option[4];
	}

	int getVsyncValue() {
		return option[5];
	}

	int getFullScreenValue() {
		return option[6];
	}

	void setVolumes(int masterVolume, int musicVolume, int sfxVolume) {
		option[0] = masterVolume;
		option[1] = musicVolume;
		option[2] = sfxVolume;
	}

	void setVideo(int resolution, int antiAlias, int vsync, int fullscreen) {
		option[3] = resolution;
		option[4] = antiAlias;
		option[5] = vsync;
		option[6] = fullscreen;
	}

};

#endif /* AUDIOMANAGER_H_ */
