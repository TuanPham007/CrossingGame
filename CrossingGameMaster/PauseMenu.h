#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class PauseMenu
{
public:
    enum class Action
    {
        None,
        Resume,
        MainMenu
    };

    explicit PauseMenu(const sf::Font& font);

    bool handlePauseButtonClick(
        sf::RenderWindow& window,
        const sf::Event& event) const;

    Action handleEvent(
        sf::RenderWindow& window,
        const sf::Event& event,
        sf::Music& music);

    void drawPauseButton(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window, float musicVolume);
    void reset();

    // Restarts the "slide down from top" entry animation. Call this every
    // time gameplay transitions into CGAME::Paused.
    void notifyOpened();

private:
    bool settingsOpen = false;
    int selectedItem = 0;
    float volumeBeforeMute = 100.f;

    sf::Clock entryAnimClock;

    // Wood/vine pause UI skin. These are loaded independently from Menu so the
    // pause overlay can be reused while gameplay is active.
    sf::Texture pauseFrameTexture;   // Khung (4).png
    sf::Texture pauseButtonTexture;  // Button1.png
    sf::Texture pauseIconTexture;    // PauseButton.png
    bool pauseSkinLoaded = false;

    sf::RectangleShape pauseButton;
    sf::RectangleShape resumeButton;
    sf::RectangleShape settingsButton;
    sf::RectangleShape mainMenuButton;
    sf::RectangleShape volumeDownButton;
    sf::RectangleShape volumeUpButton;
    sf::RectangleShape muteButton;
    sf::RectangleShape sfxDownButton;
    sf::RectangleShape sfxUpButton;
    sf::RectangleShape sfxMuteButton;
    sf::RectangleShape backButton;

    sf::Text titleText;
    sf::Text resumeText;
    sf::Text settingsText;
    sf::Text mainMenuText;
    sf::Text settingsTitleText;
    sf::Text volumeLabelText;
    sf::Text volumeValueText;
    sf::Text volumeDownText;
    sf::Text volumeUpText;
    sf::Text muteText;
    sf::Text sfxLabelText;
    sf::Text sfxValueText;
    sf::Text sfxDownText;
    sf::Text sfxUpText;
    sf::Text sfxMuteText;
    sf::Text backText;
    sf::Text settingsHintText;

    void setupHitbox(
        sf::RectangleShape& button,
        sf::Vector2f position,
        sf::Vector2f size);

    void centerText(sf::Text& text, const sf::RectangleShape& button);

    void drawRoundedRectangle(
        sf::RenderWindow& window,
        sf::Vector2f position,
        sf::Vector2f size,
        float radius,
        sf::Color fillColor,
        sf::Color outlineColor,
        float outlineThickness) const;

    void drawTextureRect(
        sf::RenderWindow& window,
        const sf::Texture& texture,
        const sf::RectangleShape& rect,
        sf::Color tint = sf::Color::White) const;

    void drawCloudPanel(sf::RenderWindow& window) const;
    void drawTitleCloud(sf::RenderWindow& window) const;

    void drawCloudButton(
        sf::RenderWindow& window,
        const sf::RectangleShape& button,
        sf::Text& text,
        bool selected,
        bool darkText = true) const;

    void drawPauseIcon(
        sf::RenderWindow& window,
        sf::Vector2f center,
        float scale,
        sf::Color color) const;

    void drawGearIcon(
        sf::RenderWindow& window,
        sf::Vector2f center,
        float scale,
        sf::Color color) const;

    void drawDoorIcon(
        sf::RenderWindow& window,
        sf::Vector2f center,
        float scale,
        sf::Color color) const;

    void drawBackIcon(
        sf::RenderWindow& window,
        sf::Vector2f center,
        float scale,
        sf::Color color) const;

    void drawSpeakerIcon(
        sf::RenderWindow& window,
        sf::Vector2f center,
        float scale,
        sf::Color color,
        bool muted) const;

    bool isHovered(
        sf::RenderWindow& window,
        const sf::RectangleShape& button) const;

    void changeVolume(sf::Music& music, float amount);
    void toggleMute(sf::Music& music);
};
