#include "SoundManager.hpp"

SoundManager::SoundManager()
{
    std::shared_ptr<SoundComponent> sound = std::make_shared<SoundComponent>("BackMusic", "./Assets/gameMusic.ogg");
    this->CreateSound(sound);
    this->PlaySound(sound->getName(), true);
    std::shared_ptr<SoundComponent> soundShoot = std::make_shared<SoundComponent>("Shoot", "./Assets/shoot.ogg");
    this->CreateSound(soundShoot);
}

SoundManager::~SoundManager()
{

}

void SoundManager::CreateSound(std::shared_ptr<SoundComponent> sound)
{
    _listSound.insert(std::make_pair(sound->getName(), std::make_shared<MySound>(sound)));
}

void SoundManager::PlaySound(const std::string& name, bool loop)
{
    std::shared_ptr<MySound> tmp = _listSound.find(name)->second;
    tmp->playSound(loop);
}

void SoundManager::StopSound(const std::string& name)
{
    std::shared_ptr<MySound> tmp = _listSound.find(name)->second;
    tmp->stopSound();
}

bool SoundManager::CheckPlayed(const std::string& name)
{
    std::shared_ptr<MySound> tmp = _listSound.find(name)->second;
    return (tmp->isPlaying());
}
