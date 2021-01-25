#include "PositionComponent.hpp"

PositionComponent::PositionComponent(const std::string& name, float x, float y, float z)
	: Component(MyComponentEnum::Position), _name(name), _x(x), _y(y), _z(z)
{

}

PositionComponent::~PositionComponent()
{

}

const std::string PositionComponent::getName()
{
	return (_name);
}

float PositionComponent::getX()
{
	return (_x);
}

float PositionComponent::getY()
{
	return (_y);
}

float PositionComponent::getZ()
{
	return (_z);
}

void PositionComponent::setX(float x)
{
	_x = x;
}

void PositionComponent::setY(float y)
{
	_y = y;
}

void PositionComponent::setZ(float z)
{
	_z = z;
}

void PositionComponent::addX(float x)
{
	_x += x;
}

void PositionComponent::addY(float y)
{
	_y += y;
}

void PositionComponent::addZ(float z)
{
	_z += z;
}
