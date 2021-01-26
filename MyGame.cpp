#include "MyGame.hpp"

MyGame::MyGame(GameEngine* gameEngine)
	: _gameEngine(gameEngine)
{
}

MyGame::~MyGame()
{}

/*
	Next step, create all components and set it.
*/

void MyGame::createPlayer(std::string name)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>(name);
	entity->setNewType(MyComponentEnum::Position);
	entity->setNewType(MyComponentEnum::Sprite);

	entity->setNewType(MyComponentEnum::Collider);
	entity->setNewType(MyComponentEnum::Scale);

	entity->setNewType(MyComponentEnum::Event);
	entity->setNewType(MyComponentEnum::Moove);
	entity->setNewType(MyComponentEnum::Turn);
	
	// entity->setNewType(MyComponentEnum::Shoot); // ->Todo

	// entity->setNewType(MyComponentEnum::Sound); -> Todo
	entity->setNewType(MyComponentEnum::Animation);

	this->_gameEngine->createComponentPosition(name, 0, 10, 0);
	this->_gameEngine->createComponentSprite(name, "robot.mesh");

	this->_gameEngine->createComponentCollider(name);
	this->_gameEngine->createComponentMoove(name, 100);
	
	this->_gameEngine->createComponentAnimation(name, "Shoot", "Idle", "Walk");

	this->_gameEngine->createComponentScale(name, 1, 1, 1);

	this->_gameEngine->createPlayer(entity);
	_entity.insert(std::make_pair(name, entity));
}

void MyGame::createWall(std::string name, std::vector<int> pos, float orientation)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>(name);
	entity->setNewType(MyComponentEnum::Position);
	entity->setNewType(MyComponentEnum::Sprite);

	entity->setNewType(MyComponentEnum::Collider);

	entity->setNewType(MyComponentEnum::Scale);

	//entity->setNewType(MyComponentEnum::Orientation);

	//entity->setNewType(MyComponentEnum::Wall);

	this->_gameEngine->createComponentPosition(name, pos[0], pos[1], pos[2]);
	this->_gameEngine->createComponentSprite(name, "cube.mesh");
	//this->_gameEngine->createComponentWall(name, orientation);
	this->_gameEngine->createComponentCollider(name);

	this->_gameEngine->createComponentScale(name, 1, 1, 1);
	//this->_gameEngine->createComponentOrientation(name);

	this->_gameEngine->createEntity(entity);
	_entity.insert(std::make_pair(name, entity));
}

void MyGame::createBloc(std::string name, std::vector<int> pos)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>(name);
	entity->setNewType(MyComponentEnum::Position);
	entity->setNewType(MyComponentEnum::Sprite);

	entity->setNewType(MyComponentEnum::Collider);

	entity->setNewType(MyComponentEnum::Scale);

	this->_gameEngine->createComponentPosition(name, pos[0], pos[1], pos[2]);
	this->_gameEngine->createComponentSprite(name, "cube.mesh");

	this->_gameEngine->createComponentCollider(name);

	this->_gameEngine->createComponentScale(name, 1, 1, 1);

	this->_gameEngine->createEntity(entity);
	_entity.insert(std::make_pair(name, entity));
}

void MyGame::start()
{
	this->createPlayer("Player");
	//this->createWall("Wall-1", { -50, 0,-120 }, 0);
	//this->createWall("Wall-2", { 50,50,-120 }, 0);
	//this->createWall("Wall-3", { -50,0,-50 }, 90);
	this->createBloc("Cube", { -50, 0,-120 });
	this->createBloc("Cube2", { 100, 0, 100 });
	_gameEngine->startRendering();
}
