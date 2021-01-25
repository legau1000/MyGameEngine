#pragma once
#include "Component.hpp"
class MooveComponent : public Component
{
public:
	MooveComponent(const std::string& name, int speed);
	~MooveComponent();

	const std::string getName();

	int getSpeed();
	void setSpeed(int speed);
	void addSpeed(int speed);

	int _speed;

private:
	const std::string _name;
};

