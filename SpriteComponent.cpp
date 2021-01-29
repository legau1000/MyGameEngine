#include "SpriteComponent.hpp"

SpriteComponent::SpriteComponent(const std::string& name, const std::string& mesh)
	: Component(MyComponentEnum::Sprite), _name(name), _mesh(mesh), _material("")
{
}

SpriteComponent::SpriteComponent(const std::string& name, const std::string& mesh, const std::string& material)
	: Component(MyComponentEnum::Sprite), _name(name), _mesh(mesh), _material(material)
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

const std::string SpriteComponent::getMaterial()
{
	return (_material);
}
