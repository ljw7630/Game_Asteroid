#ifndef __Myprogram_H__
#define __Myprogram_H__

#include "allHeader.h"
#include "Bullet.h"
#include "Player.h"
#include "Asteriod.h"
#include "OgreTest1.h"
#include "Ball.h"

#include <fmod_errors.h>
#include <fmod.hpp>
#include <string>
#include <windows.h>

#define TIME1 90
#define TIME2 80
#define MAXHP 5

const int minEnemy=10;
const int maxEnemy=20;
const int numBullet=10;
const int numKinds=3;
const int pYpos=296;
const int nYpos=-296;
const int pXpos=404;
const int nXpos=-404;
const int bulletDelay=35;
const int yawDelay=4;
const int newEnemyTime=2400;
const int showDelay=450;
const float originPlayerSpeed=140.0f;
const float originBulletSpeed=200.0f;
const float originAsteriodSpeed=100.f;
const float bigAsteriodR=20;
const float smallAsteriodR=10;

class MyProgram:public OgreTest1
{
private:

	Player *myPlayer;

	vector<Asteroid*> allAsteroids; 

	vector<Bullet *> allBullet;

	vector<Ball *> allBall;

	float myTimeDelta;

	float explosionTime;
	bool isExplosion;

	bool gameOver;
	bool win;

	int countLittleAsteriod;
	int countBall;
	float countNewEnemyTime;
	int countNewEnemy;
	int countBullet;

	int rebornPause;
	bool bReborn;
	bool bMove;

	// if your have N kinds of Asteriod, you have N kinds of Explosion
	SceneNode * allExplosion;

	OverlayElement* EleTime;
	Ogre::Overlay* mPanelOverlay;
	Ogre::Overlay* mEndOverlay;
	OverlayElement* EleEnd;
	float time;
	Timer m_timer;

	int gameLevel;
	int levelTime;
	int numEnemy;
	bool flagForGameLevel;
	bool flagForShowlevel1;
	float timeDeltaForLevel1;
	float timeDeltaForLevel2;
	bool flagForShowlevel2;

	FMOD::Sound * sound;
	FMOD::Sound * sound2;
	//FMOD::Sound * passSound;
	//FMOD::Sound * overSound;
	FMOD::Channel * channel;
	FMOD::Channel * channel2;
	int soundCount;
	//FMOD::Channel * channel3;
	//FMOD::Channel * channel4;
	FMOD::System * fmodSystem;
	//bool playing ;

public:
	void mySetup();
	void update(const float timeDelta);
	void processEvents();
	void bulletBehavior(const float timeDelta);
	void explosionBehavior(const float timeDelta);
	void asteriodBehavior(const float timeDelta);
	void ballBehavior(const float timeDelta);
	void isGameOver();
	void createNewEnemy(const float timeDelta);
	void rebuildGame();
	void freeGame();
	void timeBoard(const float timeDelta);
	void hideAll();
	void showAll();
	void outOfBount(myEntity *e);
};

#endif