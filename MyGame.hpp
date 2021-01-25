#pragma once

#include <utility>
#include <memory>
#include "GameEngine.hpp"
#include "Entity.hpp"


class MyGame
{
public:
    MyGame(GameEngine*);
    ~MyGame();

    void start();

private:
    void createPlayer(std::string name);
    void createWall(std::string name);

    GameEngine* _gameEngine;
    std::unordered_map<std::string, std::shared_ptr<Entity>> _entity;
};
