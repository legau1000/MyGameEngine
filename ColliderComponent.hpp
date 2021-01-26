#pragma once
#include "Component.hpp"
class ColliderComponent: public Component
{
public:
	ColliderComponent(const std::string& name);
	~ColliderComponent();
	
	const std::string getName();

	void* body;

private:
	const std::string _name;

};

