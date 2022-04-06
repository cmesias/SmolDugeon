/*
 * AudioManager.cpp
 *
 *  Created on: Apr 6, 2022
 *      Author: healt
 */

#include "AudioManager.h"


// TODO [ ]  - get rid of old options thing, and put this in PlayGame

void AudioManager::SaveAudioCFG()
{
	// open config file
	std::stringstream tempss;

	// set default directory
	tempss << defDir;

	// set filename
	tempss << "audio.cfg";
	std::ofstream fileSettings( tempss.str().c_str() );
	if (fileSettings.is_open()) {
		fileSettings << getMasterVolume() << " " << getMsicVolume() << " " << getSfxVolume();
	}
	fileSettings.close();
}

void AudioManager::SaveVideoCFG()
{
	// open config file
	std::stringstream tempss;

	// set default directory
	tempss << defDir;

	// set filename
	tempss << "video.cfg";
	std::ofstream fileSettings( tempss.str().c_str() );
	if (fileSettings.is_open()) {
		fileSettings <<  getResolutionValue() << " " << getAntiAliasValue() << " " << getVsyncValue()  << " " << getFullScreenValue();
	}
	fileSettings.close();
}

void AudioManager::LoadAudioCFG(){
	// open config file
	std::stringstream tempss;

	// set default directory
	tempss << defDir;

	// set filename
	tempss << "audio.cfg";
	std::fstream fileSettings( tempss.str().c_str() );

	int tempMasterVol = -1;
	int tempMusicVol = -2;
	int tempSfxVol = -3;

	// File opened
	if (fileSettings.is_open()) {
		fileSettings >>  tempMasterVol >> tempMusicVol >> tempSfxVol;
		//std::cout << "Loaded audio.cfg" << std::endl;

		// Set audio values from loaded file
		setVolumes(tempMasterVol, tempMusicVol, tempSfxVol);
	}

	// Unable to open file, create new file
	else{
		//std::cout << "Error opening audio.cfg." << std::endl;
		//std::cout << "\tCreating default audio.cfg..." << std::endl;
		std::ofstream newConfigFile("cfg/audio.cfg");
		if (newConfigFile.is_open()) {
			//std::cout << "\t\tSuccessfully created audio.cfg." << std::endl;
			newConfigFile << "100 128 128";
		}

		// Opening and creating default config file failed, set Settings manually
		else{
			//std::cout << "\t\tError creating default audio.cfg." << std::endl;
			//std::cout << "\t\t\tManually setting default audio.cfg settings." << std::endl;
			setVolumes(100, 128, 128);
		}
		newConfigFile.close();
	}
	fileSettings.close();
}

void AudioManager::LoadVideoCFG(){
	// open config file
	std::stringstream tempss;

	// set default directory
	tempss << defDir;

	// set filename
	tempss << "video.cfg";
	std::fstream fileSettings( tempss.str().c_str() );

	int tempReso = -4;
	int tempAnti = -5;
	int tempVsync = -6;
	int tempFull = -7;

	if (fileSettings.is_open())
	{
		fileSettings >>  tempReso >> tempAnti >> tempVsync >> tempFull;
		//std::cout << "Loaded video.cfg" << std::endl;

		// Set video values from loaded file
		setVideo(tempReso, tempAnti, tempVsync, tempFull);
	}

	else {
		//std::cout << "Error opening video.cfg." << std::endl;
		//std::cout << "\tCreating default video.cfg..." << std::endl;
		std::ofstream newConfigFile("cfg/video.cfg");
		if (newConfigFile.is_open()) {
			//std::cout << "\t\tSuccessfully created video.cfg." << std::endl;
			newConfigFile << "0 0 0 0";
		}
		// Opening and creating default config file failed, set Settings manually
		else{
			//std::cout << "\t\tError creating default video.cfg." << std::endl;
			//std::cout << "\t\t\tManually setting default video.cfg settings." << std::endl;
			setVideo(0, 0, 0, 0);
		}
		newConfigFile.close();
	}
	fileSettings.close();
}

