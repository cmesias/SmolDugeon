/*
 * Tile.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: Carl
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mouse.h>

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include "Tiles.h"

void Tile::load(SDL_Renderer *gRenderer) {
	gTiles.loadFromFile(gRenderer, "resource/gfx/author_0x72/0x72_16x16DungeonTileset_v4Walls_v2.png");
	gFont12 = TTF_OpenFont("resource/fonts/PressStart2P.ttf", 12);
	gFont24 = TTF_OpenFont("resource/fonts/PressStart2P.ttf", 24);
}

void Tile::free() {
	gTiles.free();
	gText.free();
	TTF_CloseFont(gFont12);
	TTF_CloseFont(gFont24);
	gFont12 = NULL;
	gFont24 = NULL;
}


void Tile::initTile(Tile tile[]) {
	layer 		= 0;
	id 			= 0;
	tileCount 	= 0;
	multiW 		= 1;
	multiH 		= 1;
	hideOtherLayers 	= false;
	for (int i = 0; i < max; i++) {
		tile[i].w = tilew;
		tile[i].h = tileh;
		tile[i].id = -1;
		tile[i].alpha = 255;
		tile[i].layer = -1;
		tile[i].animTimer = 0;
		tile[i].mouse = false;
		tile[i].mouseBox = false;
		tile[i].screen = false;
		tile[i].player = false;
		tile[i].side = "right";
		tile[i].collisionTile = false;
		tile[i].collisionMobs = false;
		tile[i].PlayerBehindTile = false;
		tile[i].promptSelf = false;
		tile[i].alive = false;
	}
}

void Tile::placeTile(Tile tile[], float x, float y,
					int id, int layer,
					bool collisionTile,
					SDL_Rect clip) {
	for (int i = 0; i < max; i++) {
		if (!tile[i].alive){
			tile[i].x 		= x;
			tile[i].y 		= y;
			tile[i].w = tilew;
			tile[i].h = tileh;
			tile[i].id 		= id;
			tile[i].alpha 	= 255;
			tile[i].clip 	= clip;
			tile[i].layer 	= layer;
			tile[i].collisionTile 	= collisionTile;
			tile[i].collisionMobs = false;
			tile[i].PlayerBehindTile = false;
			tile[i].animTimer = 0;
			tile[i].mouse 	= false;
			tile[i].mouseBox = false;
			tile[i].screen 	= false;
			tile[i].player = false;
			tile[i].side = "right";
			tile[i].alive 	= true;

			// Set default parameters
			setStatsBasedOnType(tile, i);

			tileCount++;
			break;
		}
	}
}
void Tile::copyTile(Tile tile[]) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			if (tile[i].mouse){
				if (tile[i].layer == layer) {
					id = tile[i].id;
					collisionTile = tile[i].collisionTile;
				}
			}
		}
	}
}
void Tile::changeTileCollision(Tile tile[]) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			if (tile[i].mouse){
				if (tile[i].layer == layer) {
					tile[i].collisionTile = (!tile[i].collisionTile);
				}
			}
		}
	}
}
void Tile::changeTileLayer(Tile tile[], int value) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			if (tile[i].mouse){
				if (tile[i].layer == layer) {
					tile[i].layer += value;
				}
			}
		}
	}
}
void Tile::removeTile(Tile tile[], int click) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			// Left click
			if (click == 0) {
				if (tile[i].mouseBox){
					if (tile[i].layer == layer) {
						tile[i].alive = false;
						tileCount--;
					}
				}
			// Right click
			}else{
				if (tile[i].mouse){
					if (tile[i].layer == layer) {
						tile[i].alive = false;
						tileCount--;
					}
				}
			}
		}
	}
}

void Tile::RemoveAll(Tile tile[]) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			tile[i].alive = false;
		}
	}
	tileCount = 0;
}

/* Place Tile */
void Tile::spawnTile(Tile tile[], int newMx, int newMy, int camx, int camy, SDL_Rect rTiles[]) {
	removeTile(tile, 0);
	for (int j = 0; j < multiW; j++) {
		for (int h = 0; h < multiH; h++) {
			int x = int(newMx + j * tilew+camx);
			int y = int(newMy + h * tileh+camy);
			placeTile(tile, x, y, id, layer, collisionTile, rTiles[id]);
		}
	}
}

