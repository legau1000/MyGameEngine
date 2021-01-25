#pragma once
#include "Component.hpp"
class PositionComponent : public Component
{
public:
	PositionComponent(const std::string& name, float x, float y, float z);
	~PositionComponent();

	const std::string getName();

	float getX();
	float getY();
	float getZ();

	void setX(float);
	void setY(float);
	void setZ(float);

	void addX(float);
	void addY(float);
	void addZ(float);
private:
	const std::string _name;
	float _x;
	float _y;
	float _z;
};
