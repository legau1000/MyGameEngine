#include "ColliderComponent.hpp"

ColliderComponent::ColliderComponent(const std::string& name)
	: Component(MyComponentEnum::Collider), _name(name)
{
	body = nullptr;
}

ColliderComponent::~ColliderComponent()
{

}

const std::string ColliderComponent::getName()
{
	return (_name);
}