void Tile::updateTile(Tile tile[], LWindow &gWindow,
					  float targetX, float targetY, float targetW, float targetH,
					  int newMx, int newMy,
					  int mex, int mey, int camx, int camy,
					  SDL_Rect rTiles[]) {

	//std::cout << "newMx: " << newMx << std::endl;
	//std::cout << "newMy: " << newMy << std::endl;
	int tileW = tilew*multiW;
	int tileH = tileh*multiH;
	for (int i = 0; i < max; i++)
	{
		if (tile[i].alive)
		{

			// If Tile is not a Floor Tile
			if (tile[i].layer == 1 || tile[i].layer == 2) {
				// Check if player is in front of Tile or not
				if (targetY+targetH < tile[i].y+tile[i].h){
					tile[i].PlayerBehindTile = true;
				} else {
					tile[i].PlayerBehindTile = false;
				}
			}

			//If the tile is in the screen
			if (tile[i].x + tile[i].w > camx-64 && tile[i].x < camx + screenWidth+64
			 && tile[i].y + tile[i].h > camy-64 && tile[i].y < camy + screenHeight+64) {
				tile[i].screen = true;
			} else {
				tile[i].screen = false;
			}

			//If the mouse+size is on the tile
			if (newMx+tileW-4 > tile[i].x && newMx+2 < tile[i].x + tile[i].w &&
					newMy+tileH-4 > tile[i].y && newMy+2 < tile[i].y + tile[i].h) {
				tile[i].mouseBox = true;
			} else {
				tile[i].mouseBox = false;
			}

			//If the mouse is on the tile
			if (mex > tile[i].x && mex < tile[i].x + tile[i].w &&
				mey > tile[i].y && mey < tile[i].y + tile[i].h) {
				tile[i].mouse = true;
			} else {
				tile[i].mouse = false;
			}

			// Reset layer if goes higher
			if (tile[i].layer > 6) {
				tile[i].layer = 0;
			}

			// Animations

			// If on first light Tile frame
			if (tile[i].id >= 328 && tile[i].id < 335) {
				// Start animation timer
				tile[i].animTimer += this->animTimerSpe;

				// If animation timer goes over 60 frames
				if (tile[i].animTimer > 60) {

					// Reset animation timer again
					tile[i].animTimer = 0;

					// Change tile into next light Tile id/frame
					tile[i].id++;
				}
			}

			// If on last light Tile frame, loop around
			else if (tile[i].id == 335) {
				// Start animation timer
				tile[i].animTimer += this->animTimerSpe;

				// If animation timer goes over 60 frames
				if (tile[i].animTimer > 60) {

					// Reset animation timer again
					tile[i].animTimer = 0;

					// Change tile into next light Tile id/frame
					tile[i].id = 328;
				}
			}
		}
	}
}


