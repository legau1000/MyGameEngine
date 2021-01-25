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

	entity->setNewType(MyComponentEnum::Event);
	entity->setNewType(MyComponentEnum::Moove);
	entity->setNewType(MyComponentEnum::Turn);
	entity->setNewType(MyComponentEnum::Shoot); // ->Todo

	// entity->setNewType(MyComponentEnum::Sound); -> Todo
	entity->setNewType(MyComponentEnum::Animation);

	this->_gameEngine->createComponentPosition(name, 0, 0, 0);
	this->_gameEngine->createComponentSprite(name, "robot.mesh");

	this->_gameEngine->createComponentMoove(name, 100);
	
	this->_gameEngine->createComponentAnimation(name, "Shoot", "Idle", "Walk");

	this->_gameEngine->createPlayer(entity);
	_entity.insert(std::make_pair(name, entity));
}

void MyGame::createWall(std::string name)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>(name);
	entity->setNewType(MyComponentEnum::Position);
	entity->setNewType(MyComponentEnum::Sprite);

	entity->setNewType(MyComponentEnum::Collider);

	this->_gameEngine->createComponentPosition(name, 0, 0, 0);
	this->_gameEngine->createComponentSprite(name, "robot.mesh");

	this->_gameEngine->createEntity(entity);
	_entity.insert(std::make_pair(name, entity));
}

void MyGame::start()
{
	this->createPlayer("Player");
	//this->createWall("Wall");
	_gameEngine->startRendering();
}
