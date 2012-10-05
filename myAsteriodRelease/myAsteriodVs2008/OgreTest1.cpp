#include <ogre.h>
#include <iostream>
#include "ogretest1.h"

using namespace Ogre;
using namespace std;

void OgreTest1::setupResources(void)
{
    // Load resource paths from config file
    ConfigFile cf;
    cf.load("resources.cfg");

    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }	
}

void OgreTest1::ogreSetup()
{
	_root = new Root();

	// Load the resources.cfg file
	setupResources();
	
	// Show the dialog that allows the user to configure Ogre
	_root->showConfigDialog();

	// Initialise the rendering _window
	_root->initialise(true, "DIT HIT OOP Class");

	// Dont know what this does, but it doesnt matter!!
	// You need it anyway
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	_window = _root->getAutoCreatedWindow();

	// Create the scene manager
	_sceneManager = _root->createSceneManager(ST_GENERIC, "DIT HIT OOP Scene Manager");

	// Create the camera
	_camera = _sceneManager->createCamera("Player Cam");	
	_camera->setPosition(0, 0, 600);
	_camera->setNearClipDistance(1);
	_camera->lookAt(0, 0, 0);

	// Create the viewport
	Viewport * vp = _window->addViewport(_camera);
	vp->setBackgroundColour(ColourValue(0, 0, 0));

	// Create a light
	_sceneManager->setAmbientLight(ColourValue(0.5f, 0.5f, 0.5f));
	Light * l = _sceneManager->createLight("Main Light");	

	// Create a plane
	/*Plane plane;
	plane.normal = Vector3::UNIT_Y;
	plane.d = 100;
	MeshManager::getSingleton().createPlane("Myplane",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
		5000,5000,1,1,true,1,10,10,Vector3::UNIT_Z);
	Entity* pPlaneEnt = _sceneManager->createEntity( "plane", "Myplane" );
	pPlaneEnt->setMaterialName("Examples/Rockwall");
	pPlaneEnt->setCastShadows(false);
	_sceneManager->getRootSceneNode()->createChildSceneNode(Vector3(0,0,0))->attachObject(pPlaneEnt);
	*/
	OIS::ParamList pl;	
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	_window->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	_inputManager = OIS::InputManager::createInputSystem( pl );

	//Create all devices (We only catch joystick exceptions here, as, most people have Key/Mouse)
	_keyboard = static_cast<OIS::Keyboard*>(_inputManager->createInputObject( OIS::OISKeyboard, false));
	//_mouse = static_cast<OIS::Mouse*>(_inputManager->createInputObject( OIS::OISMouse, false ));

	_root->addFrameListener(this);
}

void OgreTest1::go()
{
	ogreSetup();
	mySetup();
	// Start rendering the scene!
	_root->startRendering();
}

void OgreTest1::processEvents()
{	
	float speed = 1;
	_keyboard->capture();
	//
	//if (_keyboard->isKeyDown(KC_LSHIFT))
	//{
	//	speed *= 5;
	//}
	//if (_keyboard->isKeyDown(KC_W))
	//{
	//	Vector3 v = Vector3(0, 0, -speed);
	//	_camera->moveRelative(v);
	//}
	//if (_keyboard->isKeyDown(KC_S))
	//{
	//	Vector3 v = Vector3(0, 0, speed);
	//	_camera->moveRelative(v);
	//}
	//if (_keyboard->isKeyDown(KC_A))
	//{
	//	Vector3 v = Vector3(-speed, 0, 0);
	//	_camera->moveRelative(v);
	//}
	//if (_keyboard->isKeyDown(KC_D))
	//{
	//	Vector3 v = Vector3(speed, 0, 0);
	//	_camera->moveRelative(v);
	//}
	if (_keyboard->isKeyDown(KC_ESCAPE))
	{
		exit(0);
	}

    
	
	//const OIS::MouseState &ms = _mouse->getMouseState();
	//long relX = ms.X;
	//long relY = ms.Y;
	//	
	//Degree mRotX = Degree(-relX * 0.13);
 //   Degree mRotY = Degree(-relY * 0.13);

	//_camera->yaw(mRotX);
 //   _camera->pitch(mRotY);
        
}

bool OgreTest1::processUnbufferedMouseInput(const FrameEvent& evt)
    {
		
		// Rotation factors, may not be used if the second mouse button is pressed
		// 2nd mouse button - slide, otherwise rotate
		/*const OIS::MouseState &ms = _mouse->getMouseState();
		Degree mRotX = Degree(-ms.X.rel * 0.13);
		Degree mRotY = Degree(-ms.Y.rel * 0.13); 
		
		_camera->yaw(mRotX);
		_camera->pitch(mRotY);*/

		return true;
	}

bool OgreTest1::frameStarted(const FrameEvent& evt)
{
	processEvents();
	update(evt.timeSinceLastFrame);
	return true;
}


bool OgreTest1::frameEnded(const FrameEvent& evt)
{
	//_mouse->capture();
	processUnbufferedMouseInput(evt);
	return true;
}

bool OgreTest1::keyPressed(const KeyEvent& e) 
{ 
	return false;
}
bool OgreTest1::keyReleased(const KeyEvent& e) 
{
	return false;
}


void OgreTest1::mySetup()
{
}

void OgreTest1::update(float timeDelta)
{
}