bool Tile::checkCollisionRect( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA,   leftB;
    int rightA,  rightB;
    int topA, 	 topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA 	= a.x;
    rightA 	= a.x + a.w;
    topA 	= a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB 	= b.x;
    rightB 	= b.x + b.w;
    topB 	= b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA < topB )
    {
        return false;
    }

    if( topA > bottomB )
    {
        return false;
    }

    if( rightA < leftB )
    {
        return false;
    }

    if( leftA > rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

// Tile collision check, player x position
void Tile::checkCollisionXY(Tile tile[],
		float &x, float &y,
		int w, int h,
		float &vX, float &vY) {

	{
		/*
		 *
		 *
		 *
		 * TODO [ ] - do ccheck for collision if player presses "E" key on chest, open chest and throw out sword
		 *
		 *
		 *
		 *
		 *
		 *
		 */

		// Update collision with Tiles
		//tl.checkCollision(tile, x, y, w, h, y, vY);

		// Update collision with Tiles
		// Player Velocity X Axis
		x += vX;

		// Get rect of Player
		SDL_Rect rectA;
		rectA.x = x;
		rectA.y = y;
		rectA.w = w;
		rectA.h = h;

		// Set moveback to false
		bool moveBack;
		moveBack = false;

		// For loop Tiles
		for (int i = 0; i < max; i++) {
			if (tile[i].alive){

				// Check only collision-enabled Tiles
				if (tile[i].collisionTile) {

					// If Player colliding with Tile
					if  ( checkCollisionRect( rectA, tile[i].rectB )) {
						/*// If Player has more than 0 keys, then unlock door, otherwise continue collision check
						if (keys > 0 && useKey) {
							useKey = false;
							if (tile[i].id == 65 || tile[i].id == 69 || tile[i].id == 73 || tile[i].id == 77 ||
								tile[i].id == 67 || tile[i].id == 71 || tile[i].id == 75 || tile[i].id == 79) {
								keys--;
								Mix_PlayChannel(-1, sKeyPickup, 0);
								tile[i].alive = false;
								tl.tileCount--;
							}
						}*/

						// Continue handling collision
						moveBack = true;
					}
				}
			}
		}
		if (moveBack){
			x -= vX;
		}

		// Player Velocity Y Axis
		y += vY;
		rectA.x = x;
		rectA.y = y;
		rectA.w = w;
		rectA.h = h;

		// Set moveBack always to false
		moveBack = false;

		// For loop Tiles
		for (int i = 0; i < max; i++) {
			if (tile[i].alive){
				if (tile[i].collisionTile) {
					if  ( checkCollisionRect( rectA, tile[i].rectB )) {
						// If Player has more than 0 keys, then unlock door, otherwise continue collision check
						/*if (keys > 0 && useKey) {
							useKey = false;
							if (tile[i].id == 65 || tile[i].id == 69 || tile[i].id == 73 || tile[i].id == 77 ||
								tile[i].id == 67 || tile[i].id == 71 || tile[i].id == 75 || tile[i].id == 79) {
								keys--;
								Mix_PlayChannel(-1, sKeyPickup, 0);
								tile[i].alive = false;
								tl.tileCount--;
							}
						}*/
						// Continue handling collision
						moveBack = true;
					}
				}
			}
		}
		if (moveBack){
			y -= vY;
		}
	}
}

void Tile::renderTile(SDL_Renderer *gRenderer, Tile tile[], int layer_dummy, int camx, int camy) {
	for (int i = 0; i < max; i++)
	{
		if (tile[i].alive)
		{
			// If the tile is on the screen
			if (tile[i].screen)
			{
				// Tile trasparency on Player collision
				if (tile[i].layer != 0 && tile[i].player)
				{
					//tile[i].alpha = 90;
				}else{
					//tile[i].alpha = 255;
				}

				// Render layer in hand
				if (hideOtherLayers)
				{
					if (layer == tile[i].layer)
					{
						gTiles.setAlpha(tile[i].alpha);
						gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
					}

				// Render all tiles
				}else{
					if (layer_dummy == tile[i].layer) {
						gTiles.setAlpha(tile[i].alpha);
						gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
					}
				}
			}
		}
	}
}


void Tile::RenderBehindPlayer(SDL_Renderer *gRenderer, Tile tile[], int layerToRender, int camx, int camy, SDL_Rect rTiles[]) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive) {


			// Render layer in hand
			if (hideOtherLayers)
			{
				if (layer == tile[i].layer)
				{
					gTiles.setAlpha(tile[i].alpha);
					gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
				}
			} else {

				// If player is below Tile
				if (tile[i].PlayerBehindTile)
				{
					// Do nothing
				}
				// If player is above Tile
				if (!tile[i].PlayerBehindTile)
				{
					// Render all Tiles that are layer 1 and above
					if (tile[i].layer == layerToRender) {

						// For loop to loop through all 8 light Tiles
						for (int j=0; j<8; j++) {

							////////////////////////////////////////////////////////////////////////////
							// -----------------------------------------------------------------------//
							// ---------------- This will render ONLY light Tiles: -------------------//
							// ----------------------- id's 296 through 303 --------------------------//
							// ----------------------- id's 328 through 335 --------------------------//
							if (tile[i].id == 328 + j )
							{
								// Render top half of light tile
								gTiles.setAlpha(tile[i].alpha);
								int topLightTileId = tile[i].id-32;
								gTiles.render(gRenderer, tile[i].x - camx, tile[i].y-tile[i].h - camy, tile[i].w, tile[i].h, &rTiles[topLightTileId]);

								// Render bottom half of light tile
								gTiles.setAlpha(tile[i].alpha);
								gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
							}
							// ----------------------- id's 328 through 335 --------------------------//
							// ----------------------- id's 296 through 303 --------------------------//
							// ---------------- This will render ONLY light Tiles: -------------------//
							// -----------------------------------------------------------------------//
							////////////////////////////////////////////////////////////////////////////

							////////////////////////////////////////////////////////////////////////////
							// -----------------------------------------------------------------------//
							// ---------------- This will render all other Tiles ---------------------//
							else {
								// Render tile
								gTiles.setAlpha(tile[i].alpha);
								gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
							}
							// -----------------------------------------------------------------------//
							// ---------------- This will render all other Tiles ---------------------//
							////////////////////////////////////////////////////////////////////////////
						}
					}
				}
			}
		}
	}
}

