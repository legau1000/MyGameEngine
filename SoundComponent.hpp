#pragma once
#include "Component.hpp"
class SoundComponent : public Component
{
public:
	SoundComponent(const std::string& name, const std::string& path);
	~SoundComponent();

	const std::string getName();

	bool isPlaying;

	std::string getPath();

private:
	const std::string _name;
	const std::string _path;
};
