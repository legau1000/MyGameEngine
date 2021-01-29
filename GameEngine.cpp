#include <Terrain/OgreTerrain.h>
#include "GameEngine.hpp"
#include "SpriteComponent.hpp"
#include "PositionComponent.hpp"
#include "MooveComponent.hpp"
#include "AnimationComponent.hpp"
#include "WallComponent.hpp"
#include "ColliderComponent.hpp"
#include "ScaleComponent.hpp"
#include "IAComponent.hpp"
#include "OrientationComponent.hpp"

GameEngine::GameEngine(const std::string &name)
	: OgreBites::ApplicationContext(name)
{
	_components = array<vector<pair<string, shared_ptr<Component>>>, MyComponentEnum::Last>();
	_root = nullptr;
	_scnMgr = nullptr;
	_mLight = nullptr;
	_cam = nullptr;
	_window = nullptr;
	dynamicsWorld = nullptr;
	_soundManager = std::make_shared<SoundManager>();
}

GameEngine::~GameEngine()
{}

void GameEngine::setup()
{
	this->setConfigFile();
	this->setApplicationSetup();
	this->setOgreGlobalVariables();
	this->initObjects();
	this->initGround(); // Need to be change
	this->initWall(); // Need to be change
}

void GameEngine::initObjects() {
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void GameEngine::initWall()
{
	_mWall = new Ogre::Plane(Ogre::Vector3::UNIT_Z, 0);
	Ogre::MeshManager::getSingleton().createPlane("wall",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		*_mWall,
		200, 200,
		20, 20,
		true,
		1,
		1500, 1500,
		Ogre::Vector3::UNIT_Y
	);
}

void GameEngine::initGround()
{
	_mPlane = new Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		*_mPlane,
		1500, 1500,
		20, 20,
		true,
		1,
		1500, 1500,
		Ogre::Vector3::UNIT_Z
	);
	Ogre::Entity* ent = _scnMgr->createEntity("plan", "ground");
	Ogre::SceneNode* node = _scnMgr->getRootSceneNode()->createChildSceneNode("sol");
	node->attachObject(ent);
	ent->setMaterialName("Examples/GrassFloor");
	/**/
	// Physics
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -50, 0));

	btScalar groundMass(0.); //the mass is 0, because the ground is immovable (static)
	btVector3 localGroundInertia(0, 0, 0);

	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(1500.), btScalar(0.), btScalar(1500.)));
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);

	groundShape->calculateLocalInertia(groundMass, localGroundInertia);

	btRigidBody::btRigidBodyConstructionInfo groundRBInfo(groundMass, groundMotionState, groundShape, localGroundInertia);
	btRigidBody* groundBody = new btRigidBody(groundRBInfo);

	groundBody->setUserPointer((void*)node);

	//add the body to the dynamics world
	collisionShapes.push_back(groundShape);
	dynamicsWorld->addRigidBody(groundBody);
	
}

void GameEngine::startRendering()
{
	getRoot()->startRendering();
}

void GameEngine::setOgreGlobalVariables()
{
	_root = getRoot();
	_scnMgr = _root->createSceneManager();
	_window = getRenderWindow();
	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(_scnMgr);
	_scnMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
}

void GameEngine::setApplicationSetup()
{
	OgreBites::ApplicationContext::setup();
	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
	Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(8);
}

void GameEngine::setConfigFile()
{
	Ogre::ConfigFile configFile;
	configFile.load("resources.cfg");
}

