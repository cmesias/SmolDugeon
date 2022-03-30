/*
 * Particless.h
 *
 *  Created on: Dec 16, 2016
 *      Author: Carl
 */



#ifndef LOCAL_PARTICLESS_H_
#define LOCAL_PARTICLESS_H_

#include "LTexture.h"
#include "Helper.h"
#include "Game/TextNFont.h"

class Particle: public Helper, public TextNFont {

public:	// other variables
	int count;
	const int max = 800;
	LTexture gBullet;				// Bullet Texture blue
	LTexture blue_block;					// Blue block, this will be used to create "bits" that come off of a block when hit
	LTexture gSlash;
	LTexture gParticles;					// Particle Textures
	SDL_Rect cParticles[ 6 ];				// [0: Blue], [1: Green], [2: Orange], [3: Red], [4: White], [5: Yellow] Particle

public:	// variables
	float x2, y2;							// particle center
	int radius;								// particle radius
	float x, y;
	float w, h;
	float *xFollow;
	float *yFollow;
	bool follow = false;
	float alpha;
	float alphaspeed;
	int time;
	int deathTimer;
	int deathTimerSpeed;
	float angle;
	double vX, vY;
	double speed;
	double angleSpe, angleDir;
	bool onScreen;
	bool alive;
	bool collide;
	std::string side;
	/* -1: damages enemies, counter attack from Player, does NOT damage enemy Particles
	 * 0: damages enemies, damages enemy Particles
	 * 1: damages player
	 * 2: stars, no damage
	 * 3: blue block bits, no damage
	 */
	int type;
	int damage;
	float dmgToParticles;	// this is for damage to other particles
	float health;
	SDL_Color color;
	int layer;

	bool sizeDeath;
	float deathSpe;

	int ptimer = 0;

	double splatSpeed;			// splat speed
	double splatDistance;		// how many pixels the particle will travel before stopping, or -1 for forever

public:	// basic functions
	void init(Particle particle[]);
	void load(SDL_Renderer* gRenderer);
	void free();

public:	// functions

	// Goes in one direction
	void spawnParticleAngle(Particle particle[], int type,
			float spawnX, float spawnY,
			int spawnW, int spawnH,
			double angle, double speed,
			float damage, float dmgToParticles, float health,
			SDL_Color color, int layer,
			int angleSpe, int angleDir,
			float alpha, float alphaspeed,
			float deathTimer, float deathTimerSpeed,
			bool sizeDeath, float deathSpe,
			double splatDistance = 1, double splatSpeed = 0.0);

	// Follows the target
	void spawnParticleAngleFollow(Particle particle[], int type,
			float spawnX, float spawnY,
			int spawnW, int spawnH,
			double angle, double speed,
			float damage, float dmgToParticles, float health,
			SDL_Color color, int layer,
			int angleSpe, int angleDir,
			int alpha, int alphaspeed,
			int deathTimer, int deathTimerSpeed,
			bool sizeDeath, float deathSpe,
			double splatDistance = 1, double splatSpeed = 0.0,
			bool follow=false, float *xFollow=NULL, float *yFollow=NULL);

	// Spawn blood VFX
	void spawnBloodVFX(Particle particle[], float targetX, float targetY,
					   	   	   	   	   	    float targetW, float targetH,
											SDL_Color color);

	// Spawn no more mana VFX
	void spawnNoMoreManaVFX(Particle particle[], float centerX, float centerY);

	// Spawn Slash Attack particle
	void spawnSlashAttackProjectile(Particle particle[], float spawnX, float spawnY,
			  float spawnW, float spawnH, float damage);


	// Spawn Red Angel (type: 0) Attack
	void spawnRedAngelAttack(Particle particle[], float spawnX, float spawnY,
			float spawnW, float spawnH,
			float angle, float damage, float speed);

	void spawn360RedAttack(Particle particle[], float spawnX, float spawnY,
			float spawnW, float spawnH,
			float h, float speed, SDL_Color color);

	// Spawn attack: Quick 360 burst of blue colored projectiles PART 1
	void spawnQuick360BlueAttackPart1(Particle particle[], float spawnX, float spawnY,
			float spawnW, float spawnH,
			float h, float speed, SDL_Color color);

	// Spawn attack: Quick 360 burst of blue colored projectiles PART 2
	void spawnQuick360BlueAttackPart2(Particle particle[], float spawnX, float spawnY,
			float spawnW, float spawnH,
			float h, SDL_Color color);

	void spawnExplosion(Particle particle[], float x, float y, int size);

	void RemoveAll(Particle particle[]);

	/*void fireParticle(Particle particle[], int type, int damage,
										   int spawnx, int spawny, int w, int h,
										   double speed, float vX, float vY,
										   int alphaspeed, int alpha,
										   int deathTimer, int deathTimerSpeed, double angle, double angleSpe, double angleDir,
										   SDL_Color color, int layer);
	void fireParticle2(Particle particle[], int type, int damage, int spawnx, int spawny, int w, int h, int targetx,int targety,
											int alphaspeed, int alpha,
											int deathTimer, int deathTimerSpeed,
											double speed,
											SDL_Color color, int layer);*/
	void updateBulletParticles(Particle particle[], int mapX, int mapY, int mapW, int mapH);

	void renderBulletParticle(Particle particle[], int camX, int camY, float playerZ, SDL_Renderer* gRenderer);

	void renderBulletParticleDebug(Particle particle[], int camX, int camY, SDL_Renderer* gRenderer);

public:	// Star
	void genStars(Particle particle[], int screnWidth, int screenHeight);
	void updateStarParticles(Particle particle[], int mapX, int mapY, int mapW, int mapH);
	void renderStarParticle(Particle particle[], int camX, int camY, float playerZ, SDL_Renderer* gRenderer);
};

#endif /* LOCAL_PARTICLESS_H_ */
