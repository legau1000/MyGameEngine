#include "MooveComponent.hpp"

MooveComponent::MooveComponent(const std::string& name, int speed)
	: Component(MyComponentEnum::Moove), _name(name), _speed(speed)
{

}

MooveComponent::~MooveComponent()
{

}

const std::string MooveComponent::getName()
{
	return (_name);
}

int MooveComponent::getSpeed()
{
	return (_speed);
}

void MooveComponent::setSpeed(int speed)
{
	_speed = speed;
}

void MooveComponent::addSpeed(int speed)
{
	_speed += speed;
}
