#pragma once
#include "Component.hpp"
class SpriteComponent: public Component
{
public:
	SpriteComponent(const std::string &name, const std::string& mesh);
	~SpriteComponent();

	const std::string getName();

	const std::string getMesh();
private:
	const std::string _name;
	const std::string _mesh;
};

