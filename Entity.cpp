#include "Entity.hpp"

Entity::Entity(const std::string &entityName): name(entityName)
{
	for (int index = 0; index < MyComponentEnum::Last; index++) {
		_type[index] = false;
	}
}

Entity::~Entity()
{}

std::string Entity::getName() const
{
	return (name);
}

void Entity::setNewType(MyComponentEnum newType)
{
	_type[newType] = true;
}

bool Entity::getType(MyComponentEnum toCheck)
{
	return (_type[toCheck]);
}