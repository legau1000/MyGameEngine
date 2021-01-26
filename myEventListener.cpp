#include <OgreMath.h>
#include "myEventListener.hpp"
#include "MooveComponent.hpp"
#include "ColliderComponent.hpp"

myEventListener::myEventListener(Ogre::Camera* cam, Ogre::RenderWindow* renderWindow, 
	Ogre::SceneManager* scnMgr, vector<shared_ptr<Component>> componentsForEvent, 
	const std::string& name, Ogre::Plane* mPlane, 
	btDiscreteDynamicsWorld* dynamicsWorld, std::shared_ptr<std::vector<btRigidBody*>> walls)
{
	_mPlane = mPlane;
	_name = name;
	if (componentsForEvent.size() == 0) {
		return;
	}
	OIS::ParamList paramList;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	renderWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << (unsigned int)windowHnd;
	paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
	paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	mInputSystem = OIS::InputManager::createInputSystem(paramList);

	int index = 0;
	shared_ptr<Component> component;

	_dynamicsWorld = nullptr;
	mKeyboard = nullptr;
	mMouse = nullptr;
	MooveComponent* moove = nullptr;
	_animation = nullptr;
	ColliderComponent* collider = nullptr;
	_walls = walls;

	if (dynamicsWorld != nullptr)
		_dynamicsWorld = dynamicsWorld;
	while (index < componentsForEvent.size()) {
		std::cout << "size = " << componentsForEvent.size() << " ; index = " << index << std::endl;
		switch (componentsForEvent[index]->getType())
		{
		case MyComponentEnum::Collider:
			collider = static_cast<ColliderComponent*>(componentsForEvent[index].get());
			_me = (btRigidBody*)collider->body;
			std::cout << "Fin Collider On Event" << std::endl;
			break;
		case MyComponentEnum::Moove:
			mKeyboard = static_cast<OIS::Keyboard*>(mInputSystem->createInputObject(OIS::OISKeyboard, true));
			mKeyboard->setEventCallback(this);
			moove = static_cast<MooveComponent*>(componentsForEvent[index].get());
			_speed = &(moove->_speed);
			std::cout << "Fin Moove On Event" << std::endl;
			//break;
		//case MyComponentEnum::Turn:
			mMouse = static_cast<OIS::Mouse*>(mInputSystem->createInputObject(OIS::OISMouse, true));
			mMouse->setEventCallback(this);
			mMouse->getMouseState().height = 800;
			mMouse->getMouseState().width = 800;
			std::cout << "Fin Turn On Event" << std::endl;
			break;
		case MyComponentEnum::Animation:
			_animation = static_cast<AnimationComponent*>(componentsForEvent[index].get());
			std::cout << "Fin Animation On Event" << std::endl;
			break;
		default:
			break;
		}
		index++;
	}
	_cam = cam;
	//_mVitesseRotation = 0.2;
	_scnMgr = scnMgr;
}

myEventListener::~myEventListener()
{}

void myEventListener::setAnimation(const Ogre::FrameEvent& evt)
{
	//std::cout << "------" << std::endl;
	//std::cout << _animation->getAnimation() << std::endl;
	//std::cout << "------" << std::endl;
	Ogre::AnimationState* mAnimationState = _scnMgr->getEntity(_name)->getAnimationState(_animation->getAnimation());
	mAnimationState->setLoop(true);
	mAnimationState->setEnabled(true);
	mAnimationState->addTime(evt.timeSinceLastFrame);
}

void myEventListener::makeMouve(const Ogre::FrameEvent& evt)
{
	bool eventDone = false;

	Ogre::Vector3 deplacement = Ogre::Vector3::ZERO;
	float mMouvement = *_speed * evt.timeSinceLastFrame;
	if (mKeyboard->isKeyDown(OIS::KC_LEFT) || mKeyboard->isKeyDown(OIS::KC_A)) {
		deplacement.x = -50.0f;
		//_scnMgr->getSceneNode(_name)->translate(-mMouvement, 0.0, 0.0);
	}
	if (mKeyboard->isKeyDown(OIS::KC_RIGHT) || mKeyboard->isKeyDown(OIS::KC_D)) {
		deplacement.x = 50.0f;
		//_scnMgr->getSceneNode(_name)->translate(mMouvement, 0.0, 0.0);
	}
	if (mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_W)) {
		//deplacement.z -= mMouvement;
		deplacement.z = -50.0f;
		//_scnMgr->getSceneNode(_name)->translate(0.0, 0.0, -mMouvement);
	}
	if (mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_S)) {
		//deplacement.z += mMouvement;
		deplacement.z = 50.0f;
		//_scnMgr->getSceneNode(_name)->translate(0.0, 0.0, mMouvement);
	}

	btVector3 mouve((float)deplacement.x, 0.0f, (float)deplacement.z);
	btVector3 push;

	btTransform trans;
	_me->getMotionState()->getWorldTransform(trans);
	btQuaternion orientation = trans.getRotation();
	push = quatRotate(orientation, mouve);
	push.setY(0.0f);
	_me->activate();
	_me->applyCentralForce(mouve);

	if (deplacement != Ogre::Vector3::ZERO)
	{
		eventDone = true;
	}
	if (eventDone && _animation) {
		_animation->setAnimation("mouve");
	}
	else if (_animation) {
		_animation->setAnimation("idle");
	}
}

