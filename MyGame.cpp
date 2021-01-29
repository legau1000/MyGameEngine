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

void MyGame::createIA(std::string name, std::vector<int> startPos, std::vector<int> endPos, float orientation)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>(name);
	entity->setNewType(MyComponentEnum::Position);
	entity->setNewType(MyComponentEnum::Sprite);

	entity->setNewType(MyComponentEnum::Collider);
	entity->setNewType(MyComponentEnum::Scale);

	entity->setNewType(MyComponentEnum::Moove);
	entity->setNewType(MyComponentEnum::IA);

	entity->setNewType(MyComponentEnum::Orientation);

	// entity->setNewType(MyComponentEnum::Shoot); // ->Todo

	entity->setNewType(MyComponentEnum::Animation);

	this->_gameEngine->createComponentPosition(name, startPos[0], startPos[1], startPos[2]);
	this->_gameEngine->createComponentIA(name, startPos, endPos);
	this->_gameEngine->createComponentSprite(name, "ninja.mesh");

	this->_gameEngine->createComponentCollider(name);
	this->_gameEngine->createComponentMoove(name, 100);

	this->_gameEngine->createComponentAnimation(name, "Attack1", "Idle1", "Walk");

	this->_gameEngine->createComponentScale(name, 0.5f, 0.5f, 0.5f);
	this->_gameEngine->createComponentOrientation(name, orientation);

	this->_gameEngine->createEntity(entity);
	_entity.insert(std::make_pair(name, entity));
}

void MyGame::createBloc(std::string name, std::vector<int> pos, std::vector<float> scale)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>(name);
	entity->setNewType(MyComponentEnum::Position);
	entity->setNewType(MyComponentEnum::Sprite);

	entity->setNewType(MyComponentEnum::Collider);

	entity->setNewType(MyComponentEnum::Scale);

	this->_gameEngine->createComponentPosition(name, pos[0], pos[1], pos[2]);
	this->_gameEngine->createComponentSprite(name, "cube.mesh", "Examples/Rocky");

	this->_gameEngine->createComponentCollider(name);

	this->_gameEngine->createComponentScale(name, scale[0], scale[1], scale[2]);

	this->_gameEngine->createEntity(entity);
	_entity.insert(std::make_pair(name, entity));
}

void MyGame::start()
{
	this->createPlayer("Player");
	
	// Create Contour Blocs
	this->createBloc("Wall-1", { 0, 0, -750 }, { 15.f, 1.0f, 0.2f });
	this->createBloc("Wall-2", { 0, 0, 750 }, { 15.f, 1.0f, 0.2f });
	this->createBloc("Wall-3", { -750, 0, 0 }, { 0.2f, 1.0f, 15.f });
	this->createBloc("Wall-4", { 750, 0, 0 }, { 0.2f, 1.0f, 15.f });

	// Create Inside Blocs
	this->createBloc("Wall-5", { 100, 0, 100 }, { 3.2f, 1.0f, 0.2f });
	this->createBloc("Wall-6", { -50, 0,-100 }, { 0.2f, 1.0f, 4.f });
	this->createBloc("Wall-7", { 100, 0,-100 }, { 0.2f, 1.0f, 4.f });
	this->createBloc("Wall-8", { -100, 0, -300 }, { 1.2f, 1.0f, 0.2f });

	this->createBloc("Wall-9", { -100, 0, -450 }, { 1.2f, 1.0f, 0.2f });
	this->createBloc("Wall-10", { -150, 0, -100 }, { 0.2f, 1.0f, 4.f });

	this->createBloc("Wall-11", { -50, 0, -500 }, { 0.2f, 1.0f, 1.2f });
	this->createBloc("Wall-12", { -100, 0, -550 }, { 1.2f, 1.0f, 0.2f });

	this->createBloc("Wall-13", { -250, 0, -500 }, { 0.2f, 1.0f, 1.2f });

	this->createBloc("Wall-14", { 50, 0, -650 }, { 14.f, 1.0f, 0.2f });
	this->createBloc("Wall-15", { -50, 0, 650 }, { 14.f, 1.0f, 0.2f });
	this->createBloc("Wall-16", { -650, 0, -600 }, { 0.2f, 1.0f, 1.2f });
	this->createBloc("Wall-17", { 650, 0, 600 }, { 0.2f, 1.0f, 1.2f });

	this->createBloc("Wall-18", { -700, 0, -450 }, { 1.2f, 1.0f, 0.2f });
	this->createBloc("Wall-19", { 700, 0, 450 }, { 1.2f, 1.0f, 0.2f });

	this->createBloc("Wall-20", { -200, 0, -550 }, { 1.2f, 1.0f, 0.2f });
	this->createBloc("Wall-21", { 250, 0,-100 }, { 0.2f, 1.0f, 4.f });

	this->createBloc("Wall-22", { 400, 0,-100 }, { 0.2f, 1.0f, 4.f });
	this->createBloc("Wall-23", { 550, 0,-100 }, { 0.2f, 1.0f, 4.f });

	this->createBloc("Wall-24", { 650, 0, 250 }, { 0.2f, 1.0f, 4.f });
	this->createBloc("Wall-25", { -650, 0, -250 }, { 0.2f, 1.0f, 4.f });

	//create IA
	this->createIA("IA-1", { 650, 10, -710 }, { 130, 10, -710 }, 90.f);
	this->createIA("IA-2", { -680, 10, 710 }, { 130, 10, 710 }, -90.f);

	_gameEngine->startRendering();
}