void Tile::RenderOnTopOfPlayer(SDL_Renderer *gRenderer, Tile tile[], int layerToRender, int camx, int camy, SDL_Rect rTiles[]) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive) {

			// Render layer in hand
			if (hideOtherLayers)
			{
				if (layer == tile[i].layer)
				{
					gTiles.setAlpha(tile[i].alpha);
					gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
				}
			} else {

				// If player is front of Tile
				if (tile[i].PlayerBehindTile)
				{
					// Render all Tiles that are layer 1 and above
					if (layerToRender== tile[i].layer) {

						// For loop to loop through all 8 light Tiles
						for (int j=0; j<8; j++) {

							////////////////////////////////////////////////////////////////////////////
							// -----------------------------------------------------------------------//
							// ---------------- This will render ONLY light Tiles: -------------------//
							// ----------------------- id's 296 through 303 --------------------------//
							// ----------------------- id's 328 through 335 --------------------------//
							if (tile[i].id == 328 + j )
							{
								// Render top half of light tile
								gTiles.setAlpha(tile[i].alpha);
								int topLightTileId = tile[i].id-32;
								gTiles.render(gRenderer, tile[i].x - camx, tile[i].y-tile[i].h - camy, tile[i].w, tile[i].h, &rTiles[topLightTileId]);

								// Render bottom half of light tile
								gTiles.setAlpha(tile[i].alpha);
								gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
							}
							// ----------------------- id's 328 through 335 --------------------------//
							// ----------------------- id's 296 through 303 --------------------------//
							// ---------------- This will render ONLY light Tiles: -------------------//
							// -----------------------------------------------------------------------//
							////////////////////////////////////////////////////////////////////////////

							////////////////////////////////////////////////////////////////////////////
							// -----------------------------------------------------------------------//
							// ---------------- This will render all other Tiles ---------------------//
							else {
								// Render tile
								gTiles.setAlpha(tile[i].alpha);
								gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
							}
							// -----------------------------------------------------------------------//
							// ---------------- This will render all other Tiles ---------------------//
							////////////////////////////////////////////////////////////////////////////
						}
					}
				}


				// If player is above Tile
				if (!tile[i].PlayerBehindTile)
				{
					// Do nothing
				}
			}
		}
	}
}

void Tile::RenderUI(SDL_Renderer *gRenderer, Tile Tile[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (Tile[i].alive) {
			if (Tile[i].promptSelf) {
				std::stringstream tempsi;
				tempsi << "E";
				gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255, 255, 255}, gFont24);
				gText.render(gRenderer, Tile[i].x+Tile[i].w/2-gText.getWidth()/2-camx,
						Tile[i].y-gText.getWidth()*2-camy,
										gText.getWidth(), gText.getHeight());
			}
		}
	}
}