bool myEventListener::frameStarted(const Ogre::FrameEvent& evt)
{
	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;
	if (_dynamicsWorld != nullptr) {

		this->makeMouve(evt);
		
		_dynamicsWorld->stepSimulation((float)evt.timeSinceLastFrame, 10); //suppose you have 60 frames per second
		
		int numManifolds = _dynamicsWorld->getDispatcher()->getNumManifolds();
		//std::cout << numManifolds << std::endl;
		if (numManifolds > 0) {
			//std::cout << "*** Colision infos ***" << std::endl;
			for (int i = 0; i < numManifolds; i++) {
				btPersistentManifold* contactManifFold = _dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
				const btCollisionObject* obA = (btCollisionObject*)contactManifFold->getBody0();
				const btCollisionObject* obB = (btCollisionObject*)contactManifFold->getBody1();
				int numContacts = contactManifFold->getNumContacts();

				if (numContacts > 0) {
					for (int k = 0; k < numContacts; k++) {
						btManifoldPoint& pt = contactManifFold->getContactPoint(k);
						if (pt.getDistance() < 0.f) {
							const btVector3& ptA = pt.getPositionWorldOnA();
							const btVector3& ptB = pt.getPositionWorldOnB();
							const btVector3& normalOnB = pt.m_normalWorldOnB;
							Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(obA->getUserPointer());
							//std::cout << node->getName() << " Position impact: " << ptA.getX() << " : " << ptA.getY() << " : " << ptA.getZ() << std::endl;
							Ogre::SceneNode* node2 = static_cast<Ogre::SceneNode*>(obB->getUserPointer());
							//std::cout << node2->getName() << " Position impact: " << ptB.getX() << " : " << ptB.getY() << " : " << ptB.getZ() << std::endl;
							
							if (node->getName().compare("sol") != 0 && node2->getName().compare("sol") != 0) {
								std::cout << "------------------------------------------------------------------" << std::endl;
								std::cout << "collision entre: " << node->getName() << " && " << node2->getName() << std::endl;
								std::cout << "------------------------------------------------------------------" << std::endl;
							}
							
							//std::cout << "Contact point: " << k << std::endl;
							//std::cout << "With force: " << pt.getAppliedImpulse() << std::endl;
						}
					}
				}

			}
		}/*
		_dynamicsWorld->stepSimulation(1.0f / 60.0f); //suppose you have 60 frames per second
		for (int i = 0; i < _dynamicsWorld->getNumCollisionObjects(); i++) {
			btCollisionObject* obj = _dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);

			if (body && body->getMotionState()) {
				btTransform trans;
				body->getMotionState()->getWorldTransform(trans);
				void* userPointer = body->getUserPointer();
				
				if (userPointer == _scnMgr->getSceneNode(_name)) {
					std::cout << "Collision Player" << std::endl;
					//btQuaternion orientation = trans.getRotation();
					//Ogre::SceneNode* sceneNode = static_cast<Ogre::SceneNode*>(userPointer);
					//sceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
					//sceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
				}
			}
		}*/
		/*std::cout << _walls->size() << std::endl;
		for (int i = 0; i < _walls->size(); i++) {
			btRigidBody* obj = _walls->at(i);
			std::cout << _me->checkCollideWith(obj) << std::endl;
			if (_me->checkCollideWith(obj))
				std::cout << " find collision" << std::endl;

		}*/
		this->update();
	}
	return true;
}

void myEventListener::update()
{
	btTransform trans;
	if (_me && _me->getMotionState()) {
		btVector3 mouve(0.0f, 0.0f, 0.0f);
		btVector3 push;

		_me->getMotionState()->getWorldTransform(trans);
		btQuaternion orientation = trans.getRotation();
		push = quatRotate(orientation, mouve);
		push.setY(0.0f);
		_me->activate();
		_me->applyCentralForce(push);

		_me->getMotionState()->getWorldTransform(trans);
		orientation = trans.getRotation();
		_scnMgr->getSceneNode(_name)->setPosition(Ogre::Vector3(trans.getOrigin().getX(), 0.0f, trans.getOrigin().getZ()));

		Ogre::Vector3 playerPost = _scnMgr->getSceneNode(_name)->getPosition();
		_scnMgr->getSceneNode("Cam")->setPosition(Ogre::Vector3(playerPost.x + 20, 500, playerPost.z));
		_scnMgr->getSceneNode("Cam")->lookAt(Ogre::Vector3(playerPost.x, 0, playerPost.z), Ogre::Node::TS_PARENT);
	}
}