// haven't time to make it better.
void GameEngine::createEntity(std::shared_ptr<Entity> newEntity)
{
	_entity.insert(std::make_pair(newEntity->getName(), newEntity));
	/*
	if (newEntity->getType(MyComponentEnum::Wall) == true) {
		this->makeWall(newEntity);
		if (newEntity->getType(MyComponentEnum::Collider) == true) {
			this->makeCollider(newEntity, 0.1f);
		}
		return;
	}*/
	if (newEntity->getType(MyComponentEnum::Position) == true) {
		this->makePosition(newEntity);
	}
	if (newEntity->getType(MyComponentEnum::Collider) == true) {
		this->makeCollider(newEntity, 0.5f);
	}
	if (newEntity->getType(MyComponentEnum::IA) == true) { // make IA
		//this->makeIA(newEntity);
	} else if (newEntity->getType(MyComponentEnum::Event) == true) {
		this->makeEvents(newEntity);
	} else if (newEntity->getType(MyComponentEnum::Moove) == true) {
		//this->makeFire(newEntity);
	}
}

void GameEngine::createPlayer(std::shared_ptr<Entity> entity)
{
	// also need to tell where we are
	Ogre::SceneNode* camNode = _scnMgr->getRootSceneNode()->createChildSceneNode("Cam");
	camNode->setPosition(20, 500, 0);
	camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

	// create the camera
	_cam = _scnMgr->createCamera("myCam");
	_cam->setNearClipDistance(5); // specific to this sample
	_cam->setAutoAspectRatio(true);
	_cam->setNearClipDistance(1);
	_cam->setFarClipDistance(1500);
	camNode->attachObject(_cam);
	getRenderWindow()->addViewport(_cam);

	this->createEntity(entity);
}

/*
	Dans l'entity que je reçoit, j'aurais déjà de set le array avec toute les valeurs true/false
	A chaque fois que j'ai un true, MyGam call le game engin pour créer le Component
	On stock tout les components ici.	
*/

// [Start Engine Components]

void GameEngine::createComponentSprite(const std::string name, const std::string mesh, const std::string material)
{
	_components[MyComponentEnum::Sprite].push_back(std::make_pair(name, std::make_shared<SpriteComponent>(name, mesh, material)));
}

void GameEngine::createComponentPosition(const std::string name, float x, float y, float z)
{
	_components[MyComponentEnum::Position].push_back(std::make_pair(name, std::make_shared<PositionComponent>(name, x, y, z)));
}

void GameEngine::createComponentMoove(const std::string name, int speed)
{
	_components[MyComponentEnum::Moove].push_back(std::make_pair(name, std::make_shared<MooveComponent>(name, speed)));
}

void GameEngine::createComponentAnimation(const std::string name, std::string attack, std::string idle, std::string mouve)
{
	_components[MyComponentEnum::Animation].push_back(std::make_pair(name, std::make_shared<AnimationComponent>(name, attack, idle, mouve)));
}

void GameEngine::createComponentWall(const std::string name, float orientation)
{
	_components[MyComponentEnum::Wall].push_back(std::make_pair(name, std::make_shared<WallComponent>(name, orientation)));
}

void GameEngine::createComponentCollider(const std::string name)
{
	_components[MyComponentEnum::Collider].push_back(std::make_pair(name, std::make_shared<ColliderComponent>(name)));
}

void GameEngine::createComponentScale(const std::string name, float x, float y, float z)
{
	_components[MyComponentEnum::Scale].push_back(std::make_pair(name, std::make_shared<ScaleComponent>(name, x, y, z)));
}

void GameEngine::createComponentIA(const std::string name, std::vector<int> startPos, std::vector<int> endPos)
{
	_components[MyComponentEnum::IA].push_back(std::make_pair(name, std::make_shared<IAComponent>(name, startPos, endPos)));
}

void GameEngine::createComponentOrientation(const std::string name, float degree)
{
	std::cout << "Orientation: " << name << std::endl;
	_components[MyComponentEnum::Orientation].push_back(std::make_pair(name, std::make_shared<OrientationComponent>(name, degree)));
}

// [End Engine Components]

shared_ptr<Component> GameEngine::getComponent(MyComponentEnum value, const std::string& name)
{
	int index = 0;
	vector<pair<string, shared_ptr<Component>>> tab = _components[value];

	while (index < tab.size()) {
		if (tab[index].first == name) {
			return (tab[index].second);
		}
		index++;
	}
	return (nullptr);
}

