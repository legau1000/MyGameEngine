#include "ScaleComponent.hpp"
ScaleComponent::ScaleComponent(const std::string& name, float scaleX, float scaleY, float scaleZ)
	: Component(MyComponentEnum::Scale), _name(name), x(scaleX), y(scaleY), z(scaleZ)
{
}

ScaleComponent::~ScaleComponent()
{

}

const std::string ScaleComponent::getName()
{
	return (_name);
}
