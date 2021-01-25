#include <OgreMath.h>
#include "myEventListener.hpp"
#include "MooveComponent.hpp"

myEventListener::myEventListener(Ogre::Camera* cam, Ogre::RenderWindow* renderWindow, Ogre::SceneManager* scnMgr, vector<shared_ptr<Component>> componentsForEvent, const std::string& name, Ogre::Plane* mPlane)
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

	mKeyboard = nullptr;
	mMouse = nullptr;
	MooveComponent* moove = nullptr;
	_animation = nullptr;

	while (index < componentsForEvent.size()) {
		switch (componentsForEvent[index]->getType())
		{
		case MyComponentEnum::Moove:
			std::cout << "Moove" << std::endl;
			mKeyboard = static_cast<OIS::Keyboard*>(mInputSystem->createInputObject(OIS::OISKeyboard, true));
			mKeyboard->setEventCallback(this);
			moove = static_cast<MooveComponent*>(componentsForEvent[index].get());
			_speed = &(moove->_speed);
		case MyComponentEnum::Turn:
			mMouse = static_cast<OIS::Mouse*>(mInputSystem->createInputObject(OIS::OISMouse, true));
			mMouse->setEventCallback(this);
			mMouse->getMouseState().height = 800;
			mMouse->getMouseState().width = 800;
		case MyComponentEnum::Animation:
			_animation = static_cast<AnimationComponent*>(componentsForEvent[index].get());
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
	std::cout << "------" << std::endl;
	std::cout << _animation->getAnimation() << std::endl;
	std::cout << "------" << std::endl;
	Ogre::AnimationState* mAnimationState = _scnMgr->getEntity(_name)->getAnimationState(_animation->getAnimation());
	mAnimationState->setLoop(true);
	mAnimationState->setEnabled(true);
	mAnimationState->addTime(evt.timeSinceLastFrame);
}

bool myEventListener::frameEnded(const Ogre::FrameEvent& evt)
{
	bool eventDone = false;
	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;
	Ogre::Vector3 deplacement = Ogre::Vector3::ZERO;
	float mMouvement = *_speed * evt.timeSinceLastFrame;
	if (mKeyboard->isKeyDown(OIS::KC_LEFT) || mKeyboard->isKeyDown(OIS::KC_A)) {
		deplacement.x -= mMouvement;
		_scnMgr->getSceneNode(_name)->translate(-mMouvement, 0.0, 0.0);
	}
	if (mKeyboard->isKeyDown(OIS::KC_RIGHT) || mKeyboard->isKeyDown(OIS::KC_D)) {
		deplacement.x += mMouvement;
		_scnMgr->getSceneNode(_name)->translate(mMouvement, 0.0, 0.0);
	}
	if (mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_W)) {
		deplacement.z += mMouvement;
		_scnMgr->getSceneNode(_name)->translate(0.0, 0.0, mMouvement);
	}
	if (mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_S)) {
		deplacement.z -= mMouvement;
		_scnMgr->getSceneNode(_name)->translate(0.0, 0.0, -mMouvement);
	}
	if (_cam != nullptr && deplacement != Ogre::Vector3::ZERO) {
		Ogre::Vector3 playerPost = _scnMgr->getSceneNode(_name)->getPosition();
		_scnMgr->getSceneNode("Cam")->setPosition(Ogre::Vector3(playerPost.x, 500, playerPost.z));
		_scnMgr->getSceneNode("Cam")->lookAt(Ogre::Vector3(playerPost.x, 0, playerPost.z), Ogre::Node::TS_PARENT);
		eventDone = true;
	}
	if (eventDone && _animation) {
		_animation->setAnimation("mouve");
	} else if (_animation) {
		_animation->setAnimation("idle");

	}
	return (true);
}

bool myEventListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
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
		std::cout << "Point: " << point << std::endl;
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
