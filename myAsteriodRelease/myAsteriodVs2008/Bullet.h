#pragma once
#include "myEntity.h"

class Bullet : public myEntity
{
public:
	bool used;
	Bullet(SceneManager * sceneManager, Vector3 pos, char * name);
	void move(float timeDelta);
};

Bullet::Bullet(SceneManager *sceneManager, Ogre::Vector3 pos, char *name)
{
	myPos=pos;
	Entity *entity;
	entity = sceneManager->createEntity(name,"geosphere4500.mesh");
	mySceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	mySceneNode->attachObject(entity);
	mySceneNode->setPosition(myPos);
	mySceneNode->scale(0.01,0.01,0.01);
	mySceneNode->roll(Radian(-Math::PI / 2));
}

void Bullet::move(float timeDelta)
{
	myPos.x+=myDirection.x*mySpeed*timeDelta;
	myPos.y+=myDirection.y*mySpeed*timeDelta;
	myPos.z+=myDirection.z*mySpeed*timeDelta;
	mySceneNode->pitch(Radian(timeDelta * mySpeed * 0.25f));
	mySceneNode->setPosition(myPos);
}