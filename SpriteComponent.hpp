#pragma once
#include "Component.hpp"
class SpriteComponent: public Component
{
public:
	SpriteComponent(const std::string& name, const std::string& mesh);
	SpriteComponent(const std::string& name, const std::string& mesh, const std::string& material);
	~SpriteComponent();

	const std::string getName();

	const std::string getMesh();
	const std::string getMaterial();
private:
	const std::string _name;
	const std::string _mesh;
	const std::string _material;
};

