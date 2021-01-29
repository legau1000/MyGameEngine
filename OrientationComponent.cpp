#include "OrientationComponent.hpp"

OrientationComponent::OrientationComponent(const std::string& name, float degree)
	: Component(MyComponentEnum::Orientation), _name(name)
{
	_degree = degree;
}

OrientationComponent::~OrientationComponent()
{

}

const std::string OrientationComponent::getName()
{
	return (_name);
}