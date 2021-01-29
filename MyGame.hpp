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
    void createBloc(std::string name, std::vector<int> pos, std::vector<float> scale);
    void createIA(std::string name, std::vector<int> startPos, std::vector<int> endPos, float orientation);

    GameEngine* _gameEngine;
    std::unordered_map<std::string, std::shared_ptr<Entity>> _entity;
};
