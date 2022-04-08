/*
 * Players.h
 *
 *  Created on: Dec 18, 2016
 *      Author: Carl
 */

#ifndef LOCAL_PLAYERS_H_
#define LOCAL_PLAYERS_H_

#include <SDL2/SDL_ttf.h>

#include "../LWindow.h"
#include "../LTexture.h"
#include "../Particless.h"
#include "../Helper.h"
#include "Maps.h"
#include "Tiles.h"
#include "Tilec.h"
#include "Object.h"
#include "TextNFont.h"

#include "../Settings.h"

//Player
class Players: public Helper, public TextNFont {
public:
	enum Result { QUIT, LOADMENU, PLAYGAME, QUITPLAYGAME };
	Settings settings;

public:
	// Joystick Sensitivity
	const int JOYSTICK_DEAD_ZONE = 8000;
	void OnKeyDown(SDL_Keycode sym );
	void OnKeyUp(SDL_Keycode sym );
	void mouseClickState(SDL_Event &e);

	// Mouse stuff
	bool leftclick;
	bool rightclick;
	bool test;

public:	// resources
	// Local resources
	LTexture gPlayer;
	LTexture gPlayerShadow;
	LTexture gShield;
	LTexture gSwords;

	/* Sword types
	 * 0  - 10: Fist & Swords
	 * 11 - 12: Rapier and Hammer
	 * 13 - 18: Better Swords
	 * 19 - 22: Broken Swords
	 * 23: 		Bomb
	 * 24: 		Heart
	 * 25: 		Coin
	 * 26: 		Silver key
	 * 27: 		Gold key
	 */
	SDL_Rect rSwords[28];

	/*
	 * 0-3: Walk
	 * 4:   Before slash
	 * 5:   Slash
	 * 6:   Parry
	 * 7:   Down Stab
	 * 8:   Dash frame 0
	 * 9:   Dash frame 1
	 */
	SDL_Rect rPlayer[10];
	SDL_Rect rShield[7];

	/*
	 * 0-3: Walk
	 * 4:   Before slash
	 * 5:   Slash
	 * 6:   Parry
	 * 7:   Down Stab
	 * 8:   Dash frame 0
	 * 9:   Dash frame 1
	 */
	int sprite_index;

	/*
	 * 0: Keyboard
	 * 1: Keyboard + Mouse
	 */
	int ControlsPreference = 1;	// default: 0

public:	// variables
	std::string name;
	int timer = 0;
	int realw = 48, realh = 48;			// object size in actual pixel size
	int w = 24, h = 24;					// object size in collision pixels
	float x = 0,
		  y = 0;
	float armX;							// This is where we will attach the sword to
	float armY;
	float x2, y2;						// player center
	float angle 		= 0.0,
		   radians 		= 0.0,
		   radianCos 	= 0.0,
		   radianSin 	= 0.0;
	float vX 			= 0.0;
	float vY 			= 0.0;
	const float velMax 	= 4;
	const float velSpeed= 2;
	bool camlocked 		= true;
	bool shift 			= false;
	bool deathScreen	= false;
	bool alive;

	// Movement
	std::string facing;
	bool moveleft 		= false,
		 moveright 		= false,
		 moveup 		= false,
		 movedown		= false;
	bool moving = false;

	/*
	 * Parrying small blue Bullet from Boss:		score += 1;
	 * Parrying small red Bullet from Boss:			score += 2;
	 * Parrying large red Bullet from Boss:			score += 4;
	 * Hitting Boss directly w/ Slash Projectile: 	score += 5
	 * Hitting Boss directly w/ Sword: 				score += 5
	 * Hitting Boss with counter is:				score += 10
	 */
	unsigned int score	= 0;
	unsigned int highscore = 0;

	double tempc, temps;
	SDL_Rect continueButton[3];

	// High-Score display
	std::string highList[10];
	int indx 		= 0;
	float travel 	= 0.0;
	int position	= 0;
	int position2	= 0;
	int dir 		= 1;

public: // Attack variables

	SDL_RendererFlip flipW;
	float attackTimer;
	float slashAtkSpe = 1;	// default: 1 or 3 for fast attacks
	int attackFrame;
	int attackType;	// 0: slash, 1: down stab
	int stunTimer;
	bool attack;
	bool stunned;
	bool clash;
	bool spawnAttack;

    //float *tempXX;
    //float *tempYY;

public:	// functions
	// Functions

	void Load(SDL_Renderer* gRenderer);

	void Free();

	void Init(float spawnX, float spawnY, std::string newName);

	// Called when player loses all their health
	void RespawnPlayer();

