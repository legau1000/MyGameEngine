#pragma once
#include "Component.hpp"
class OrientationComponent : public Component
{
public:
	OrientationComponent(const std::string& name, float degree);
	~OrientationComponent();

	const std::string getName();

	float _degree;

private:
	const std::string _name;
};

	