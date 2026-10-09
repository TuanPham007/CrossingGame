#include "SoundManager.h"

#include <algorithm>

SoundManager::SoundManager()
    : moveSound(moveBuffer),
      hitSound(hitBuffer),
      stageSound(stageBuffer),
      winSound(winBuffer),
      loseSound(loseBuffer)
{
}

SoundManager& SoundManager::instance()
{
    static SoundManager manager;
    return manager;
}

bool SoundManager::load()
{
    // Existing project audio is reused; no new external assets are required.
    const bool moveOk = moveBuffer.loadFromFile("Move.wav");
    const bool hitOk = hitBuffer.loadFromFile("ByeBye.wav");
    const bool stageOk = stageBuffer.loadFromFile("Success.wav");
    const bool winOk = winBuffer.loadFromFile("win.wav");
    const bool loseOk = loseBuffer.loadFromFile("lose.wav");
    applyVolume();
    return moveOk && hitOk && stageOk && winOk && loseOk;
}

void SoundManager::applyVolume()
{
    const float effective = muted ? 0.f : volume;
    moveSound.setVolume(effective);
    hitSound.setVolume(effective);
    stageSound.setVolume(effective);
    winSound.setVolume(effective);
    loseSound.setVolume(effective);
}

void SoundManager::setVolume(float value)
{
    volume = std::clamp(value, 0.f, 100.f);
    if (volume > 0.f)
        volumeBeforeMute = volume;
    muted = false;
    applyVolume();
}

void SoundManager::toggleMute()
{
    if (!muted)
    {
        volumeBeforeMute = std::max(volume, 10.f);
        muted = true;
    }
    else
    {
        muted = false;
        volume = std::max(volumeBeforeMute, 10.f);
    }
    applyVolume();
}

void SoundManager::playMove()  { moveSound.play(); }
void SoundManager::playHit()   { hitSound.play(); }
void SoundManager::playStage() { stageSound.play(); }
void SoundManager::playWin()   { winSound.play(); }
void SoundManager::playLose()  { loseSound.play(); }
void SoundManager::stopLose()  { loseSound.stop(); }