// [Start Components generators]

void GameEngine::makePosition(std::shared_ptr<Entity> entity)
{
	if (entity->getType(MyComponentEnum::Sprite) == true) {
		shared_ptr<Component> componentSprite = this->getComponent(MyComponentEnum::Sprite, entity->getName());
		if (componentSprite == nullptr)
			return;
		SpriteComponent* sprite = static_cast<SpriteComponent*>(componentSprite.get());
		Ogre::Entity* ent = _scnMgr->createEntity(entity->getName(), sprite->getMesh());
		Ogre::SceneNode* node = _scnMgr->getRootSceneNode()->createChildSceneNode(entity->getName());

		if (sprite->getMaterial() != "")
			ent->setMaterialName(sprite->getMaterial());

		node->_updateBounds();
		//node->showBoundingBox(true);

		node->attachObject(ent);

		shared_ptr<Component> componentScale = this->getComponent(MyComponentEnum::Scale, entity->getName());
		if (componentScale == nullptr)
			return;
		ScaleComponent* scale = static_cast<ScaleComponent*>(componentScale.get());
		node->setScale(scale->x, scale->y, scale->z);

		shared_ptr<Component> componentDegree = this->getComponent(MyComponentEnum::Orientation, entity->getName());
		if (componentDegree != nullptr) {
			OrientationComponent* degree = static_cast<OrientationComponent*>(componentDegree.get());
			std::cout << entity->getName() << "degre: " << degree->_degree << std::endl;
			node->yaw(Ogre::Degree(degree->_degree));
		}

		shared_ptr<Component> componentPosition = this->getComponent(MyComponentEnum::Position, entity->getName());
		if (componentPosition == nullptr)
			return;
		PositionComponent* position = static_cast<PositionComponent*>(componentPosition.get());
		node->setPosition(position->getX(), position->getY(), position->getZ());
		node->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_LOCAL);
		ent->setCastShadows(true);
	}
}
/*
void GameEngine::makeWall(std::shared_ptr<Entity> entity)
{
	if (entity->getType(MyComponentEnum::Sprite) == true) {
		shared_ptr<Component> componentSprite = this->getComponent(MyComponentEnum::Sprite, entity->getName());
		if (componentSprite == nullptr)
			return;
		SpriteComponent* sprite = static_cast<SpriteComponent*>(componentSprite.get());
		Ogre::Entity* ent = _scnMgr->createEntity(entity->getName(), sprite->getMesh());
		//ent->setMaterialName("Examples/RustySteel");//Examples/GrassFloor
		Ogre::SceneNode* node = _scnMgr->getRootSceneNode()->createChildSceneNode(entity->getName());
		node->attachObject(ent);
		node->setScale(1, 1, 0.2);

		node->_updateBounds();
		node->showBoundingBox(true);

		shared_ptr<Component> componentPosition = this->getComponent(MyComponentEnum::Position, entity->getName());
		if (componentPosition == nullptr)
			return;
		PositionComponent* position = static_cast<PositionComponent*>(componentPosition.get());
		node->setPosition(position->getX(), position->getY(), position->getZ());
		shared_ptr<Component> componentWall = this->getComponent(MyComponentEnum::Wall, entity->getName());
		if (componentWall == nullptr)
			return;
		WallComponent* wall = static_cast<WallComponent*>(componentWall.get());
		node->yaw(Ogre::Degree(wall->orientation));
		ent->setCastShadows(true);
	}
}
*/
void GameEngine::makeEvents(std::shared_ptr<Entity> entity)
{
	shared_ptr<Component> componentSprite = nullptr;
	vector<shared_ptr<Component>> componentForEvent = vector<shared_ptr<Component>>();

	if (entity->getType(MyComponentEnum::Moove) == true) {
		componentSprite = this->getComponent(MyComponentEnum::Moove, entity->getName());
		if (componentSprite != nullptr) {
			componentForEvent.push_back(componentSprite);
		}
		componentSprite = nullptr;
	}
	if (entity->getType(MyComponentEnum::Turn) == true) {
		componentSprite = nullptr;
	}
	if (entity->getType(MyComponentEnum::Shoot) == true) {
		componentSprite = nullptr;
	}
	if (entity->getType(MyComponentEnum::Animation) == true) {
		componentSprite = this->getComponent(MyComponentEnum::Animation, entity->getName());
		if (componentSprite != nullptr) {
			componentForEvent.push_back(componentSprite);
		}
		componentSprite = nullptr;
	}
	if (entity->getType(MyComponentEnum::Collider) == true) {
		componentSprite = this->getComponent(MyComponentEnum::Collider, entity->getName());
		if (componentSprite != nullptr) {
			componentForEvent.push_back(componentSprite);
		}
		componentSprite = nullptr;
	}
	myListener = make_shared<myEventListener>(_cam, _window, _scnMgr, componentForEvent, entity->getName(), _mPlane, dynamicsWorld, _soundManager);
	getRoot()->addFrameListener(myListener.get());
}

