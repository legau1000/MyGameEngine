#pragma once
#include <vector>
#include "Component.hpp"
class IAComponent : public Component
{
public:
	IAComponent(const std::string& name, std::vector<int> startPos, std::vector<int> endPos);
	~IAComponent();

	const std::string getName();

	std::vector<float> getForce();

	std::vector<int> _startPos;
	std::vector<int> _endPos;

	std::vector<float> _force;
private:
	const std::string _name;
	int _direction = 1;
};

