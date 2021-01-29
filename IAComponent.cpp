#include "IAComponent.hpp"

IAComponent::IAComponent(const std::string& name, std::vector<int> startPos, std::vector<int> endPos)
	: Component(MyComponentEnum::IA), _name(name), _startPos(startPos), _endPos(endPos)
{
	if (_startPos[0] == _endPos[0]) {
		_force = {20.f, 0.f, 0.f };
	}
	else {
		_force = { 0.f, 0.f, 20.f };
	}
	_direction = 1;
}

IAComponent::~IAComponent()
{

}

const std::string IAComponent::getName()
{
	return (_name);
}

std::vector<float> IAComponent::getForce()
{
	std::vector<float> tmp = { _force[0] * _direction, 0.f, _force[2] * _direction };
	return (tmp);
}

