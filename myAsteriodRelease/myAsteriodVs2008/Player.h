#pragma once

#include "myEntity.h"

class Player : public myEntity
{
public:
	int myHP;
	bool myFiring;

	void zRotation(float r);
	bool isPlayerDied(myEntity *e);
	void reborn(float pauseForBirth,float timeDelta);
};

void Player::zRotation(float r)
{
	float tx;
	float ty;
	tx=myDirection.x*cos(r)-myDirection.y*sin(r);
	ty=myDirection.x*sin(r)+myDirection.y*cos(r);
	myDirection.x=tx;
	myDirection.y=ty;
	myDirection.normalise();
}

bool Player::isPlayerDied(myEntity *e)
{
	if(myPos.distance(e->myPos)<15)
	{
		myHP--;
		return true;
	}
	return false;
}

void Player::reborn(float pauseForBirth,float timeDelta)
{
	pauseForBirth+=timeDelta;
	if(pauseForBirth>6*timeDelta)
	{
		//mySceneNode->setPosition(myPos);
		mySceneNode->setVisible(true);
		mySceneNode->setPosition(myPos);
	}
	if(pauseForBirth>12*timeDelta)
	{
		pauseForBirth=0;
	}
}