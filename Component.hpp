#pragma once

#include "Entity.hpp"

class Component
{
public:
	Component(MyComponentEnum myClass);
	~Component();

	const MyComponentEnum getType();
protected:
	const MyComponentEnum _myClass;
};