	// Called when player loses all their lives
	void ResetLivesAndPlayer();

	bool checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);

	// Get audio files
	//void getAudio(Mix_Chunk* sLazer);

	// Player shoot
	void fire(Particle particle[], Particle &p_dummy, Mix_Chunk* sCastSFX, int mx, int my);

	void Update(Map &map,
				Particle particle[], Particle &p_dummy,
				Tile &tl, Tile tile[],
				Tilec &tlc, Tilec tilec[],
				Object &obj, Object object[],
				int mx, int my, int camx, int camy,
				float spawnX, float spawnY,
				LWindow gWindow, SDL_Renderer* gRenderer,
				SDL_Color color,
				bool &RestartLevel,
				int LevelWeLoaded, bool &ShakeCamera);

	void Render(int mx, int my, int camX, int camY, LWindow gWindow,
				SDL_Renderer* gRenderer,
				SDL_Color color, int &PARTICLES);

	void RenderUI(SDL_Renderer *gRenderer, int camX, int camY, int CurrentLevel);

	void RenderDebug(SDL_Renderer* gRenderer, int camX, int camY);

private:	// Private variables

	// Walktimer for sprite usage
	double walkTimer;

	// Walktimer visual effects
	double walkTimerVFX;

private:	// Private variables

private:	// Abilities

	// Health
	//int hearts 			= 3;
	//int health			= 100;
	//const int healthMax	= 100;

	// Fire-rate
	double particleW 	= 48;
	double particleH 	= 48;
	bool shootDelay 	= false;
	bool initialshot 	= false;
	bool trigger 		= false;
	//float AttackSpeed;	// Attack speed
	float shootTimer;		// Attack speed

	//float damage;			// Sword damage
	//float castDamage;		// Cast damage
	//float damageMultipler;	// Parrying will increase damage multiplier

	//float maxMana;		// Max mana
	//float mana;			// Mana
	//float manaRegenSpeed;	// Mana
	float manaRegenTimer;	// 1 second timer for mana regen;
	float manaGainOnParry;	// Gain mana if you parry successfully

	float knockBackPower;	// Knock back power

	// Parry state
	bool parry;
	//float parryLength;	// Parry length: 15 default
	float parryTimer;		// Parry timer
	float parryCDTimer;		// Parry cooldown timer tracker
	float parryCDMax;		// Parry cooldown, default: 60*3

	// Dash state
	bool dash;							// Dash ability
	float dashSpeed;					// default: 15
	float dashLength;					// default: 10
	const float dashCooldown 	= 60;	// default: 60
	float dashCounter;					// default: 0
	float dashCoolCounter;				// default: 0

	// Invurnerable state
	int invurnerableFrame;
	int invurnerableTick;
	int invurnerableT;
	bool invurnerable;

private:	// Save these player stats in a file. This will be the players save data.

	// Inventory
	int silverKeys;						// Keys
	int goldKeys;						// Keys
	int coins;							// Coins
	const int silverKeysMax = 9999;		// Keys,  	Default: 99
	const int goldKeysMax = 9999;		// Keys,  	Default: 99
	const int coinsMax = 9999;			// Coins,  	Default: 99
	int bombs;							// Bombs
	const int bombsMax = 99;			// Bombs,	Deafult: 99

	// Health
	int hearts;							// Default: 3
	const int heartsMax = 99;			// Default: 99
	float health;						// Default: 100
	float healthDecay;					// Default: 100
	float healthMax;					// Default: 100

	// Damage
	float damage;			// Sword damage
	float castDamage;		// Cast damage
	float damageMultipler;	// Parrying will increase damage multiplier
	float castAtkSpe;		// Cast atk speed - deprecated, delete?

	// Mana
	float mana;				// Mana
	float maxMana;			// Max mana
	float manaRegenSpeed;	// Mana

	// Parry
	float parryLength;		// Parry length: 15 default

private:	// Official player control keys

	bool pressedEquipKey;

