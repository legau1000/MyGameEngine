#pragma once
#include <iostream>
#include <Ogre.h>
#include <OgreFrameListener.h>
#include <OIS.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include "Component.hpp"
#include "AnimationComponent.hpp"
using namespace std;
class myEventListener : public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
	myEventListener(Ogre::Camera*, Ogre::RenderWindow*,
		Ogre::SceneManager*, vector<shared_ptr<Component>>,
		const std::string&, Ogre::Plane* mPlane,
		btDiscreteDynamicsWorld* dynamicsWorld, std::shared_ptr<std::vector<btRigidBody*>> walls);
	~myEventListener();

	bool frameEnded(const Ogre::FrameEvent& evt);
	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	bool keyPressed(const OIS::KeyEvent& e) { return(true); };
	bool keyReleased(const OIS::KeyEvent& e) { return(true); };

	bool mouseMoved(const OIS::MouseEvent& me);
	bool mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id) { return(true); };
	bool mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id) { return(true); };

private:
	void setAnimation(const Ogre::FrameEvent& evt);
	void update();
	void makeMouve(const Ogre::FrameEvent& evt);

	bool _collision;

	Ogre::Camera* _cam;
	OIS::Keyboard* mKeyboard;
	OIS::Mouse* mMouse;

	std::string _name;

	int *_speed;

	OIS::InputManager* mInputSystem;

	static myEventListener* mInputManager;
	Ogre::SceneManager* _scnMgr;
	Ogre::Plane* _mPlane;

	vector<shared_ptr<Component>> _componentsForEvent;
	
	AnimationComponent* _animation;
	
	btDiscreteDynamicsWorld* _dynamicsWorld;

	std::shared_ptr<std::vector<btRigidBody*>> _walls;

	btRigidBody* _me;
};
