#pragma once
#include <memory>
#include <unordered_map>
#include <SFML/Audio.hpp>
#include "SoundComponent.hpp"

class MySound
{
public:
	MySound(std::shared_ptr<SoundComponent> music)
	{
		_soundComponent = music;
		if (!this->_Music.openFromFile(_soundComponent->getPath())) {
			_error = true;
			return;
		}
		_error = false;
	};
	~MySound() {};
	void playSound(int loop)
	{
		if (_error == true) {
			//_error = true;
			return;
		}
		this->_Music.play();
		this->_Music.setLoop(loop);
		_soundComponent->isPlaying = true;
	};
	void stopSound()
	{
		if (_error == true) {
			//_error = true;
			return;
		}
		this->_Music.stop();
		_soundComponent->isPlaying = false;
	}
	bool isPlaying()
	{
		return (_soundComponent->isPlaying);
	}
private:
	std::shared_ptr<SoundComponent> _soundComponent;
	sf::Music _Music;
	bool _error;
};

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void CreateSound(std::shared_ptr<SoundComponent>);
	void PlaySound(const std::string& name, bool loop);
	void StopSound(const std::string& name);
	bool CheckPlayed(const std::string& name);

private:
	std::unordered_map<std::string, std::shared_ptr<MySound>> _listSound;
};