void Tile::renderTileDebug(SDL_Renderer *gRenderer, Tile tile[], int newMx, int newMy, int mex, int mey, int camx, int camy, SDL_Rect rTiles[]){
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			if (tile[i].layer == layer) {

				// If its a collision tile, render filled blue square on top left inside the Tile
				if (tile[i].collisionTile) {
					SDL_Rect tempr = {tile[i].x - camx, tile[i].y - camy, 4, 8};
					SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
					SDL_RenderFillRect(gRenderer, &tempr);
				} else {
					SDL_Rect tempr = {tile[i].x - camx, tile[i].y - camy, 4, 8};
					SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
					SDL_RenderFillRect(gRenderer, &tempr);
				}
			}
		}
	}
}

void Tile::RenderHand(SDL_Renderer *gRenderer, Tile tile[], int newMx, int newMy, int mex, int mey, int camx, int camy, SDL_Rect rTiles[]){
	// Render Tile info
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){

			// If mouse is hovering on Tile and we have the same layer in Hand
			if (tile[i].mouse && tile[i].layer == layer) {
				std::stringstream tempss;
				tempss << tile[i].layer;
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont12);
				gText.setAlpha(255);
				gText.render(gRenderer, tile[i].x+tile[i].w-gText.getWidth() - camx, tile[i].y - camy, gText.getWidth(), gText.getHeight());

				// If its a collision tile, render filled blue square on top left inside the Tile
				if (tile[i].collisionTile) {
					SDL_Rect tempr = {tile[i].x+6 - camx, tile[i].y+6 - camy, 10, 10};
					SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
					SDL_RenderFillRect(gRenderer, &tempr);
				} else {
					SDL_Rect tempr = {tile[i].x+6 - camx, tile[i].y+6 - camy, 10, 10};
					SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}

				// Render green square inside tile to let the editor know we are hovering over it
				SDL_Rect tempr = {tile[i].x+6 - camx, tile[i].y+6 - camy, tile[i].w-12, tile[i].h-12};
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}
		}
	}
	// Render tile in hand
	for (int j = 0; j < multiW; j++) {
		for (int h = 0; h < multiH; h++) {
			gTiles.setAlpha(110);
			gTiles.render(gRenderer, newMx+j*tilew, newMy+h*tileh, tilew, tileh, &rTiles[id]);

			// Render mouse coordinates snapped to grid
			SDL_Rect tempr = {newMx, newMy, tilew*multiW, tileh*multiH};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempr);
		}
	}
}

void Tile::setStatsBasedOnType(Tile tile[], int i) {

	// Every other Tile
	{
		tile[i].rectB.x = tile[i].x;
		tile[i].rectB.y = tile[i].y;
		tile[i].rectB.w = tile[i].w;
		tile[i].rectB.h = tile[i].h;
	}

	// Specific Tiles
	{
		// Wall tiles
		if (tile[i].id == 32) {
			tile[i].rectB.x = tile[i].x;
			tile[i].rectB.y = tile[i].y+(8*4);
			tile[i].rectB.w = tile[i].w;
			tile[i].rectB.h = tile[i].h/2;
		}
		if (tile[i].id == 33) {
			tile[i].rectB.x = tile[i].x;
			tile[i].rectB.y = tile[i].y+(8*4);
			tile[i].rectB.w = tile[i].w;
			tile[i].rectB.h = tile[i].h/2;
		}
		if (tile[i].id == 34) {
			tile[i].rectB.x = tile[i].x;
			tile[i].rectB.y = tile[i].y+(8*4);
			tile[i].rectB.w = tile[i].w;
			tile[i].rectB.h = tile[i].h/2;
		}

		// Top of yellow box tile
		if (tile[i].id == 197) {
			tile[i].rectB.x = tile[i].x;
			tile[i].rectB.y = tile[i].y+(11*4);
			tile[i].rectB.w = tile[i].w;
			tile[i].rectB.h = 2.5*4;
		}

		// Bottom of yellow box tile
		if (tile[i].id == 229) {
			tile[i].rectB.x = tile[i].x;
			tile[i].rectB.y = tile[i].y;
			tile[i].rectB.w = tile[i].w;
			tile[i].rectB.h = 8*4;
		}

		// Locked door
		if (tile[i].id == 306) {
			tile[i].w = 128;
			tile[i].h = 128;
			tile[i].clip = {272, 128, 32, 32};
		}

		// Unlocked door
		if (tile[i].id == 309) {
			tile[i].w = 128;
			tile[i].h = 128;
			tile[i].clip = {320, 128, 32, 32};
		}


		// Chest tile
		if (tile[i].id == 366) {
			tile[i].rectB.x = tile[i].x;
			tile[i].rectB.y = tile[i].y+(8*4);
			tile[i].rectB.w = tile[i].w;
			tile[i].rectB.h = 4*4;
		}
		if (tile[i].id == 367) {
			tile[i].rectB.x = tile[i].x;
			tile[i].rectB.y = tile[i].y+(8*4);
			tile[i].rectB.w = tile[i].w;
			tile[i].rectB.h = 4*4;
		}
	}

	// Default parameters
	{
		tile[i].promptSelf = false;
	}

}

