#ifndef __OgreTest1_H__
#define __OgreTest1_H__

#include <ogre.h>
#include <OIS/OISMouse.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISJoyStick.h>
#include <OIS/OISInputManager.h>

#include <iostream>

using namespace Ogre;
using namespace OIS;
using namespace std;

class OgreTest1: public FrameListener, public KeyListener
{
protected:
	Root * _root;
	RenderWindow * _window;
	SceneManager * _sceneManager;
	Camera * _camera;
	void setupResources();

public:
	void ogreSetup();
	virtual void go();
	virtual void mySetup();
	virtual bool frameStarted(const FrameEvent& evt);
	virtual void update(float timeDelta);
	virtual void processEvents();
	virtual bool frameEnded(const FrameEvent& evt);

	bool keyPressed( const KeyEvent &arg );
	bool keyReleased( const KeyEvent &arg );

	bool processUnbufferedMouseInput(const FrameEvent& evt);

	OIS::InputManager * _inputManager;
	OIS::Keyboard * _keyboard;
	OIS::Mouse * _mouse;

};

#endif