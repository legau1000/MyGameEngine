#pragma once

#include <iostream>
#include <array>

enum MyComponentEnum
{
    Collider,
    Position,
    Sprite,
    Cam,
    Scale,
    Orientation,
    Wall,
    Event,
    Moove,
    Turn,
    IA,
    Shoot,
    Sound,
    Animation,
    Empty,
    First = Collider,
    Last = Empty
};

class Entity
{
public:
	Entity(const std::string& entityName);
	~Entity();

    std::string getName() const;

    void setNewType(MyComponentEnum);
    bool getType(MyComponentEnum);

private:
	const std::string &name;
    std::array<bool, MyComponentEnum::Last> _type;
};