void Tile::MoveTiles(Tile tile[], std::string direction){
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			if (direction == "left") {
				tile[i].x -= 64;
			}
			else if (direction == "right") {
				tile[i].x += 64;
			}
			else if (direction == "up") {
				tile[i].y -= 64;
			}
			else if (direction == "down") {
				tile[i].y += 64;
			}

		}
	}
}

void Tile::LoadData(Tile tile[], std::fstream &fileToReturn)
{
	//std::cout << "Attempting to load... Tile" << std::endl;
	// Load Tile count data
	fileToReturn >> this->tileCount;
	//std::cout << this->tileCount << std::endl;

	// Load rest of Tile data
	for (int i = 0; i < this->tileCount; i++) {
		if (!tile[i].alive) {
			fileToReturn >>  tile[i].x 				>>
							  tile[i].y 			>>
							  tile[i].w 			>>
							  tile[i].h 			>>
							  tile[i].id 			>>
							  tile[i].collisionTile >>
							  tile[i].layer 		>>
							  tile[i].alpha 		>>
							  tile[i].clip.x 		>>
							  tile[i].clip.y 		>>
							  tile[i].clip.w 		>>
							  tile[i].clip.h 		>>
							  tile[i].mouse 		>>
							  tile[i].screen 		>>
							  tile[i].alive;
		}

		// Set collision rects for specific Tiles
		for (int i = 0; i < this->tileCount; i++) {
			if (tile[i].alive) {

				// Set default parameters
				setStatsBasedOnType(tile, i);
			}
		}

		//break;
	}
}

// Save Tile data
std::string Tile::SaveData(Tile tile[])
{
	/////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------//
	//----------------------------- Save Everything in One File -------------------//

	// Save all tile data in a string first
	std::stringstream tempsstb;
	for (int i = 0; i < max; i++) {
		if (tile[i].alive) {
			tempsstb << tile[i].x 				<< " "
					 << tile[i].y 				<< " "
					 << tile[i].w  				<< " "
					 << tile[i].h  				<< " "
					 << tile[i].id  			<< " "
					 << tile[i].collisionTile 	<< " "
					 << tile[i].layer 			<< " "
					 << tile[i].alpha  			<< " "
					 << tile[i].clip.x 			<< " "
					 << tile[i].clip.y 			<< " "
					 << tile[i].clip.w 			<< " "
					 << tile[i].clip.h 			<< " "
					 << tile[i].mouse 			<< " "
					 << tile[i].screen 			<< " "
			   	   	 << tile[i].alive 			<< "\n";
		}
	}
	std::string temps;
	temps = tempsstb.str().c_str();

	// Commented out because this takes out the extra line thats empty at the end
	//temps = temps.substr(0, temps.size()-1);

	// Save number of tiles in World
	std::stringstream aVeryLongString;
	aVeryLongString << tileCount << "\n";

	// Save the data of all Tiles
	aVeryLongString << temps.c_str();
	//----------------------------- Save Everything in One File -------------------//
	//-----------------------------------------------------------------------------//
	/////////////////////////////////////////////////////////////////////////////////

	// Return save data for all Tiles
	return aVeryLongString.str().c_str();
}


