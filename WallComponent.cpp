#include "WallComponent.hpp"

WallComponent::WallComponent(const std::string& name,float orien)
	: Component(MyComponentEnum::Wall), _name(name), orientation(orien)
{
}

WallComponent::~WallComponent()
{

}
const std::string WallComponent::getName()
{
	return (_name);
}