void AudioManager::LoadAudio()
{
	// Players.cpp
	sCast 			= Mix_LoadWAV("sounds/bfxr/snd_cast.wav");
	sDownStab		= Mix_LoadWAV("sounds/bfxr/snd_downStab.wav");
	sParry			= Mix_LoadWAV("sounds/bfxr/snd_parry.wav");
	sStep			= Mix_LoadWAV("sounds/cmesias/snd_step.wav");
	sSlash 			= Mix_LoadWAV("sounds/cmesias/snd_slash.wav");
	sDash			= Mix_LoadWAV("sounds/cmesias/snd_dash.wav");
}

void AudioManager::FreeAudio()
{
    // Players.cpp
	Mix_FreeChunk(sCast);
	Mix_FreeChunk(sSlash);
	Mix_FreeChunk(sDownStab);
	Mix_FreeChunk(sParry);
	Mix_FreeChunk(sStep);
	Mix_FreeChunk(sDash);
	sCast 			= NULL;
	sSlash 			= NULL;
	sDownStab		= NULL;
	sParry			= NULL;
	sStep			= NULL;
	sDash			= NULL;
}

//Get's input from user and returns it
void AudioManager::start(LWindow &gWindow, SDL_Renderer *gRenderer)
{
	// Clear everytime we come back
	vButtons.clear();

	// Load audio settings everytime we come back
	LoadAudioCFG();
	LoadVideoCFG();

	// Other classes fonts
	textNFont.LoadFonts();
	pauseLoop = true;
	gCursor.loadFromFile(gRenderer, "resource/gfx/cursor.png");

	// Initialize buttons
	audioBtn.Init("Audio", "Index1", 70, 10);
	videoBtn.Init("Video", "Index2", 70, 10);
	backBtn.Init("Back", "Back", 70, 10);
	exitBtn.Init("Exit", "Exit", 70, 10);

	masterVolBtn.Init("Main volume", "MasterVolume", 300, 10);
	musicVolBtn.Init("Music volume", "MusicVolume", 300, 10);
	sfxVolBtn.Init("SFX volume", "SFXVolume", 300, 10);

	resolutionBtn.Init("Resolution", "Resolution", 250, 10);
	antiAliasBtn.Init("Anti Aliasing", "AntiAliasing", 250, 10);
	vsyncBtn.Init("Vsync", "Vsync", 250, 10);
	fullscrenBtn.Init("Fullscreen", "Fullscreen", 250, 10);

	applyBtn.Init("Apply", "Apply", 70, 10);
	keepBtn.Init("Keep", "Keep", 70, 10);
	revertBtn.Init("Revert", "Revert", 70, 10);

	// Store buttons in a vector
	vButtons.push_back(&audioBtn);
	vButtons.push_back(&videoBtn);
	vButtons.push_back(&backBtn);
	vButtons.push_back(&exitBtn);

	vButtons.push_back(&masterVolBtn);
	vButtons.push_back(&musicVolBtn);
	vButtons.push_back(&sfxVolBtn);

	vButtons.push_back(&resolutionBtn);
	vButtons.push_back(&antiAliasBtn);
	vButtons.push_back(&vsyncBtn);
	vButtons.push_back(&fullscrenBtn);

	vButtons.push_back(&applyBtn);
	vButtons.push_back(&keepBtn);
	vButtons.push_back(&revertBtn);

	// Load buttons
	for (unsigned int i=0; i<vButtons.size(); i++) {
		vButtons[i]->Load();
	}

	// Used globally
	buttonsSize = vButtons.size();

	while (pauseLoop) {

		// Start FPS cap
		helper.fps.start();

		// Handle events
		while (SDL_PollEvent(&e) != 0) {

			if (e.type == SDL_QUIT) {
				audioManagerResult	= Exit;
				pauseLoop 	= false;
				gCursor.free();
			} else {

				//Handle window events
				gWindow.handleEvent(gRenderer, e);

				if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
					switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						audioManagerResult	= Back;
						pauseLoop = false;
						break;
					case SDLK_DOWN:
						//
						break;
					}
				}

				//If Key Released
				else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
					switch (e.key.keysym.sym) {
					case SDLK_UP:
						//
						break;
					case SDLK_DOWN:
						//
						break;
					}
				}

				// Mouse Pressed
				mousePressed(e);

				// Mouse Released
				audioManagerResult = mouseReleased(e);

				// Results
				switch (audioManagerResult)  {
					case Back:				// Back (to Main Menu)
						audioManagerResult	= Back;
						pauseLoop = false;
						break;
					case Exit:			// Start Game
						audioManagerResult	= Exit;
						pauseLoop 	= false;
						gCursor.free();
						return;
				}
			}
		}

		// Get mouse coordinates
		SDL_GetMouseState(&mx, &my);

		// New mouse coordinates, no relation to camera
		mex = (helper.screenWidth*mx)/gWindow.getWidth();
		mey = (helper.screenHeight*my)/gWindow.getHeight();

		// Default positions for buttons that are always in view
		int xPos = helper.screenWidth  * 0.05;
		int yPos = helper.screenHeight * 0.15;

		// Always in view: Audio, Video, Back and Exit buttons
		for (unsigned int i=0; i<=3; i++) {
			vButtons[i]->Update(mex, mey, xPos, yPos + i * 45,
								leftClick);
		}

		// Always in view: Apply button
		for (unsigned int i=11; i<=3; i++) {
			vButtons[i]->Update(mex, mey, xPos, yPos + i * 45,
								leftClick);
		}

		// Default positions for modifying buttons
		xPos = helper.screenWidth  * 0.15;

		// View Audio buttons
		if (settingsIndex == 0) {

			// Set location of Audio sliders in view
			for (unsigned int i=4; i<=6; i++) {
				vButtons[i]->Update(mex, mey, xPos, yPos + (i-4) * btnSpacingH, leftClick);
			}

			// Set location of Video sliders OUT of view
			for (unsigned int i=7; i<=10; i++) {
				vButtons[i]->Update(mex, mey, -2000, -2000, leftClick);
			}

			// Set location of Apply button
			int anchorIndex = 6;
			vButtons[11]->Update(mex, mey, xPos, vButtons[anchorIndex]->y + vButtons[anchorIndex]->h+applyButtonYOffset,
								leftClick);

			// Set location of Keep & Revert btns in view
			if (promptToKeep) {

				// Set location of Keep button
				anchorIndex = 11;
				float x = vButtons[anchorIndex]->x + vButtons[anchorIndex]->w+keepXOffset;
				float y = vButtons[anchorIndex]->y;
				vButtons[12]->Update(mex, mey, x, y, leftClick);

				// Set location of Revert button
				anchorIndex = 12;
				x = vButtons[anchorIndex]->x + vButtons[anchorIndex]->w+revertXOffset;
				y = vButtons[anchorIndex]->y;
				vButtons[13]->Update(mex, mey, x, y, leftClick);
			}

			// Hide keep & Revert btns
			else {
				vButtons[12]->Update(mex, mey, -2000, -2000, leftClick);
				vButtons[13]->Update(mex, mey, -2000, -2000, leftClick);
			}
		}

		// View Video buttons
		else {

			// Video buttons in view
			for (unsigned int i=7; i<=10; i++) {
				vButtons[i]->Update(mex, mey, xPos, yPos + (i-7)* btnSpacingH, leftClick);
			}

			// Audio buttons OUT of view
			for (unsigned int i=4; i<=6; i++) {
				vButtons[i]->Update(mex, mey, -2000, -2000, leftClick);
			}

			// Set location of Apply button
			int anchorIndex = 10;
			vButtons[11]->Update(mex, mey, xPos, vButtons[anchorIndex]->y + vButtons[anchorIndex]->h+applyButtonYOffset,
								leftClick);

			// Set location of Keep & Revert btns in view
			if (promptToKeep) {

				// Set location of Keep button
				anchorIndex = 11;
				float x = vButtons[anchorIndex]->x + vButtons[anchorIndex]->w+keepXOffset;
				float y = vButtons[anchorIndex]->y;
				vButtons[12]->Update(mex, mey, x, y, leftClick);

				// Set location of Revert button
				anchorIndex = 12;
				x = vButtons[anchorIndex]->x + vButtons[anchorIndex]->w+revertXOffset;
				y = vButtons[anchorIndex]->y;
				vButtons[13]->Update(mex, mey, x, y, leftClick);
			}

			// Hide keep & Revert btns
			else {
				vButtons[12]->Update(mex, mey, -2000, -2000, leftClick);
				vButtons[13]->Update(mex, mey, -2000, -2000, leftClick);
			}
		}


		// Change options based based on mouse position
		{
			if (leftClick) {

				// Change volume buttons
				for (unsigned int i=4; i<=6; i++)
				{
					if (vButtons[i]->pressed) {
						int barMaxValue;
						if (i == 4)
							barMaxValue = 100;
						else
							barMaxValue = 128;
						option[i-4] = barMaxValue*(mex - (vButtons[i]->x-20))/(vButtons[i]->w+40);
					}
				}
			}
		}

		// Prompt timer
		if (promptToKeep) {
			promptTimer -= 1;

			// Time out from user, revert to old settings
			if (promptTimer <= 0) {
				promptToKeep = false;

				// Load old settings
				LoadAudioCFG();
			}
		}

		// Clear screen
		SDL_SetRenderDrawColor(gRenderer, 20, 50, 70, 255);
		SDL_RenderClear(gRenderer);

		// Render everything
		{
			// Render buttons: Audio, Video, Back & Exit
			for (unsigned int i=0; i<=3; i++)
			{
				vButtons[i]->Render(gRenderer);
			}

			// Render Audio buttons
			for (unsigned int i=4; i<=6; i++)
			{
				// Render BG for sliders
				{
					int barVlue = option[i-4];
					int barPixelWidth = vButtons[i]->w+40;
					int barMaxValue;
					if (i == 4)
						barMaxValue = 100;
					else
						barMaxValue = 128;
					int width = barPixelWidth*(barVlue)/barMaxValue;
					helper.RenderFillRect(gRenderer, vButtons[i]->x-20, vButtons[i]->y-10, width, vButtons[i]->h+20, {13,18,40});
				}

				// Render buttons
				vButtons[i]->Render(gRenderer, btnTextXOffset, btnTextYOffset);

				// Render value for buttons
				textNFont.RenderText(gRenderer, vButtons[i]->x+vButtons[i]->w, vButtons[i]->y, getValue(i-4), 255, {255,255,255}, "left");
			}

			// Render Video buttons
			for (unsigned int i=7; i<=10; i++)
			{

				if (i != 7) {

					// Render BG for sliders
					{
						int barVlue = option[i-4]+1;
						int barPixelWidth = vButtons[i]->w+40;
						int barMaxValue = 2;
						int width = barPixelWidth*(barVlue)/barMaxValue+1;

						// Render rect on the left side of slider (On)
						if (barVlue == 1) {
							helper.RenderFillRect(gRenderer, vButtons[i]->x-20, vButtons[i]->y-10, width, vButtons[i]->h+20, {13,18,40});
						}

						// Render rect on the left side of slider (Off)
						else {
							helper.RenderFillRect(gRenderer, vButtons[i]->x+(width/2)-20, vButtons[i]->y-10, width/2, vButtons[i]->h+20, {13,18,40});
						}
					}

					// Rebder buttons
					vButtons[i]->Render(gRenderer, btnTextXOffset, btnTextYOffset);

					// Render value for buttons
					std::string name = "On";
					if (getValue(i-4) == 1)
						name = "On";
					else
						name = "Off";
					textNFont.RenderText(gRenderer, vButtons[i]->x+vButtons[i]->w, vButtons[i]->y, name.c_str(), 255, {255,255,255}, "left");
				} else {
					// Render BG for sliders
					{
						int barVlue = option[i-4];
						int barPixelWidth = vButtons[i]->w+40;
						int barMaxValue;
						barMaxValue = 5;
						int width = barPixelWidth*(barVlue)/barMaxValue;
						helper.RenderFillRect(gRenderer, vButtons[i]->x-20, vButtons[i]->y-10, width, vButtons[i]->h+20, {13,18,40});
					}

					// Rebder buttons
					vButtons[i]->Render(gRenderer, btnTextXOffset, btnTextYOffset);

					// Render value for buttons
					textNFont.RenderText(gRenderer, vButtons[i]->x+vButtons[i]->w, vButtons[i]->y, getValue(i-4), 255, {255,255,255}, "left");
				}
			}

			// Render Apply btn
			vButtons[11]->Render(gRenderer);

			// Render Keep btn
			vButtons[12]->Render(gRenderer);

			// Render Revert btn
			vButtons[13]->Render(gRenderer);

			// Render text to prompt user
			if (promptToKeep) {
				std::stringstream tempss;
				tempss << "Reverting settings in " << promptTimer/60 << ".";
				textNFont.RenderText(gRenderer, vButtons[11]->x, vButtons[11]->y+vButtons[11]->gText.getHeight()+20, tempss.str().c_str(), 255, {255,255,255}, "right");
			}

			// Render mouse location
			gCursor.render(gRenderer, mex, mey, 20, 20);
		}

		// Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		helper.frame++;
		if((helper.cap == true ) && (helper.fps.get_ticks() < 1000 / helper.FRAMES_PER_SECOND ))
			SDL_Delay((1000 / helper.FRAMES_PER_SECOND ) - helper.fps.get_ticks());

	}

	// Other classes fonts
	textNFont.LoadFonts();
	gCursor.free();
}



