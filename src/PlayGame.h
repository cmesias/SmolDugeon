/*
 * PlayGame.h
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#ifndef PLAYGAME_H_
#define PLAYGAME_H_

#include "Game/Boss.h"
#include "Game/Maps.h"
#include "Game/Players.h"
#include "Game/Spawners.h"
#include "Game/Object.h"
#include "Game/TileBar.h"
#include "Game/Tiles.h"
#include "Game/Tilec.h"
#include "Game/Item.h"
#include "Game/Mob.h"
#include "Helper.h"
#include "Options.h"

class PlayGame : public Helper, public Options {
public:	// Global World Variables
	double gravity = 0.6;
	bool RestartLevel = false;

public: // global functions
	bool checkCollisionRect( SDL_Rect a, SDL_Rect b );

public:	// globals
	bool shift;
	bool editor;
	bool debug;
	/* 0: Textured tiles
	 * 1: Collision Tiles
	 * 2: Boss
	 * 3: Item
	 * 4: Mobs
	 *
	 */
	int place_type;
	int newMx, newMy;
	int mex, mey;
	bool camUp = false;
	bool camDown = false;
	bool camLeft = false;
	bool camRight = false;
	bool leftClick = false;
	bool rightClick = false;

public: // System variables
	enum Result { Back, Nothing, StartGame, ShowingMenu, Exit };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer,
			  PlayGame::Result &result, int &LevelToLoad);
	PlayGame::Result mousePressed(SDL_Event event);
	PlayGame::Result mouseReleased(SDL_Event event);

public:	// Scene textures
	LTexture gBG;
	LTexture gBG2;
	LTexture gTargetTexture;
	LTexture gCircle;
	LTexture gShadow;

	// Mouse cursor
	LTexture gCursor;

	class Text {
	public:
		int count;
		float x;
		float y;
		int w;
		int h;
		int alpha;
		int timer;
		bool alive;
		float vX;
		float vY;
		SDL_Color color;

		/*
		 * 0: General information logs like debug logs
		 * 1: These will have an x & y coordinate somewhere
		 */
		int type;

		std::string textfield;
		void init(Text text[]) {
			count =0;
			for (int i=0; i<100; i++) {
				text[i].alive = false;
			}
		}
		void spawn(Text text[], float x, float y, float vX, float vY, int alpha, std::string textfield, int type, SDL_Color color={244,144,40}) {
			for (int i=0; i<100; i++) {
				if (!text[i].alive) {
					//std::cout << "Spawning text object." << std::endl;
					text[i].type = type;
					text[i].color = color;
					text[i].x = x;
					text[i].y = y;
					text[i].w = 139;
					text[i].h = 96;
					text[i].vX = vX;
					text[i].vY = vY;
					text[i].timer = timer;
					text[i].alpha = alpha;
					text[i].textfield = textfield;
					text[i].alive = true;
					count++;
					break;
				}
			}
		}
		void updateDebugText(Text text[]) {
			for (int i=0; i<100; i++) {
				if (text[i].alive && text[i].type == 0) {
					text[i].x += text[i].vX;
					text[i].y += text[i].vY;

					text[i].alpha -= 2;
					if (text[i].alpha <= 0) {
						text[i].alive = false;
						count--;
					}
				}
			}
		}

		void updateDamageText(Text text[]) {
			for (int i=0; i<100; i++) {
				if (text[i].alive && text[i].type == 1) {
					text[i].x += text[i].vX;
					text[i].y += text[i].vY;

					text[i].alpha -= 2;
					if (text[i].alpha <= 0) {
						text[i].alive = false;
						count--;
					}
				}
			}
		}
	};

	Text tex;
	Text text[100];

public:	// Other classes
	Map map;
	Particle part;
	Spawner spaw;
	//Pause pau;
	//Pause::Result pauseResult;
	Players player;

	// Class instance, multiple
	Particle particles[800];
	Spawner spawner[200];
	//Mixed mix;
	//Controls cont;
	//static Items items[10];

	// Objects (Player attack collisions
	Object obj;
	Object object[10];

	// Boss
	Boss bos;
	Boss boss[100];

	// Mob
	Mob mb;
	Mob mob[100];

	// Items (mostly swords)
	Item ite;
	Item item[100];

