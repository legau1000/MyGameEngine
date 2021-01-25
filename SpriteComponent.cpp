#include "SpriteComponent.hpp"

SpriteComponent::SpriteComponent(const std::string& name, const std::string& mesh)
	: Component(MyComponentEnum::Sprite), _name(name), _mesh(mesh)
{

}

SpriteComponent::~SpriteComponent()
{

}

const std::string SpriteComponent::getName()
{
	return (_name);
}

const std::string SpriteComponent::getMesh()
{
	return (_mesh);
}
