#include "myEntity.h"

class Ball:public myEntity
{
private:
	bool exist;
public:
	Ball(SceneManager * sceneManager, Vector3 pos,char *name,float speed)
	{
		myPos = pos;
		mySpeed = speed;

		Entity * entity; 
		entity = sceneManager->createEntity(name, "sphere.mesh");
		mySceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
		mySceneNode->attachObject(entity);
		mySceneNode->setPosition(myPos);

		exist = true;
		mySceneNode->scale(0.05,0.05,0.05); 
		mySceneNode->setVisible(true);

		myDirection=Vector3(rand(),rand(),-150);
		myDirection.normalise();
		//_sceneNode->roll(Radian(-Math::PI / 2)); 
	}

	bool getExist() {return exist;}
	void setExist(bool e)  {exist = e;}
	void move(float timeDelta);
};

void Ball::move(float timeDelta)
{
	myPos.x+=timeDelta*myDirection.x*mySpeed;
	myPos.y+=timeDelta*myDirection.y*mySpeed;
	mySceneNode->setPosition(myPos);
}