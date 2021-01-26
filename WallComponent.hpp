#pragma once
#include "Component.hpp"
class WallComponent : public Component
{
public:
	WallComponent(const std::string& name, float);
	~WallComponent();

	const std::string getName();

	float orientation;

private:
	const std::string _name;
};

