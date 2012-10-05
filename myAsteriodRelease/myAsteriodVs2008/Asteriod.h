#pragma once

#include "myEntity.h"

class Asteroid : public myEntity
{
public:
	int kindsOfAsteriod;
	bool isBig;
	bool isAlive;
	float myTimeDelta;
	Asteroid(SceneManager * sceneManager, Vector3 * pos, char * name ,int whichAsteriod,bool whetherBig);
	void moveToPlayer(myEntity *thePlayer,const float timeDelta);
	void moveStraight(myEntity *m,const float timeDelta,bool reborn);
	bool asteriodCollision(Asteroid *e,const float timeDelta);
	void randomDirection();
	bool isClose(myEntity *e,const float timeDelta);
};