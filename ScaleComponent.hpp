#pragma once
#include "Component.hpp"
class ScaleComponent: public Component
{
public:
	ScaleComponent(const std::string& name, float, float, float);
	~ScaleComponent();

	const std::string getName();

	float x;
	float y;
	float z;

private:
	const std::string _name;
};