bool myEventListener::frameEnded(const Ogre::FrameEvent& evt)
{
	/*bool eventDone = false;
	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;
	Ogre::Vector3 deplacement = Ogre::Vector3::ZERO;
	float mMouvement = *_speed * evt.timeSinceLastFrame;
	if (mKeyboard->isKeyDown(OIS::KC_LEFT) || mKeyboard->isKeyDown(OIS::KC_A)) {
		deplacement.x -= mMouvement;
		//_scnMgr->getSceneNode(_name)->translate(-mMouvement, 0.0, 0.0);
	}
	if (mKeyboard->isKeyDown(OIS::KC_RIGHT) || mKeyboard->isKeyDown(OIS::KC_D)) {
		deplacement.x += mMouvement;
		//_scnMgr->getSceneNode(_name)->translate(mMouvement, 0.0, 0.0);
	}
	if (mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_W)) {
		deplacement.z -= mMouvement;
		//_scnMgr->getSceneNode(_name)->translate(0.0, 0.0, -mMouvement);
	}
	if (mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_S)) {
		deplacement.z += mMouvement;
		//_scnMgr->getSceneNode(_name)->translate(0.0, 0.0, mMouvement);
	}
	if (deplacement != Ogre::Vector3::ZERO) {
		btVector3 mouve((float)deplacement.x, 0.0f, (float)deplacement.z);
		btVector3 push;

		btTransform trans;
		_me->getMotionState()->getWorldTransform(trans);
		btQuaternion orientation = trans.getRotation();
		push = quatRotate(orientation, mouve);
		_me->activate();
		_me->applyCentralForce(push);

		if (_cam != nullptr) {
			Ogre::Vector3 playerPost = _scnMgr->getSceneNode(_name)->getPosition();
			_scnMgr->getSceneNode("Cam")->setPosition(Ogre::Vector3(playerPost.x + 20, 500, playerPost.z));
			_scnMgr->getSceneNode("Cam")->lookAt(Ogre::Vector3(playerPost.x, 0, playerPost.z), Ogre::Node::TS_PARENT);
		}
		eventDone = true;
	}
	if (eventDone && _animation) {
		_animation->setAnimation("mouve");
	} else if (_animation) {
		_animation->setAnimation("idle");
	}*/
	return (true);
}

bool myEventListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	this->update();
	if (_animation)
		this->setAnimation(evt);
	if (mMouse)
		mMouse->capture();
	if (mKeyboard)
		mKeyboard->capture();
	return (true);
}

bool myEventListener::mouseMoved(const OIS::MouseEvent& me)
{
	//std::cout << "-------------------------------------" << std::endl;
	//std::cout << "-------------------------------------" << std::endl;

	Ogre::Viewport* vp = _scnMgr->getCurrentViewport();
	Ogre::Real x = me.state.X.abs / float(me.state.width);
	Ogre::Real y = me.state.Y.abs / float(me.state.height);

	//std::cout << x << " : " << float(me.state.width) << std::endl;
	//std::cout << y << " : " << float(me.state.width) << std::endl;

	Ogre::Ray mouseRay = _cam->getCameraToViewportRay(x, y);

	std::pair<bool, Ogre::Real> result = mouseRay.intersects(*_mPlane);

	//std::cout << mouseRay.getOrigin() << std::endl;
	//std::cout << x << std::endl;
	//std::cout << y << std::endl;

	//_scnMgr->getSceneNode(_name)->rotate(Ogre::Vector3(mMouse->getMouseState().X.rel, 0, mMouse->getMouseState().Y.abs));
	if (result.first) {
		// if the ray intersect the plane, we have a distance value
		// telling us how far from the ray origin the intersection occurred.
		// the last thing we need is the point of the intersection.
		// Ray provides us getPoint() function which returns a point
		// along the ray, supplying it with a distance value.

		// get the point where the intersection is
		Ogre::Vector3 point = mouseRay.getPoint(result.second);
		point.y = 0;
		//std::cout << "Point: " << point << std::endl;
		// position our player to that point  
		//_scnMgr->getSceneNode(_name)->setPosition(point);
		_scnMgr->getSceneNode(_name)->lookAt(point, Ogre::Node::TS_WORLD);
		_scnMgr->getSceneNode(_name)->yaw(Ogre::Degree(90));
	}
	//camMouve.x = me.state.X.rel;
	//camMouve.z = me.state.Y.rel;
	//_scnMgr->getSceneNode(_name)->lookAt(Ogre::Vector3(camMouve.x, 0, camMouve.z), Ogre::Node::TS_PARENT);
	/*
	_posX += x;
	_posZ += y;
	
	_scnMgr->getSceneNode(_name)->lookAt(Ogre::Vector3(_posX, 0, _posZ), Ogre::Node::TS_WORLD);
	
	std::cout << _scnMgr->getSceneNode(_name)->getOrientation() << std::endl;
	*/
	//_scnMgr->getSceneNode(_name)->yaw(Ogre::Degree(-0.2 * me.state.X.rel)); //lookAt(Ogre::Vector3(camMouve.x, 0, camMouve.z), Ogre::Node::TS_LOCAL);
	//_scnMgr->getSceneNode(_name)->yaw(Ogre::Degree(-0.2 * me.state.Y.rel)); //lookAt(Ogre::Vector3(camMouve.x, 0, camMouve.z), Ogre::Node::TS_LOCAL);
	//_scnMgr->getSceneNode(_name)->yaw(Ogre::Degree(me.state.Y.rel));
	return (true);
}