// Mouse Pressed
void AudioManager::mousePressed(SDL_Event event)
{
	// If mouse button down
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = true;
			/*
			*/
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			//
		}
	}
}

// Mouse Released
AudioManager::AudioManagerResult AudioManager::mouseReleased(SDL_Event event){
	AudioManager::AudioManagerResult result = Nothing;

	if (event.type == SDL_MOUSEBUTTONUP)
	{
		// If Left mouse button
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			leftClick = false;

			// Change to view Audio or Video Settings, or Back or Exit
			for (unsigned int i=0; i<buttonsSize; i++)
			{
				if (vButtons[i]->CheckMouseReleased(mex, mey) == "Index1")
				{
					settingsIndex = 0;
				}
				if (vButtons[i]->CheckMouseReleased(mex, mey) == "Index2")
				{
					settingsIndex = 1;
				}
				if (vButtons[i]->CheckMouseReleased(mex, mey) == "Back")
				{
					result = Back;
				}
				if (vButtons[i]->CheckMouseReleased(mex, mey) == "Exit")
				{
					result = Exit;
				}
				if (vButtons[i]->CheckMouseReleased(mex, mey) == "Apply")
				{
					promptToKeep = true;
					promptTimer = 60*11;
				}
				if (vButtons[i]->CheckMouseReleased(mex, mey) == "Keep")
				{
					promptToKeep = false;
					promptTimer = 60*11;
					SaveAudioCFG();
					SaveVideoCFG();
				}
				if (vButtons[i]->CheckMouseReleased(mex, mey) == "Revert")
				{
					LoadAudioCFG();
					LoadVideoCFG();
				}
			}


			// Change video buttons
			{
				for (unsigned int i=7; i<=10; i++)
				{
					if (vButtons[i]->pressed) {

						// If resolution bar, there will be 5 chocies
						if (i == 7) {
							int barMaxValue;
							barMaxValue = 5;
							option[i-4] = (barMaxValue*(mex - (vButtons[i]->x-20))/(vButtons[i]->w+40)) + 1;
						}

						// Everything else has only 2 choices: "Off" or "On"
						else {
							int barMaxValue = 2;

							// Set to off
							if (helper.checkCollision(mex, mey, 1, 1, vButtons[i]->x, vButtons[i]->y, vButtons[i]->w/2, vButtons[i]->h)) {
								option[i-4] = barMaxValue*(mex - (vButtons[i]->x-20))/(vButtons[i]->w+40);
							}
							// Set to on
							else {
								option[i-4] = barMaxValue*(mex - (vButtons[i]->x-20))/(vButtons[i]->w+40);
							}
						}
					}
				}
			}
		}
		// If Right mouse button
		if (event.button.button == SDL_BUTTON_RIGHT)
		{
		}
	}
	return result;
}