void GameEngine::makeCollider(std::shared_ptr<Entity> entity, float Entitymass)
{
	// std::cout << "COLLIDER FOR: " << entity->getName() << std::endl;
	Ogre::SceneNode* node = _scnMgr->getSceneNode(entity->getName());
	node->_updateBounds();
	const Ogre::AxisAlignedBox& b = node->_getWorldAABB();

	Ogre::Vector3 meshBoundingBox(b.getSize());

	if (meshBoundingBox == Ogre::Vector3::ZERO) {
		std::cout << "bounding = 0" << std::endl;
	}

	btCollisionShape* colShape = new btBoxShape(btVector3(meshBoundingBox.x/2.0f, meshBoundingBox.y/2.0f, meshBoundingBox.z/2.0f));
	std::cout << (float)meshBoundingBox.x << " : " << (float)meshBoundingBox.y << " : " << (float)meshBoundingBox.z << std::endl;
	
	//collisionShapes.push_back(colShape);

	//set the initial position and transform. For this demo, we set the tranform to be none
	btTransform startTransform;
	startTransform.setIdentity();
	

	Ogre::Quaternion quat2 = _scnMgr->getSceneNode(entity->getName())->_getDerivedOrientation();
	startTransform.setRotation(btQuaternion(quat2.x, quat2.y, quat2.z, quat2.w));

	Ogre::Vector3 pos = _scnMgr->getSceneNode(entity->getName())->_getDerivedPosition();
	startTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));


	//Ogre::Vector3 origin = _scnMgr->getSceneNode(entity->getName())->getPosition();
	//startTransform.setOrigin(btVector3(origin.x, origin.y, origin.z));

	btScalar mass = Entitymass;
	//set the mass of the object. a mass of "0" means that it is an immovable object
	btVector3 localInertia(0, 0, 0);
	if (entity->getName().find("Wall") != std::string::npos) // maybe make  mass class
		mass = 0.0f;
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	//actually contruvc the body and add it to the dynamics world
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setDamping(0.2f, 0.8f);

	body->setUserPointer(node);

	//body->setRestitution(1);
	shared_ptr<Component> componentCollider = this->getComponent(MyComponentEnum::Collider, entity->getName());
	if (componentCollider == nullptr)
		return;
	ColliderComponent* collider = static_cast<ColliderComponent*>(componentCollider.get());
	collider->body = body;
	/*
	if (Entitymass == 0.0f) {
		std::cout << "add wall" << std::endl;
		this->_walls->push_back(body);
	} */
	//physicsEngine->trackRigidBodyWithName(body, physicsCubeName);
	collisionShapes.push_back(colShape);
	dynamicsWorld->addRigidBody(body);
	// std::cout << "END COLLIDER FOR: " << entity->getName() << std::endl;
}

// [End Components generators]
