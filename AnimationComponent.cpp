#include "AnimationComponent.hpp"

AnimationComponent::AnimationComponent(const std::string& name, std::string attack, std::string idle, std::string mouve)
	: Component(MyComponentEnum::Animation), _name(name), _attack(attack), _idle(idle), _mouve(mouve), _animation(0)
{
	_animation = 0;
}

AnimationComponent::~AnimationComponent()
{

}

const std::string AnimationComponent::getName()
{
	return (_name);
}

std::string AnimationComponent::getAnimation()
{
	if (_animation == 0) {
		return (_idle);
	}
	if (_animation == 1) {
		return (_attack);
	}
	if (_animation == 2) {
		return (_mouve);
	}
	return (_idle);
}

void AnimationComponent::setAnimation(std::string which)
{
	if (which == "attack")
		_animation = 1;
	else if (which == "mouve")
		_animation = 2;
	else
		_animation = 0;
}
