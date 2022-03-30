/*
 * Tile.h
 *
 *  Created on: Jan 16, 2017
 *      Author: Carl
 */

#ifndef LOCAL_TILES_H_
#define LOCAL_TILES_H_

#include "../LWindow.h"
#include "../LTexture.h"

#include "../Helper.h"

class Tile: public Helper {
public:
	/* Tilec Variables */
	const int max = 5000;
	int tileCount = 0;
	int multiW = 1;
	int multiH = 1;
	int tilew = 64;
	int tileh = 64;
	const int expandW = 4;		// 4 times larger than original texture size
	const int expandH = 4;	// 4 times larger than original texture size
	bool hideOtherLayers = false;
	LTexture gTiles;
	LTexture gText;
	TTF_Font *gFont12 = NULL;
	TTF_Font *gFont24 = NULL;

public:
	float x;
	float y;
	int w;
	int h;

	// Get rect of each specific Tile
	SDL_Rect rectB;

	/*
	 * 0: Used to collide with the player to move into a new area/level
	 * 306: locked door
	 * 309: Unlocked door
	 */
	int id;
	int alpha;
	SDL_Rect clip;

	/* Layers
	 *
	 * 0: Floor
	 * 1: Appliance			- Only rendered on top of Player when player.y+player.h < tile.x+tile.w
	 * 2: Top of Appliance	- Always rendered on top of Player
	 * 3: Roof 				- Always rendered on top of Player
	 */
	int layer;
	bool mouse;				// Mouse point
	bool mouseBox;			// Mouse box
	bool screen;
	bool player;
	bool promptSelf;
	bool alive;
	std::string side;		// collision with player
	std::string sideSlime;	// collision with Slime
	bool collisionTile;		// If true, this Tile is considered a collision tile
	bool collisionMobs;

public:	// animation for light Tile

	// 296 - 303:	top of light tile
	// 328 - 335:	bottom of light tile
	float animTimer;
	const float animTimerSpe = 5;

public:	// Initial
	void initTile(Tile tile[]);

	void load(SDL_Renderer *gRenderer);

	void free();

public:	// editor functions
	void placeTile(Tile tile[], float x, float y,
					int id, int layer,
					bool collisionTile,
					SDL_Rect clip);

	void copyTile(Tile tile[]);

	void changeTileCollision(Tile tile[]);

	void changeTileLayer(Tile tile[], int value);

	void removeTile(Tile tile[], int click);

	void RemoveAll(Tile tile[]);

	/* Place Tile */
	void spawnTile(Tile tile[], int newMx, int newMy, int camx, int camy, SDL_Rect rTiles[]);

public:	// core game functions
	void updateTile(Tile tile[], LWindow &gWindow,
			  float targetX, float targetY, float targetW, float targetH,
			  int newMx, int newMy,
			  int mex, int mey, int camx, int camy,
			  SDL_Rect rTiles[]);

	bool checkCollisionRect( SDL_Rect a, SDL_Rect b );

	// Tile collision check, player x position
	void checkCollisionXY(Tile tile[], float &x, float &y, int w, int h, float &vX, float &vY);

	void renderTile(SDL_Renderer *gRenderer, Tile tile[], int layer_dummy, int camx, int camy);

	bool PlayerBehindTile;

	void RenderBehindPlayer(SDL_Renderer *gRenderer, Tile tile[], int layerToRender, int camx, int camy, SDL_Rect rTiles[]);

	void RenderOnTopOfPlayer(SDL_Renderer *gRenderer, Tile tile[], int layerToRender, int camx, int camy, SDL_Rect rTiles[]);

	void RenderUI(SDL_Renderer *gRenderer, Tile tile[], int camx, int camy);

	// Render Tile Debug info
	void renderTileDebug(SDL_Renderer *gRenderer, Tile tile[], int newMx, int newMy, int mex, int mey, int camx, int camy, SDL_Rect rTiles[]);

	// Render Tile Debug in Hand
	void RenderHand(SDL_Renderer *gRenderer, Tile tile[], int newMx, int newMy, int mex, int mey, int camx, int camy, SDL_Rect rTiles[]);

	void setStatsBasedOnType(Tile tile[], int i);

public:	// Get functions
	// Get size of Tile in pixels
	int getPixelW(SDL_Rect rTiles[]);

	// Get size of Tile in pixels
	int getPixelH(SDL_Rect rTiles[]);

public:	// Tile manipulation

	void MoveTiles(Tile tile[], std::string direction);

public: // tile saving functions

	// Load Tile data
	void LoadData(Tile tile[], std::fstream &fileToReturn);

	// Save Tile data
	std::string SaveData(Tile tile[]);
};

#endif /* LOCAL_TILES_H_ */
