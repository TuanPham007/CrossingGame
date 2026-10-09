#pragma once

#include <SFML/Audio.hpp>

class SoundManager
{
public:
    static SoundManager& instance();

    bool load();

    void playMove();
    void playHit();
    void playStage();
    void playWin();
    void playLose();
    void stopLose();

    void setVolume(float value);
    float getVolume() const { return volume; }
    void toggleMute();
    bool isMuted() const { return muted; }

private:
    SoundManager();
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

    sf::SoundBuffer moveBuffer;
    sf::SoundBuffer hitBuffer;
    sf::SoundBuffer stageBuffer;
    sf::SoundBuffer winBuffer;
    sf::SoundBuffer loseBuffer;

    sf::Sound moveSound;
    sf::Sound hitSound;
    sf::Sound stageSound;
    sf::Sound winSound;
    sf::Sound loseSound;

    float volume = 80.f;
    float volumeBeforeMute = 80.f;
    bool muted = false;

    void applyVolume();
};
