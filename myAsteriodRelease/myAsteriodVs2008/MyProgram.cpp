#include "myprogram.h"


void main()
{
	srand(time(NULL));
	MyProgram myProgram;
	myProgram.go();
}

void MyProgram::mySetup()
{
	FMOD_RESULT result = FMOD::System_Create(&fmodSystem);		// Create the main system object.
	if (result != FMOD_OK)
	{
		char errorMsg[255];
		sprintf(errorMsg, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		cout << errorMsg;
	}

	result = fmodSystem->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
	if (result != FMOD_OK)
	{
		char errorMsg[255];
		sprintf(errorMsg, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		cout << errorMsg;
	}

	string path = "Back.mid";
	fmodSystem->createSound(path.c_str(), FMOD_2D, 0, &sound);
	sound->setMode(FMOD_LOOP_NORMAL);
	fmodSystem->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);

	/*string path = "Back.mid";
	string path2 = "Explode.wav";
	string path4 = "Celebrate.wav";
	string path5 = "GameOver.wav";
	// Do this once for every sound you want to play
	
	result = fmodSystem->createSound(path.c_str(), FMOD_2D, 0, &sound);
	if (result != FMOD_OK)
	{
		string errorMsg = "Could not load sound file: " + path;
		cout << errorMsg;
	}
	sound->setMode(FMOD_LOOP_NORMAL);

	result = fmodSystem->createSound(path2.c_str(), FMOD_2D, 0, &sound2);
	if (result != FMOD_OK)
	{
		string errorMsg = "Could not load sound file: " + path;
		cout << errorMsg;
	}

	fmodSystem->createSound(path4.c_str(), FMOD_2D, 0, &passSound);
	fmodSystem->createSound(path5.c_str(), FMOD_2D, 0, &overSound);

	// Do this to play the sound
	
	result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
	if (result != FMOD_OK)
	{
		string errorMsg = "Could not play sound file: " + path;
		cout << errorMsg;
	}


	// Put this in your update function somewhere
	playing = true;*/



	mPanelOverlay = OverlayManager::getSingleton().getByName("Asteroid/PanelOverlay");
	mEndOverlay = OverlayManager::getSingleton().getByName("End/PanelOverlay");
	mPanelOverlay->show();
	gameLevel=0;
	timeDeltaForLevel1=0;
	timeDeltaForLevel2=0;

	countNewEnemy=0;
	countBullet=0;
	countLittleAsteriod=0;
	countBall=0;

	win = false;
	myPlayer=new Player;

	_sceneManager->setSkyBox(true,"Examples/SpaceSkyBox",50);
	
	Entity * entity;
	
	entity = _sceneManager->createEntity("robot1", "RZR-002.mesh");	


	// set player
	myPlayer->mySceneNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
	myPlayer->mySceneNode->attachObject(entity);
	myPlayer->mySceneNode->pitch(Radian(Math::PI / 2));
	myPlayer->mySceneNode->yaw(Radian(-Math::PI));

	rebuildGame();

	Ogre::String name[4]=
	{"Explosion0","Explosion1","Explosion2","Explosion3"};
	Ogre::String sname[4]=
	{"Examples/Smoke","Examples/EnvMappedRustySteel",
	"Examples/DarkMaterial","Examples/MorningSkyBoxHDRfp"};

	// set explosion
	ParticleSystem *par= _sceneManager->createParticleSystem(name[0],sname[0]);
	allExplosion=_sceneManager->getRootSceneNode()->createChildSceneNode();
	allExplosion->attachObject(par);
	allExplosion->setVisible(false);
	allExplosion->setPosition(1000,1000,1000);

}

void MyProgram::processEvents()
{
	OgreTest1::processEvents();
}

void MyProgram::update(float timeDelta)
{
	if(flagForShowlevel1==true)
	{
		timeDeltaForLevel1+=timeDelta;
		hideAll();
		EleEnd = OverlayManager::getSingleton().getOverlayElement("End");
		EleEnd->setCaption("Level One!");
		mEndOverlay->show();
		if(timeDeltaForLevel1>showDelay*timeDelta)
		{
			flagForShowlevel1=false;
			timeDeltaForLevel1=0;
			mEndOverlay->hide();
			showAll();
		}
	}
	else if(flagForShowlevel2==true)
	{
		timeDeltaForLevel2+=timeDelta;
		hideAll();
		EleEnd = OverlayManager::getSingleton().getOverlayElement("End");
		EleEnd->setCaption("Level Two!");
		mEndOverlay->show();
		if(timeDeltaForLevel2>showDelay*timeDelta)
		{
			flagForShowlevel2=false;
			timeDeltaForLevel2=0;
			mEndOverlay->hide();
			showAll();
		}
	}
	else if(!gameOver)
	{
		timeBoard(timeDelta);

		isGameOver();
		
		if(bMove)
		{
			if(_keyboard->isKeyDown(KC_SPACE)&&(!myTimeDelta||myTimeDelta>timeDelta*bulletDelay))
			{
				myTimeDelta=0;
				for(int i=0;i<allBullet.size();++i)
				{
					if(allBullet[i]->used==false)
					{
						allBullet[i]->used=true;
						allBullet[i]->mySceneNode->setVisible(true);
						allBullet[i]->myDirection=myPlayer->myDirection;
						allBullet[i]->myPos=myPlayer->myPos;
						break;
					}
				}
			}

			if (_keyboard->isKeyDown(KC_LEFT))
			{
				Vector3 tempVec;
				tempVec.x=-myPlayer->myDirection.y;
				tempVec.y=myPlayer->myDirection.x;
				tempVec.z=myPlayer->myDirection.z;
				myPlayer->myPos.x = myPlayer->myPos.x + tempVec.x * myPlayer->mySpeed * timeDelta;
				myPlayer->myPos.y = myPlayer->myPos.y + tempVec.y * myPlayer->mySpeed * timeDelta;
			}
			
			if (_keyboard->isKeyDown(KC_RIGHT))
			{
				Vector3 tempVec;
				tempVec.x=myPlayer->myDirection.y;
				tempVec.y=-myPlayer->myDirection.x;
				tempVec.z=myPlayer->myDirection.z;
				myPlayer->myPos.x = myPlayer->myPos.x + tempVec.x * myPlayer->mySpeed * timeDelta;
				myPlayer->myPos.y = myPlayer->myPos.y + tempVec.y * myPlayer->mySpeed * timeDelta;
			}

			if (_keyboard->isKeyDown(KC_UP))
			{
				myPlayer->myPos.x = myPlayer->myPos.x + myPlayer->myDirection.x * myPlayer->mySpeed * timeDelta;
				myPlayer->myPos.y = myPlayer->myPos.y + myPlayer->myDirection.y * myPlayer->mySpeed * timeDelta;
			}

			if (_keyboard->isKeyDown(KC_DOWN))
			{
				myPlayer->myPos.x = myPlayer->myPos.x - myPlayer->myDirection.x * myPlayer->mySpeed * timeDelta;
				myPlayer->myPos.y = myPlayer->myPos.y - myPlayer->myDirection.y * myPlayer->mySpeed * timeDelta;
			}

			//if(_keyboard->isKeyDown(KC_Q))
			//{
			//	myPlayer->mySceneNode->yaw(Radian(timeDelta)*yawDelay);
			//	myPlayer->zRotation(timeDelta*yawDelay);
			//}
			//if(_keyboard->isKeyDown(KC_E))
			//{
			//	myPlayer->mySceneNode->yaw(Radian(-timeDelta)*yawDelay);
			//	myPlayer->zRotation(-timeDelta*yawDelay);
			//}
			// set player position
			myPlayer->mySceneNode->setPosition(myPlayer->myPos);
			outOfBount(myPlayer);
			myTimeDelta+=timeDelta;
		}
		
		createNewEnemy(timeDelta);

		if(bReborn)
		{
			rebornPause++;
			if(rebornPause == 60) // one second
			{
				bMove = true;
				myPlayer->mySceneNode->setVisible(true);
			}
			if(rebornPause==120) // two second
			{
				bReborn = false;
				rebornPause = 0;
			}
		}

		bulletBehavior(timeDelta);
		
		explosionBehavior(timeDelta);

		asteriodBehavior(timeDelta);

		ballBehavior(timeDelta);
	}
	else
	{
		EleEnd = OverlayManager::getSingleton().getOverlayElement("End");
		if(win&&flagForGameLevel)
		{
			gameLevel++;
			flagForGameLevel=false;
		}
		if(gameLevel==2)
		{
			EleEnd->setCaption("You Pass!");
			mEndOverlay->show();
			/*string path4 = "Celebrate.wav";
			fmodSystem->createSound(path4.c_str(), FMOD_2D, 0, &passSound);
			fmodSystem->playSound(FMOD_CHANNEL_FREE, passSound, false, &channel);
			passSound->release();*/
		}
		else
		{
			if(win)
				EleEnd->setCaption("You Win!");
			else
			{
				EleEnd->setCaption("GameOver\n  Fail!");
				/*string path5 = "GameOver.wav";
				fmodSystem->createSound(path5.c_str(), FMOD_2D, 0, &overSound);
				fmodSystem->playSound(FMOD_CHANNEL_FREE,overSound, false, &channel);
				overSound->release();*/
			}
			mEndOverlay->show();	
		}
		if(_keyboard->isKeyDown(KC_RETURN))
		{
			if(gameLevel==2)
				gameLevel=0;
			mEndOverlay->hide();
			if(gameLevel==0)
				flagForShowlevel1=true;
			else
				flagForShowlevel2=true;
			freeGame();
			//sound->release();
			sound2->release();
			//SpassSound->release();
			//overSound->release();
			rebuildGame();
		}
	}
}

void MyProgram::rebuildGame()
{
	EleTime = OverlayManager::getSingleton().getOverlayElement("TimeLeft");
	myPlayer->myPos.x=0;
	myPlayer->myPos.y=-100;
	myPlayer->myPos.z=-150;
	myPlayer->myFiring=false;
	myPlayer->myHP=MAXHP;
	myPlayer->mySceneNode->setPosition(myPlayer->myPos);
	myPlayer->mySpeed=originPlayerSpeed;
	myPlayer->myDirection=Vector3(0,1,0);
	myPlayer->myDirection.normalise();

	// set asteriod
	int start, interval;
	start = -350;
	interval = 60;

	if(gameLevel==0)
		numEnemy=minEnemy;
	else if(gameLevel==1)
		numEnemy=maxEnemy;

	for (int i = 0 ; i < numEnemy ; ++i)
	{
		char name[200];
		sprintf(name, "Asteroid%d", countNewEnemy++);
		Vector3 pos = Vector3(start + i * interval, 80, -150); 
		Asteroid * asteroid = new Asteroid(_sceneManager, & pos, name,rand()%3,true);
		asteroid->myPos=pos;
		asteroid->isAlive=true;
		asteroid->mySpeed=originAsteriodSpeed;
		asteroid->randomDirection();
		asteroid->myTimeDelta=0;
		allAsteroids.push_back(asteroid);

		string path2 = "Explode.wav";
		//string path4 = "Celebrate.wav";
		//string path5 = "GameOver.wav";
		// Do this once for every sound you want to play
		
		fmodSystem->createSound(path2.c_str(), FMOD_2D, 0, &sound2);
		//fmodSystem->createSound(path4.c_str(), FMOD_2D, 0, &passSound);
		//fmodSystem->createSound(path5.c_str(), FMOD_2D, 0, &overSound);
		soundCount = 0;

		// Do this to play the sound
	}
	

	Vector3 tempFishPos=Vector3(0, -80, -160);

	for(int i=0;i<numBullet;++i)
	{
		char name[200];
		sprintf(name,"Bullet%d",countBullet++);
		Bullet * bullet = new Bullet(_sceneManager, tempFishPos,name);
		bullet->used=false;
		bullet->mySceneNode->setVisible(false);
		bullet->myPos=myPlayer->myPos;
		bullet->myDirection=myPlayer->myDirection;
		bullet->mySpeed=originBulletSpeed;
		allBullet.push_back(bullet);
	}

	// some factor
	myTimeDelta=0; // for bullet delay
	explosionTime=0; // for explosion delay
	isExplosion=false;
	gameOver=false; // is game over

	// for player reborn
	bReborn = true;
	rebornPause = 60;
	bMove = true;

	countNewEnemyTime=0;
	win = false;
	if(gameLevel==0)
	{
		levelTime=TIME1;
		flagForShowlevel1=true;
	}
	else if(gameLevel==1)
	{
		levelTime=TIME2;
		flagForShowlevel2=true;
	}
	time=levelTime;
	flagForGameLevel=false;

}

void MyProgram::timeBoard(const float timeDelta)
{
	/*m_RunTimerF = (float)this->m_timer.getMilliseconds()/1000;

	int decimal, sign;
    char *buffer;
    buffer =   _fcvt(m_RunTimerF,7,&decimal,&sign);
    String temp(buffer);
	String subtemp =   temp.substr(decimal,3);
	int t =   atoi(subtemp.c_str());
    int secondNum;
    secondNum = 30 - m_RunTimerF - 2;*/
	int decimal, sign;

	time = time-timeDelta;
	//time = time -(float)this->m_timer.getMilliseconds()/1000;
	
	char *buffer;
	buffer =  _fcvt(time,7,&decimal,&sign);
	String temp(buffer);
	String uptemp;
	String subtemp;
	if(time>=10)
	{
		uptemp = temp.substr(0,2);
		subtemp = temp.substr(decimal,2);
	}
	else if(time<10 && time>=1)
	{
		uptemp = temp.substr(0,1);
		subtemp = temp.substr(decimal,2);
	}
	else 
	{
		uptemp = '0';
		subtemp = temp.substr(0,2);
	}

	if(time<0.01) 
	{
			gameOver = true;
			win = false;
			EleTime->setCaption("TimeLeft: 0:00\n Life:  " + Ogre::StringConverter::toString(myPlayer->myHP));
	}
	else
		EleTime->setCaption("TimeLeft: " + uptemp +":"+ subtemp +"\n Life:  " + Ogre::StringConverter::toString(myPlayer->myHP));
	

	//EleTime->setCaption("TimeLeft: " + Ogre::StringConverter::toString(secondNum) +":"+ Ogre::StringConverter::toString(t) +"\n Life:  " + Ogre::StringConverter::toString(myPlayer->myHP));

}

void MyProgram::freeGame()
{
	for(int i=0;i<allAsteroids.size();++i)
	{
		delete allAsteroids[i]->mySceneNode;
		delete allAsteroids[i];
		allAsteroids.erase(allAsteroids.begin()+i);
		i--;
	}
	for(int i=0;i<allBall.size();++i)
	{
		delete allBall[i]->mySceneNode;
		delete allBall[i];
		allBall.erase(allBall.begin()+i);
		i--;
	}
	for(int i=0;i<allBullet.size();++i)
	{
		delete allBullet[i]->mySceneNode;
		delete allBullet[i];
		allBullet.erase(allBullet.begin()+i);
		i--;
	}
	allAsteroids.clear();
	allBall.clear();
	allBullet.clear();
}

void MyProgram::createNewEnemy(const float timeDelta)
{
	countNewEnemyTime+=timeDelta;
	if(countNewEnemyTime>levelTime/2+1)
	{
		countNewEnemyTime=0;
		int num=rand()%numEnemy+numEnemy/2;
		for (int i = 0 ; i < num ; ++i)
		{
			int rFactor0=rand();
			int rFactor1=rand();
			char name[200];
			sprintf(name, "Asteroid%d", countNewEnemy++);
			Vector3 pos;
			if(rFactor0%2==0)
			{
				if(rFactor1%2==0)
					pos = Vector3(rFactor0%pXpos, nYpos, -150); 
				else
					pos = Vector3(rFactor0%pXpos, pYpos, -150); 
			}
			else
			{
				if(rFactor1%2==0)
					pos = Vector3(nXpos, rFactor0%pYpos, -150); 
				else
					pos = Vector3(pXpos, rFactor1%pYpos, -150); 			
			}
			Asteroid * asteroid = new Asteroid(_sceneManager, & pos, name,rand()%3,true);
			asteroid->myPos=pos;
			asteroid->isAlive=true;
			asteroid->isBig=true;
			asteroid->mySpeed=originAsteriodSpeed;
			asteroid->randomDirection();
			allAsteroids.push_back(asteroid);
		}
	}
}

void MyProgram::bulletBehavior(const float timeDelta)
{
	for(int i=0;i<allBullet.size();++i)
	{
		if(allBullet[i]->used==true)
		{
			allBullet[i]->move(timeDelta);
		}
		if(allBullet[i]->myPos.x>pXpos||allBullet[i]->myPos.x<nXpos
			||allBullet[i]->myPos.y>pYpos||allBullet[i]->myPos.y<nYpos)
		{
			allBullet[i]->used=false;
			allBullet[i]->mySceneNode->setVisible(false);
			allBullet[i]->myPos=myPlayer->myPos;
			allBullet[i]->myDirection=myPlayer->myDirection;
			allBullet[i]->mySceneNode->setPosition(myPlayer->myPos);
		}
	}	
}

void MyProgram::explosionBehavior(const float timeDelta)
{
	if(isExplosion==true)
	{
		explosionTime+=timeDelta;
	}
	if(explosionTime>12*timeDelta)
	{
		allExplosion->setVisible(false);
		isExplosion=false;
		explosionTime=0;
		allExplosion->setPosition(1000,1000,1000);
	}

	for(int i=0;i<allBullet.size();++i)
	{
		for(int j=0;j<allAsteroids.size();++j)
		{
			if(allBullet[i]->used&&
				( (allBullet[i]->myPos.distance(allAsteroids[j]->myPos)<bigAsteriodR&&allAsteroids[j]->isBig==true) ||
				  (allBullet[i]->myPos.distance(allAsteroids[j]->myPos)<smallAsteriodR&&allAsteroids[j]->isBig==false) ) )
			{
				allExplosion->setPosition(allAsteroids[j]->myPos);
				allBullet[i]->mySceneNode->setVisible(false);
				allBullet[i]->used=false;
				allBullet[i]->mySceneNode->setPosition(myPlayer->myPos);
				allExplosion->setVisible(true);
				isExplosion=true;
 				fmodSystem->playSound(FMOD_CHANNEL_FREE, sound2, false, &channel2);
				soundCount++;
				if(soundCount>30)
				{
					sound2->release();
					string path2 = "Explode.wav";
					fmodSystem->createSound(path2.c_str(), FMOD_2D, 0, &sound2);
					soundCount = 0;

				}
				allAsteroids[j]->mySceneNode->setVisible(false);
				allAsteroids[j]->isAlive=false;
				if(allAsteroids[j]->isBig==true)
				{
					if(allAsteroids[j]->kindsOfAsteriod==1)
					{
						char name[200];
						sprintf(name,"Ball%d",countBall++);
						Ball *b= new Ball(_sceneManager,allAsteroids[j]->myPos,name,originAsteriodSpeed);
						allBall.push_back(b);
					}
					char name[200];
					Vector3 pos=Vector3(allAsteroids[j]->myPos);
					int tKindsOfAsteriod=allAsteroids[j]->kindsOfAsteriod;
					allAsteroids.erase(allAsteroids.begin()+j);
					sprintf(name,"LittleAsteriod%d",countLittleAsteriod++);
					Asteroid * asteroid = new Asteroid(_sceneManager, & pos,name ,tKindsOfAsteriod,false);
					asteroid->myPos=pos;
					asteroid->isAlive=true;
					asteroid->mySpeed=originAsteriodSpeed;
					allAsteroids.push_back(asteroid);
					sprintf(name,"LittleAsteriod%d",countLittleAsteriod++);
					pos.y-=30;
					asteroid = new Asteroid(_sceneManager, & pos,name ,tKindsOfAsteriod,false);
					asteroid->myPos=pos;
					asteroid->isAlive=true;
					asteroid->mySpeed=originAsteriodSpeed;
					allAsteroids.push_back(asteroid);
				}
				else
				{
					allAsteroids.erase(allAsteroids.begin()+j);
				}
			}
		}
	}
}

void MyProgram::asteriodBehavior(const float timeDelta)
{
	for(int i=0;i<allAsteroids.size();++i)
	{
		if(allAsteroids[i]->isAlive==true)
		{
			for(int j=0;j<allAsteroids.size();++j)
			{
				if(allAsteroids[i]->isAlive==true&&i!=j)
				{
					allAsteroids[i]->asteriodCollision(allAsteroids[j],timeDelta);
				}
			}
			if(!bReborn && allAsteroids[i]->isBig==false)
				allAsteroids[i]->moveToPlayer(myPlayer,timeDelta);
			else
				allAsteroids[i]->moveStraight(myPlayer,timeDelta,bReborn);
			outOfBount(allAsteroids[i]);
			allAsteroids[i]->mySceneNode->setPosition(allAsteroids[i]->myPos);
			if(!bReborn && myPlayer->isPlayerDied(allAsteroids[i]))
			{
				if(myPlayer->myHP>0)
				{
					bReborn = true;
					bMove = false;
					rebornPause = 0;
					myPlayer->mySceneNode->setVisible(false);
				}
			}
		}
	}
}

void MyProgram::ballBehavior(const float timeDelta)
{
	for(int i=0;i<allBall.size();++i)
	{
		if(allBall[i]->myPos.distance(myPlayer->myPos)<7)
		{
			if(myPlayer->myHP<MAXHP)
				myPlayer->myHP++;
			allBall[i]->mySceneNode->setVisible(false);
			allBall.erase(allBall.begin()+i);
		}
		else if(allBall[i]->getExist()==true)
		{
			allBall[i]->move(timeDelta);
			outOfBount(allBall[i]);
		}
	}
}

void MyProgram::outOfBount(myEntity *e)
{
	if(e->myPos.y<nYpos)
		e->myPos.y=-e->myPos.y-1;
	if(e->myPos.y>pYpos)
		e->myPos.y=-e->myPos.y+1;
	if(e->myPos.x<nXpos)
		e->myPos.x=-e->myPos.x-1;
	if(e->myPos.x>pXpos)
		e->myPos.x=-e->myPos.x+1;

}

void MyProgram::isGameOver()
{
	if(!allAsteroids.size())
	{
		gameOver=true;
		win = true;
	}
	if(myPlayer->myHP<=0)
	{
		gameOver=true;
		win = false;
	}
	if(gameOver==true)
	{
		for(int i=0;i<allBullet.size();++i)
		{
			allBullet[i]->mySceneNode->setVisible(false);
		}
		allExplosion->setVisible(false);
	}
	flagForGameLevel=true;
}

void MyProgram::hideAll()
{
	for(int i=0;i<allAsteroids.size();++i)
	{
		allAsteroids[i]->mySceneNode->setVisible(false);
	}
	myPlayer->mySceneNode->setVisible(false);
}

void MyProgram::showAll()
{
	for(int i=0;i<allAsteroids.size();++i)
	{
		allAsteroids[i]->mySceneNode->setVisible(true);
	}
	myPlayer->mySceneNode->setVisible(true);
}