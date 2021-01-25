#include "GameEngine.hpp"
#include "SpriteComponent.hpp"
#include "PositionComponent.hpp"
#include "MooveComponent.hpp"
#include "AnimationComponent.hpp"

GameEngine::GameEngine(const std::string &name)
	: OgreBites::ApplicationContext(name)
{
	_components = array<vector<pair<string, shared_ptr<Component>>>, MyComponentEnum::Last>();
	_root = nullptr;
	_scnMgr = nullptr;
	_mLight = nullptr;
	_cam = nullptr;
	_window = nullptr;
}

GameEngine::~GameEngine()
{}

void GameEngine::setup()
{
	this->setConfigFile();
	this->setApplicationSetup();
	this->setOgreGlobalVariables();
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
}

void GameEngine::startRendering()
{
	//std::cout << _root << std::endl;
	Ogre::Root *tmp = getRoot();
	tmp->startRendering();
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
	if (newEntity->getType(MyComponentEnum::Collider) == true) {
		//this->makeCollider(newEntity);
	}
	if (newEntity->getType(MyComponentEnum::Position) == true) {
		this->makePosition(newEntity);
	}
	if (newEntity->getType(MyComponentEnum::IA) == true) { // make IA
		//this->makeIA(newEntity);
	} else if (newEntity->getType(MyComponentEnum::Event) == true) {
		this->makeEvents(newEntity);
	} else if (newEntity->getType(MyComponentEnum::Moove) == true) {
		//this->makeFire(newEntity);
	}
	if (newEntity->getType(MyComponentEnum::Animation) == true) {
		//this->makeAnimation(newEntity);
	}
}

void GameEngine::createPlayer(std::shared_ptr<Entity> entity)
{
	// also need to tell where we are
	Ogre::SceneNode* camNode = _scnMgr->getRootSceneNode()->createChildSceneNode("Cam");
	camNode->setPosition(0, 500, 0);
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

void GameEngine::createComponentSprite(const std::string name, const std::string mesh)
{
	_components[MyComponentEnum::Sprite].push_back(std::make_pair(name, std::make_shared<SpriteComponent>(name, mesh)));
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
		node->attachObject(ent);
		shared_ptr<Component> componentPosition = this->getComponent(MyComponentEnum::Position, entity->getName());
		if (componentPosition == nullptr)
			return;
		PositionComponent* position = static_cast<PositionComponent*>(componentPosition.get());
		node->setPosition(position->getX(), position->getY(), position->getZ());
		node->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_LOCAL);
		ent->setCastShadows(true);
	}
}

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
	myListener = make_shared<myEventListener>(_cam, _window, _scnMgr, componentForEvent, entity->getName(), _mPlane);
	getRoot()->addFrameListener(myListener.get());
}

// [End Components generators]
