/*
 * Mob.h
 *
 *  Created on: Jul 21, 2017
 *      Author: Carl
 */

#ifndef GAME_MOB_H_
#define GAME_MOB_H_

#include "Object.h"

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "../LWindow.h"
#include "../LTexture.h"

#include "../Helper.h"
#include "Monster.h"
#include "Maps.h"
#include "TextNFont.h"
#include "../Particless.h"

class Mob: public Monster, public Helper, public TextNFont {
public:
	int count;
	const int max = 100;
	LTexture gMob;
	LTexture gMobFlashed;
	LTexture gMobShadow;
	Mix_Chunk *sCast 			= NULL;
	SDL_RendererFlip flip;

	SDL_Rect rClip[6];

	/*
	 * 0 - 3: 	Idle
	 * 4: 	  	Cool down
	 * 5: 	  	Pre-Attack
	 *
	 */
	int sprite_index;

public: // variables

	// Walking timer
	double walkTimer;
	double walkSpeed;
	double walkFrame;

	// Idle timer
	float idleTimer;
	const float idleSpe = 10;

public:	// Core Functions
	void Init(Mob mob[]);
	void Load(SDL_Renderer *gRenderer);
	void Free();
	void RemoveAll(Mob mob[]);
	void Remove(Mob mob[]);
	void Spawn(Mob mob[], float x, float y, float w, float h, float angle, float speed, int type);

	void Update(Mob mob[],
			Object &obj, Object object[],
			Particle particle[], Particle &p_dummy,
			Map &map, int mex, int mey,
			int camx, int camy, bool playerAlive);

	void UpdateEditor(Mob mob[], int mex, int mey, int camx, int camy);

	void Move(Mob mob[], std::string direction);

	void RenderShadow(SDL_Renderer *gRenderer, Mob mob[], int camx, int camy);

	void RenderBack(SDL_Renderer *gRenderer, Mob mob[], TTF_Font *gFont, LTexture gText, int camx, int camy);

	void RenderFront(SDL_Renderer *gRenderer, Mob mob[], TTF_Font *gFont, LTexture gText, int camx, int camy);

	void RenderUI(SDL_Renderer *gRenderer, Mob mob[], int camx, int camy);

	void RenderDebug(SDL_Renderer *gRenderer, Mob mob[], TTF_Font *gFont, LTexture gText, int camx, int camy);

	void RenderHand(SDL_Renderer *gRenderer, Mob mob[], int newMx, int newMy,
						  int mex, int mey, int camx, int camy);

public:	// Functions that work with other classes

	void GetDistanceOfPlayer(Mob mob[], float targetX, float targetY,
				float targetW, float targetH,
				float *xFollow, float *yFollow);

public:	// Setter functions

	void setStatsBasedOnType(Mob mob[], int i);

public:	// Getter functions

public:	// Animations

	// This will be more specific later, just like our Item.cpp,
	// it has different sizes based on clipped size
	int pixelSizeW = 20;
	int pixelSizeH = 24;
	int	shadowSize = 15;

	// Used for textures
	float yOffset;

	// Mob will use this to lock on to the player when the bullet shoots out
	float *xFollow;
	float *yFollow;

	// Target center
	float bmx, bmy;

	// Self center
	float bmx2, bmy2;

	// Target center
	float targetDistanceX;
	float targetDistanceY;

	bool renderInFront;
	bool mouse;
	bool flash;				// flash mobes texture on getting hit by players attacks

	/*
	 * 0: Attack 1
	 * 1: Attack 2
	 */
	int randomAttack;	// Depracated, get rid of this



	/*
	 * -1: 	Cooldown state (can't attack)
	 * 0: 	Walking around
	 * 1: 	Moving towards player
	 * 2: 	Normal 4 shot Attack
	 * 3: 	Pre BARRAGE Attack!!!		- Part 1 of Barrage Attack
	 * 4: 	BARRAGE Attack!				- Part 2 of Barrage Attack
	 */
	int animState;
	bool chargingAttack;
	float chargeTime;					// After 3 seconds, attack
	const float chargeTimeStart = 60;	// Change this to change the charge-attack animation start time

	float constantFiringTimer;

	float coolDownTimer;
	float coolDownTimeStart = 180;

public: // Saving & loading functions

	void LoadData(Mob mob[], std::fstream &fileTileDataL);		// Load data

	std::string SaveData(Mob mob[]);		// Save data

public: // Other classes: Objects


};

#endif /* GAME_BOSS_H_ */