private:	// Variables used in Textures

	const float xOffset = 0;
	const float yOffset = -29;
	int shadowW = 44;

	/* Sword Types (TODO [ ] (3-25-2022) - save this in player data)
	 *
	 * 0: Wooden Sword
	 * 1: Rusty Sword
	 * 2: Iron Sword
	 *
	 *
	 * 10: Red Sword					10 x 21
	 *
	 *
	 * 11: Tool? 						7 x 17
	 * 12: Hammer 						10 x 21
	 *
	 * 13: Thicc Sword 					10 x 25
	 * 14: Long Rapier 					8 x 27
	 * 15: Golden Long Sword 			10 x 29
	 * 16: Silver Sword 				6 x 27
	 * 17: Silver Thicc Sword 			10 x 24
	 * 18: Silver Thicc Sword w/ Gold 	10 x 24
	 *
	 * 19: Broken Iron sword short 		6 x 9
	 * 20: Broken Iron sword 			6 x 12
	 * 21: Broken Golden sword 			6 x 12
	 * 22: Broken Silver sword 			6 x 12
	 *
	 * 23: Bomb							14 x 13
	 * 24: Heart 						10 x 10
	 * 25: Coin 						10 x 10
	 * 26: Silver key 					5 x 10
	 * 27: Gold key 					5 x 10
	 */
	int swordInHand_Index = 0;
	int swordW = 20;
	int swordH = 42;

	//--------------- Sword offsets ---------------
	// X offset Sword Slashing Left
	// X offset Sword Slashing Left
	const float xOffSetFistPunchLeft = -15;
	const float xOffSetFistPunchRight = 15;

	// X offset Sword Slashing Left
	const float xOffSetSwordSlashingLeft = -24;

	// X offset Sword Slashing Left
	const float xOffSetSwordSlashingRight = 30;

	// X offset walking right and left
	//const float xOffSetSwordWalkAndDash = 30;

	// Y offset Sword default
	const float yOffSetSword = 3;

	// Y offset Sword Slashing
	const float yOffSetSwordSlashing = 15;

	//--------------- Player offsets ---------------
	// X offset Player Slashing Left
	const float xOffSetSlashingLeft = -48;

	// X offset Player Walking Right
	const float xOffSetWalkingRight = -12;

public:

	// X offset walking right and left
	const float xOffSetSwordWalkAndDash = 30;

public:	// Functions to do stuff?

	void SlashAttack();

	void ActivateParry();

	void ActivateDash();

public:	// Mutator functions

	// Move x pos
	float moveX(float value);

	// Move y pos
	float moveY(float value);

	// Applies invurnerability to Player
	void ActivateInvurnerability();

	// Stops parry but goes on cool down
	void StopParrying();

	// Stops parry but reset cool down to 0
	void ResetParry();

	// Extend parry duration, and Gain mana back
	void ExtendParryDuration();

	// Increase Silver Keys
	void IncreaseSilverKeys(int value = 1);

	// Increase Gold Keys
	void IncreaseGoldKeys(int value = 1);

	// Increase Coins
	void IncreaseCoins(int value = 1);

	// Increase Bombs
	void IncreaseBombs(int value = 1);

	// Increase Hearts
	void IncreaseHearts(int value = 1);

	// Increase Health
	void IncreaseHealth(float value);

	// Increase Score
	void IncreaseScore(float value);

	// Shorten parry cool down
	void ShortenParryCD(float value);

	// Stop movement
	void StopMovement();

	// Stop Slash Attack
	void StopSlashAttack();

	// Stop dashing
	void StopDashing();

	// Reset dashing
	void ResetDashing();

	// Equip a sword
	void EquipSword(int swordInHand_Index, float damage, float slashAtkSpe);

	// Set false to equip key state
	void stopEquipState();

public:	// Accessor functions

	// Get x pos
	float getX();

	// Get y pos
	float getY();

	// Get width
	float getW();

	// Get height
	float getH();

	// Get left pos
	float getLeftSide();

	// Get right pos
	float getRightSide();

	// Get top pos
	float getTopSide();

	// Get bottom pos
	float getBottomSide();

	// Get player damage
	float getDamage();

	// Get player cast damage
	float getCastDamage();

	// Get x center of player
	float getCenterX();

	// Get y center of player
	float getCenterY();

	// Get value depending on direction facing
	float getXDir();

	// Get knockback power
	float getKnockBackPower();

	// Get status of invurnerability
	float getInvurnerableStatus();

	// Get status of parrying
	float getParryStatus();

	// Get status of dashing
	float getDashStatus();

	// Get score
	float getScore();

	// Get equip key state
	bool getEquipState();

	// Check if item in hand is this index
	int getItemEqipped(int checkThisIndex);

	// Get item index in hand
	int getItemID();

	// Return number of Coins keys for Player
	int getCoins();

	// Return number of Silver keys for Player
	int getSilverKeys();

	// Return number of Gold keys for Player
	int getGoldKeys();

public:

	// Reset High Scores
	void ResetHighScore(int LevelWeLoaded);

	// Load previous high score from PlayGame.cpp
	void ApplyHighScore(float previousHighScore);

	// Save current highs-core for current Level
	void SaveHighScore(int LevelToLoad);


public:	// Change volume

	void changeVolume(float newVolume);

};

#endif /* LOCAL_PLAYERS_H_ */
