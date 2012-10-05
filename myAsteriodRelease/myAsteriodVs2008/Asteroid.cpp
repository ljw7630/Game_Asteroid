#include "Asteriod.h"
#include <string>

using namespace std;

Asteroid::Asteroid(SceneManager * sceneManager, Vector3 * pos, char * name,int whichAsteriod,bool whetherBig)
{
	myPos = * pos;
	Entity * entity; 
	if(whichAsteriod==0)
		entity = sceneManager->createEntity(name, "ogrehead.mesh");
	else if(whichAsteriod==1)
		entity = sceneManager->createEntity(name, "facial.mesh");
	else
		entity = sceneManager->createEntity(name, "knot.mesh");
	mySceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	mySceneNode->attachObject(entity);
	mySceneNode->setPosition(myPos);
	if(whetherBig)
	{
		if(whichAsteriod==0)
			mySceneNode->scale(.7, .7, .7);
		else if(whichAsteriod==1)
			mySceneNode->scale(0.6, 0.6, 0.6);
		else
			mySceneNode->scale(0.2, 0.2, 0.2);
	}
	else
	{
		if(whichAsteriod==0)
			mySceneNode->scale(.4, .4, .4);
		else if(whichAsteriod==1)
			mySceneNode->scale(0.25, 0.25, 0.25);
		else
			mySceneNode->scale(0.15, 0.15, 0.15);
	}
	kindsOfAsteriod=whichAsteriod;
	isBig=whetherBig;
}

void Asteroid::moveToPlayer(myEntity *thePlayer,const float timeDelta)
{
	Vector3 vec=this->myPos-thePlayer->myPos;
	vec.normalise();
	//Vector3 verticalVec;
	myDirection=vec;
	int randomValue=rand();
	myPos.x-=vec.x*mySpeed*timeDelta;
	myPos.y-=vec.y*mySpeed*timeDelta;
	//myPos.x-=verticalVec.x*mySpeed*timeDelta;
	//myPos.y-=verticalVec.y*mySpeed*timeDelta;
	mySceneNode->setPosition(myPos);
}

bool Asteroid::asteriodCollision(Asteroid *e,const float timeDelta)
{
	if(myPos.distance(e->myPos)<40&&isClose(e,timeDelta))
	{
		swap(myDirection,e->myDirection);
		myDirection.z=0;
		myDirection.normalise();
		e->myDirection.z=0;
		e->myDirection.normalise();
		return true;
	}
	else
		return false;
}

void Asteroid::moveStraight(myEntity *e,const float timeDelta,bool reborn)
{
	myTimeDelta+=timeDelta;
	if(myTimeDelta>4*timeDelta)
	{
		if(kindsOfAsteriod==2)
			mySceneNode->yaw(Radian(-Math::PI / 2));
		if(kindsOfAsteriod==0)
			mySceneNode->pitch(Radian(-Math::PI / 2));
		myTimeDelta=0;
	}
	if(!reborn && kindsOfAsteriod==1&&myPos.distance(e->myPos)<100)
	{
		moveToPlayer(e,timeDelta);
	}
	else
	{
		myPos.x+=myDirection.x*mySpeed*timeDelta;
		myPos.y+=myDirection.y*mySpeed*timeDelta;
		mySceneNode->setPosition(myPos);
	}
}

bool Asteroid::isClose(myEntity *e,const float timeDelta)
{
	Vector3 nextPos0=myPos+myDirection*mySpeed*timeDelta;
	Vector3 nextPos1=e->myPos+e->myDirection*mySpeed*timeDelta;
	if(myPos.distance(e->myPos) >= nextPos0.distance(nextPos0))
		return true;
	return false;
}

void Asteroid::randomDirection()
{
	int r1=rand();
	int r2=rand();
	myDirection=Vector3(r1,r2,0);
	myDirection.normalise();
	if(r1%2==0)
		myDirection.x=-myDirection.x;
	if(r2%2==0)
		myDirection.y=-myDirection.y;
}