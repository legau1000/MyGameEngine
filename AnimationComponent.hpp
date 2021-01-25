#pragma once
#include "Component.hpp"
class AnimationComponent : public Component
{
public:
	AnimationComponent(const std::string& name, std::string, std::string, std::string);
	~AnimationComponent();

	const std::string getName();

	std::string _idle;
	std::string _attack;
	std::string _mouve;

	std::string getAnimation();

	void setAnimation(std::string which);

private:
	const std::string _name;
	int _animation;
};

