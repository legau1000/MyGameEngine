#include "SoundComponent.hpp"

SoundComponent::SoundComponent(const std::string& name, const std::string& path):
	Component(MyComponentEnum::Sound), _name(name), _path(path), isPlaying(false)
{

}

SoundComponent::~SoundComponent()
{

}

const std::string SoundComponent::getName()
{
	return (this->_name);
}

std::string SoundComponent::getPath()
{
	return (this->_path);
}