public: // Tile Variables

	const int HowManyTilesVertical   = 16;
	const int HowManyTilesHorizontal = 32;

	const int TilesCutSizeW   = 16;
	const int TilesCutSizeH = 16;

	// Tilebar
	TileBar tb;
	TileBar tilebar[UniqueTileBarTiles];

	// For Tiles.cpp
	SDL_Rect rTiles[UniqueTileBarTiles];

	// Tiles
	Tile tl;
	Tile tile[5000];

	// Collision Tiles
	Tilec tlc;
	Tilec tilec[1000];

public:	// Player variables
	// Player spawn point
	float spawnX = 0, spawnY = 0;
	std::string saveSpawnPoint();
	std::string saveMapSize();
	void loadSpawnPoint();
public:
    // camera
    int camx;
    int camy;
    bool camlock;

public:	// Initial functions

	// Initialize
	void Init();

	// Load resources
	void Load(LWindow &gWindow, SDL_Renderer *gRenderer);

	// Free resources
	void Free();

public:	// Core functions
	// Update everything
	void Update(LWindow &gWindow, SDL_Renderer *gRenderer);

	// Render foreground
	void RenderFG(SDL_Renderer *gRenderer, LWindow &gWindow);

	// Render shadows
	void RenderShadows(SDL_Renderer *gRenderer, LWindow &gWindow);

	// Render game objects
	void Render(SDL_Renderer *gRenderer, LWindow &gWindow);

	// Render UI
	void RenderUI(SDL_Renderer *gRenderer, LWindow &gWindow);

	// Render debug information
	void RenderDebug(SDL_Renderer *gRenderer);

	// Render text
	void RenderText(SDL_Renderer *gRenderer, LWindow &gWindow);

public:	// Functions mixed with other classes

	// Check collision between Particle & Tile
	void checkCollisionParticleTile();

	//------------------ Hurts Boss
	// Check collision between Particle & Boss
	void checkCollisionParticleBoss();

	//------------------ Items

	// Check collision between Player & Item
	void checkCollisionPlayerItem();

	//------------------ Items


	//------------------ Mobs

	//------------------ Hurts Mobs
	// Check collision between Mobs & Particle
	void checkCollisionParticleMob();

	//------------------ Hurts Mobs
	// Check collision between Mobs & Player attacks
	void checkPlayerAttacksCollisionMob();

	// Check if Mobs have vision of Player
	void checkMobPlayerVision();

	// Check collision between Mobs & Tile
	// Checks collision with Tiles, while at the same
	// time creates a path to go towards the Player while avoiding Tiles
	void checkCollisionTileMob();

	//------------------ Mobs

	// Check collision between Boss & Tile
	void checkBossTileCollision();

	//------------------ Hurts Boss
	// Check collision between Player attacks & Boss
	void checkPlayerAttacksCollisionBoss();

	// Check collision between Player attacks & Tile
	void checkPlayerAttacksTileCollision();

	//------------------ Hurts Boss
	// Check collision between Player attacks & Boss Particle
	void checkPlayerAttacksBossParticleCollision();

	// Check collision between Boss attacks & Player
	void checkBossAttacksCollisionPlayer();

	// Check collision between Boss & Player
	void checkCollisionBossPlayer();

	//---------- Hurts Player
	// Check collision between Particle & Player
	void checkCollisionParticlePlayer();

	// Check collision between Player Particle & Boss Particle
	void checkCollisionParticleParticle();

	//---------- Boss deafeated? Update levelsCompleted.mp
	void checkBossOrMobDied();

public:	// Controls
	// Editor controls
	void editorOnKeyDown(SDL_Keycode sym);
	void editorOnKeyUp(SDL_Keycode sym);

private:	// used for some debugging
	int mx, my;
	bool quit;
	SDL_Event event;

private:	// Load level

	// Boss died, update levelsUnlocked.mp
	void SaveLevelsCompleted();

	// Load config files
	void LoadCFG();

	// Load previous high-score for Level
	void LoadHighScore();

	// Check collision between Player & Tilec
	void checkPlayerTilceCollision();

	unsigned int previousLevel = -1;
	unsigned int LevelToLoad;
	float lastKnownPositionX, lastKnownPositionY;

	// Load level
	void LoadLevel();
};

#endif /* PLAYGAME_H_ */
