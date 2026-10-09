#include "PauseMenu.h"
#include "SoundManager.h"

#include <algorithm>
#include <cmath>
#include <string>

namespace
{
    // Modern pause palette: dark glass + warm accent.
    const sf::Color OverlayColor(5, 11, 20, 190);
    const sf::Color CloudBlue(22, 34, 52, 248);
    const sf::Color CloudBlueHighlight(31, 49, 72, 252);
    const sf::Color CloudPink(28, 43, 63, 252);
    const sf::Color Navy(232, 239, 246, 255);
    const sf::Color Gold(242, 184, 74, 255);
    const sf::Color White(255, 255, 255, 255);
    const sf::Color SelectedGreen(105, 245, 105, 255);

    // Entry-animation tuning for the pause overlay's "slide down" reveal.
    constexpr float PAUSE_SLIDE_DURATION = 0.3f;
    constexpr float PAUSE_SLIDE_DISTANCE = 220.f;
}

PauseMenu::PauseMenu(const sf::Font& font)
    : titleText(font),
      resumeText(font),
      settingsText(font),
      mainMenuText(font),
      settingsTitleText(font),
      volumeLabelText(font),
      volumeValueText(font),
      volumeDownText(font),
      volumeUpText(font),
      muteText(font),
      sfxLabelText(font),
      sfxValueText(font),
      sfxDownText(font),
      sfxUpText(font),
      sfxMuteText(font),
      backText(font),
      settingsHintText(font)
{
    pauseSkinLoaded =
        pauseFrameTexture.loadFromFile("Khung (4).png") &&
        pauseButtonTexture.loadFromFile("Button1.png") &&
        pauseIconTexture.loadFromFile("PauseButton.png");

    // Pause icon keeps the aspect ratio of PauseButton.png and stays inside the
    // top-right safe corner of the 1280x720 viewport.
    setupHitbox(pauseButton, { 1188.f, 24.f }, { 72.f, 50.f });

    // Khung (4) has a dedicated title plaque and a tall inner panel.
    // Buttons are slightly narrower and closer so they remain fully inside the frame.
    setupHitbox(resumeButton, { 440.f, 255.f }, { 400.f, 66.f });
    setupHitbox(settingsButton, { 440.f, 345.f }, { 400.f, 66.f });
    setupHitbox(mainMenuButton, { 440.f, 435.f }, { 400.f, 66.f });

    // Compact settings controls, all still rendered with Button1.png.
    setupHitbox(volumeDownButton, { 455.f, 275.f }, { 90.f, 46.f });
    setupHitbox(volumeUpButton, { 735.f, 275.f }, { 90.f, 46.f });
    setupHitbox(sfxDownButton, { 455.f, 385.f }, { 90.f, 46.f });
    setupHitbox(sfxUpButton, { 735.f, 385.f }, { 90.f, 46.f });
    setupHitbox(muteButton, { 420.f, 460.f }, { 215.f, 52.f });
    setupHitbox(sfxMuteButton, { 645.f, 460.f }, { 215.f, 52.f });
    setupHitbox(backButton, { 440.f, 530.f }, { 400.f, 54.f });

    titleText.setString("GAME PAUSED");
    titleText.setCharacterSize(36);
    titleText.setFillColor(Gold);
    titleText.setOutlineColor(sf::Color::Transparent);
    titleText.setOutlineThickness(0.f);

    resumeText.setString("RESUME GAME");
    settingsText.setString("SETTINGS");
    mainMenuText.setString("MAIN MENU");

    resumeText.setCharacterSize(26);
    settingsText.setCharacterSize(26);
    mainMenuText.setCharacterSize(26);

    settingsTitleText.setString("SETTINGS");
    settingsTitleText.setCharacterSize(38);
    settingsTitleText.setFillColor(Gold);
    settingsTitleText.setOutlineColor(sf::Color::Transparent);
    settingsTitleText.setOutlineThickness(0.f);

    volumeLabelText.setString("BGM VOLUME");
    volumeLabelText.setCharacterSize(23);
    volumeLabelText.setFillColor(Navy);

    sfxLabelText.setString("SFX VOLUME");
    sfxLabelText.setCharacterSize(23);
    sfxLabelText.setFillColor(Navy);

    volumeValueText.setCharacterSize(38);
    volumeValueText.setFillColor(Gold);
    volumeValueText.setOutlineColor(sf::Color::Transparent);
    volumeValueText.setOutlineThickness(0.f);

    volumeDownText.setString("-");
    volumeUpText.setString("+");
    volumeDownText.setCharacterSize(36);
    volumeUpText.setCharacterSize(36);

    muteText.setCharacterSize(20);
    sfxMuteText.setCharacterSize(20);

    sfxValueText.setCharacterSize(30);
    sfxValueText.setFillColor(Gold);
    sfxDownText.setString("-");
    sfxUpText.setString("+");
    sfxDownText.setCharacterSize(36);
    sfxUpText.setCharacterSize(36);

    backText.setString("BACK TO PAUSE");
    backText.setCharacterSize(22);

    settingsHintText.setString("Left/Right: BGM | A/D: SFX | M: mute BGM | N: mute SFX | Esc: back");
    settingsHintText.setCharacterSize(17);
    settingsHintText.setFillColor(Navy);

    centerText(resumeText, resumeButton);
    centerText(settingsText, settingsButton);
    centerText(mainMenuText, mainMenuButton);
    centerText(volumeDownText, volumeDownButton);
    centerText(volumeUpText, volumeUpButton);
    centerText(sfxDownText, sfxDownButton);
    centerText(sfxUpText, sfxUpButton);
    centerText(backText, backButton);

    auto titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({
        titleBounds.position.x + titleBounds.size.x / 2.f,
        titleBounds.position.y + titleBounds.size.y / 2.f
    });
    titleText.setPosition({ 640.f, 145.f });

    auto settingsTitleBounds = settingsTitleText.getLocalBounds();
    settingsTitleText.setOrigin({
        settingsTitleBounds.position.x + settingsTitleBounds.size.x / 2.f,
        settingsTitleBounds.position.y + settingsTitleBounds.size.y / 2.f
    });
    settingsTitleText.setPosition({ 640.f, 145.f });

    auto volumeLabelBounds = volumeLabelText.getLocalBounds();
    volumeLabelText.setOrigin({
        volumeLabelBounds.position.x + volumeLabelBounds.size.x / 2.f,
        volumeLabelBounds.position.y + volumeLabelBounds.size.y / 2.f
    });
    volumeLabelText.setPosition({ 640.f, 235.f });

    auto sfxLabelBounds = sfxLabelText.getLocalBounds();
    sfxLabelText.setOrigin({
        sfxLabelBounds.position.x + sfxLabelBounds.size.x / 2.f,
        sfxLabelBounds.position.y + sfxLabelBounds.size.y / 2.f
    });
    sfxLabelText.setPosition({ 640.f, 345.f });

    auto hintBounds = settingsHintText.getLocalBounds();
    settingsHintText.setOrigin({
        hintBounds.position.x + hintBounds.size.x / 2.f,
        hintBounds.position.y + hintBounds.size.y / 2.f
    });
    settingsHintText.setPosition({ 640.f, 610.f });
}

void PauseMenu::setupHitbox(
    sf::RectangleShape& button,
    sf::Vector2f position,
    sf::Vector2f size)
{
    button.setPosition(position);
    button.setSize(size);
    button.setFillColor(sf::Color::Transparent);
}

void PauseMenu::centerText(sf::Text& text, const sf::RectangleShape& button)
{
    const auto bounds = text.getLocalBounds();
    text.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    });

    const auto position = button.getPosition();
    const auto size = button.getSize();
    text.setPosition({
        position.x + size.x / 2.f,
        position.y + size.y / 2.f
    });
}

void PauseMenu::drawRoundedRectangle(
    sf::RenderWindow& window,
    sf::Vector2f position,
    sf::Vector2f size,
    float radius,
    sf::Color fillColor,
    sf::Color outlineColor,
    float outlineThickness) const
{
    auto drawFill = [&window](sf::Vector2f pos, sf::Vector2f rectSize, float r, sf::Color color)
    {
        sf::RectangleShape horizontal({ rectSize.x - 2.f * r, rectSize.y });
        horizontal.setPosition({ pos.x + r, pos.y });
        horizontal.setFillColor(color);
        window.draw(horizontal);

        sf::RectangleShape vertical({ rectSize.x, rectSize.y - 2.f * r });
        vertical.setPosition({ pos.x, pos.y + r });
        vertical.setFillColor(color);
        window.draw(vertical);

        const sf::Vector2f centers[4] = {
            { pos.x + r, pos.y + r },
            { pos.x + rectSize.x - r, pos.y + r },
            { pos.x + r, pos.y + rectSize.y - r },
            { pos.x + rectSize.x - r, pos.y + rectSize.y - r }
        };

        for (const auto& center : centers)
        {
            sf::CircleShape corner(r);
            corner.setOrigin({ r, r });
            corner.setPosition(center);
            corner.setFillColor(color);
            window.draw(corner);
        }
    };

    if (outlineThickness > 0.f)
    {
        drawFill(position, size, radius, outlineColor);

        const sf::Vector2f innerPosition({
            position.x + outlineThickness,
            position.y + outlineThickness
        });
        const sf::Vector2f innerSize({
            size.x - 2.f * outlineThickness,
            size.y - 2.f * outlineThickness
        });
        drawFill(
            innerPosition,
            innerSize,
            std::max(1.f, radius - outlineThickness),
            fillColor);
    }
    else
    {
        drawFill(position, size, radius, fillColor);
    }
}


void PauseMenu::drawTextureRect(
    sf::RenderWindow& window,
    const sf::Texture& texture,
    const sf::RectangleShape& rect,
    sf::Color tint) const
{
    const auto size = texture.getSize();
    if (size.x == 0 || size.y == 0)
        return;

    sf::Sprite sprite(texture);
    sprite.setPosition(rect.getPosition());
    sprite.setScale({
        rect.getSize().x / static_cast<float>(size.x),
        rect.getSize().y / static_cast<float>(size.y)
    });
    sprite.setColor(tint);
    window.draw(sprite);
}

void PauseMenu::drawCloudPanel(sf::RenderWindow& window) const
{
    sf::RectangleShape overlay({ 1280.f, 720.f });
    overlay.setPosition({ 0.f, 0.f });
    overlay.setFillColor(sf::Color(8, 13, 22, 150));
    window.draw(overlay);

    sf::RectangleShape frame;
    frame.setPosition({ 350.f, 62.f });
    frame.setSize({ 580.f, 590.f });
    frame.setFillColor(sf::Color::Transparent);

    if (pauseSkinLoaded)
        drawTextureRect(window, pauseFrameTexture, frame);
    else
        drawRoundedRectangle(window, frame.getPosition(), frame.getSize(), 30.f, CloudBlue, Gold, 2.f);
}

void PauseMenu::drawTitleCloud(sf::RenderWindow& window) const
{
    // The wooden frame already provides a strong title area; no extra badge is
    // drawn here, preventing title decorations from covering the buttons.
    (void)window;
}

bool PauseMenu::isHovered(
    sf::RenderWindow& window,
    const sf::RectangleShape& button) const
{
    const sf::Vector2f mousePosition =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));
    return button.getGlobalBounds().contains(mousePosition);
}

void PauseMenu::drawCloudButton(
    sf::RenderWindow& window,
    const sf::RectangleShape& button,
    sf::Text& text,
    bool selected,
    bool darkText) const
{
    (void)darkText;

    const bool active = selected || isHovered(window, button);

    if (pauseSkinLoaded)
    {
        drawTextureRect(
            window, pauseButtonTexture, button,
            active ? sf::Color::White : sf::Color(238, 238, 238));
    }
    else
    {
        drawRoundedRectangle(
            window, button.getPosition(), button.getSize(), 16.f,
            active ? CloudBlueHighlight : CloudBlue, Gold, active ? 2.5f : 1.5f);
    }

    text.setFillColor(active ? SelectedGreen : White);
    text.setOutlineColor(sf::Color(65, 34, 18, 190));
    text.setOutlineThickness(1.2f);
    window.draw(text);
}

void PauseMenu::drawPauseIcon(
    sf::RenderWindow& window,
    sf::Vector2f center,
    float scale,
    sf::Color color) const
{
    const sf::Vector2f barSize({ 12.f * scale, 39.f * scale });
    const float radius = 5.f * scale;

    drawRoundedRectangle(
        window,
        { center.x - 18.f * scale, center.y - 19.5f * scale },
        barSize,
        radius,
        color,
        sf::Color::Transparent,
        0.f);

    drawRoundedRectangle(
        window,
        { center.x + 6.f * scale, center.y - 19.5f * scale },
        barSize,
        radius,
        color,
        sf::Color::Transparent,
        0.f);
}

void PauseMenu::drawGearIcon(
    sf::RenderWindow& window,
    sf::Vector2f center,
    float scale,
    sf::Color color) const
{
    sf::CircleShape outer(21.f * scale);
    outer.setOrigin({ 21.f * scale, 21.f * scale });
    outer.setPosition(center);
    outer.setFillColor(color);
    window.draw(outer);

    sf::CircleShape hole(8.f * scale);
    hole.setOrigin({ 8.f * scale, 8.f * scale });
    hole.setPosition(center);
    hole.setFillColor(CloudBlue);
    window.draw(hole);

    const sf::Vector2f offsets[] = {
        { 0.f, -27.f }, { 0.f, 27.f }, { -27.f, 0.f }, { 27.f, 0.f },
        { -19.f, -19.f }, { 19.f, -19.f }, { -19.f, 19.f }, { 19.f, 19.f }
    };

    for (const auto& offset : offsets)
    {
        sf::CircleShape tooth(6.f * scale);
        tooth.setOrigin({ 6.f * scale, 6.f * scale });
        tooth.setPosition({ center.x + offset.x * scale, center.y + offset.y * scale });
        tooth.setFillColor(color);
        window.draw(tooth);
    }
}

void PauseMenu::drawDoorIcon(
    sf::RenderWindow& window,
    sf::Vector2f center,
    float scale,
    sf::Color color) const
{
    sf::RectangleShape door({ 31.f * scale, 48.f * scale });
    door.setOrigin({ 15.5f * scale, 24.f * scale });
    door.setPosition(center);
    door.setFillColor(sf::Color::Transparent);
    door.setOutlineColor(color);
    door.setOutlineThickness(5.f * scale);
    window.draw(door);

    sf::CircleShape knob(3.f * scale);
    knob.setOrigin({ 3.f * scale, 3.f * scale });
    knob.setPosition({ center.x + 8.f * scale, center.y });
    knob.setFillColor(color);
    window.draw(knob);

    sf::RectangleShape floor({ 42.f * scale, 5.f * scale });
    floor.setOrigin({ 21.f * scale, 2.5f * scale });
    floor.setPosition({ center.x, center.y + 28.f * scale });
    floor.setFillColor(color);
    window.draw(floor);
}

void PauseMenu::drawBackIcon(
    sf::RenderWindow& window,
    sf::Vector2f center,
    float scale,
    sf::Color color) const
{
    sf::RectangleShape shaft({ 38.f * scale, 8.f * scale });
    shaft.setOrigin({ 19.f * scale, 4.f * scale });
    shaft.setPosition({ center.x + 8.f * scale, center.y });
    shaft.setFillColor(color);
    window.draw(shaft);

    sf::ConvexShape head(3);
    head.setPoint(0, { 0.f, 15.f * scale });
    head.setPoint(1, { 22.f * scale, 0.f });
    head.setPoint(2, { 22.f * scale, 30.f * scale });
    head.setOrigin({ 11.f * scale, 15.f * scale });
    head.setPosition({ center.x - 16.f * scale, center.y });
    head.setFillColor(color);
    window.draw(head);
}

void PauseMenu::drawSpeakerIcon(
    sf::RenderWindow& window,
    sf::Vector2f center,
    float scale,
    sf::Color color,
    bool muted) const
{
    sf::RectangleShape body({ 12.f * scale, 20.f * scale });
    body.setOrigin({ 6.f * scale, 10.f * scale });
    body.setPosition({ center.x - 11.f * scale, center.y });
    body.setFillColor(color);
    window.draw(body);

    sf::ConvexShape cone(3);
    cone.setPoint(0, { 0.f, 12.f * scale });
    cone.setPoint(1, { 20.f * scale, 0.f });
    cone.setPoint(2, { 20.f * scale, 24.f * scale });
    cone.setOrigin({ 10.f * scale, 12.f * scale });
    cone.setPosition({ center.x + 2.f * scale, center.y });
    cone.setFillColor(color);
    window.draw(cone);

    if (muted)
    {
        sf::RectangleShape slashA({ 5.f * scale, 28.f * scale });
        slashA.setOrigin({ 2.5f * scale, 14.f * scale });
        slashA.setPosition({ center.x + 29.f * scale, center.y });
        slashA.setFillColor(color);
        window.draw(slashA);

        sf::RectangleShape slashB({ 28.f * scale, 5.f * scale });
        slashB.setOrigin({ 14.f * scale, 2.5f * scale });
        slashB.setPosition({ center.x + 29.f * scale, center.y });
        slashB.setFillColor(color);
        window.draw(slashB);
    }
    else
    {
        sf::CircleShape wave(14.f * scale, 32);
        wave.setOrigin({ 14.f * scale, 14.f * scale });
        wave.setPosition({ center.x + 22.f * scale, center.y });
        wave.setFillColor(sf::Color::Transparent);
        wave.setOutlineColor(color);
        wave.setOutlineThickness(3.f * scale);
        window.draw(wave);
    }
}

bool PauseMenu::handlePauseButtonClick(
    sf::RenderWindow& window,
    const sf::Event& event) const
{
    const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>();
    if (mouse == nullptr || mouse->button != sf::Mouse::Button::Left)
        return false;

    const sf::Vector2f position = window.mapPixelToCoords(mouse->position);
    return pauseButton.getGlobalBounds().contains(position);
}

PauseMenu::Action PauseMenu::handleEvent(
    sf::RenderWindow& window,
    const sf::Event& event,
    sf::Music& music)
{
    if (const auto* key = event.getIf<sf::Event::KeyPressed>())
    {
        if (settingsOpen)
        {
            if (key->code == sf::Keyboard::Key::Escape ||
                key->code == sf::Keyboard::Key::Backspace)
            {
                settingsOpen = false;
                selectedItem = 0;
                return Action::None;
            }

            if (key->code == sf::Keyboard::Key::Left)
                changeVolume(music, -10.f);

            if (key->code == sf::Keyboard::Key::Right)
                changeVolume(music, 10.f);

            if (key->code == sf::Keyboard::Key::M)
                toggleMute(music);

            if (key->code == sf::Keyboard::Key::A)
                SoundManager::instance().setVolume(SoundManager::instance().getVolume() - 10.f);
            if (key->code == sf::Keyboard::Key::D)
                SoundManager::instance().setVolume(SoundManager::instance().getVolume() + 10.f);
            if (key->code == sf::Keyboard::Key::N)
                SoundManager::instance().toggleMute();

            return Action::None;
        }

        if (key->code == sf::Keyboard::Key::Escape)
            return Action::Resume;

        if (key->code == sf::Keyboard::Key::Up)
        {
            selectedItem--;
            if (selectedItem < 0)
                selectedItem = 2;
        }

        if (key->code == sf::Keyboard::Key::Down)
        {
            selectedItem++;
            if (selectedItem > 2)
                selectedItem = 0;
        }

        if (key->code == sf::Keyboard::Key::Enter)
        {
            if (selectedItem == 0)
                return Action::Resume;

            if (selectedItem == 1)
            {
                settingsOpen = true;
                return Action::None;
            }

            if (selectedItem == 2)
                return Action::MainMenu;
        }
    }

    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouse->button != sf::Mouse::Button::Left)
            return Action::None;

        const sf::Vector2f position = window.mapPixelToCoords(mouse->position);

        if (settingsOpen)
        {
            if (volumeDownButton.getGlobalBounds().contains(position))
                changeVolume(music, -10.f);
            else if (volumeUpButton.getGlobalBounds().contains(position))
                changeVolume(music, 10.f);
            else if (sfxDownButton.getGlobalBounds().contains(position))
                SoundManager::instance().setVolume(SoundManager::instance().getVolume() - 10.f);
            else if (sfxUpButton.getGlobalBounds().contains(position))
                SoundManager::instance().setVolume(SoundManager::instance().getVolume() + 10.f);
            else if (muteButton.getGlobalBounds().contains(position))
                toggleMute(music);
            else if (sfxMuteButton.getGlobalBounds().contains(position))
                SoundManager::instance().toggleMute();
            else if (backButton.getGlobalBounds().contains(position))
                settingsOpen = false;

            return Action::None;
        }

        if (resumeButton.getGlobalBounds().contains(position))
            return Action::Resume;

        if (settingsButton.getGlobalBounds().contains(position))
        {
            settingsOpen = true;
            return Action::None;
        }

        if (mainMenuButton.getGlobalBounds().contains(position))
            return Action::MainMenu;
    }

    return Action::None;
}

void PauseMenu::changeVolume(sf::Music& music, float amount)
{
    const float newVolume = std::clamp(music.getVolume() + amount, 0.f, 100.f);
    music.setVolume(newVolume);

    if (newVolume > 0.f)
        volumeBeforeMute = newVolume;
}

void PauseMenu::toggleMute(sf::Music& music)
{
    if (music.getVolume() > 0.f)
    {
        volumeBeforeMute = music.getVolume();
        music.setVolume(0.f);
    }
    else
    {
        music.setVolume(std::max(volumeBeforeMute, 10.f));
    }
}

void PauseMenu::drawPauseButton(sf::RenderWindow& window)
{
    const bool hovered = isHovered(window, pauseButton);

    if (pauseSkinLoaded)
    {
        drawTextureRect(
            window, pauseIconTexture, pauseButton,
            hovered ? sf::Color::White : sf::Color(235, 235, 235));
    }
    else
    {
        drawRoundedRectangle(
            window, pauseButton.getPosition(), pauseButton.getSize(), 14.f,
            hovered ? CloudBlueHighlight : CloudBlue, Gold, 2.f);
        drawPauseIcon(window, { 1224.f, 49.f }, 0.58f, hovered ? SelectedGreen : White);
    }
}

void PauseMenu::draw(sf::RenderWindow& window, float musicVolume)
{
    // Slide the whole pause panel down from above the screen into place.
    const float elapsed = entryAnimClock.getElapsedTime().asSeconds();
    const float progress = std::min(1.f, elapsed / PAUSE_SLIDE_DURATION);
    const float eased = 1.f - std::pow(1.f - progress, 3.f); // ease-out cubic
    const float offsetY = -PAUSE_SLIDE_DISTANCE * (1.f - eased); // -DIST -> 0

    const sf::View defaultView = window.getView();
    if (offsetY < -0.5f)
    {
        sf::View slideView = defaultView;
        slideView.setCenter({
            defaultView.getSize().x / 2.f,
            defaultView.getSize().y / 2.f - offsetY
        });
        window.setView(slideView);
    }

    drawCloudPanel(window);
    drawTitleCloud(window);

    if (!settingsOpen)
    {
        window.draw(titleText);
        drawCloudButton(window, resumeButton, resumeText, selectedItem == 0, false);
        drawCloudButton(window, settingsButton, settingsText, selectedItem == 1, false);
        drawCloudButton(window, mainMenuButton, mainMenuText, selectedItem == 2, false);

        window.setView(defaultView);
        return;
    }

    window.draw(settingsTitleText);
    window.draw(volumeLabelText);
    window.draw(sfxLabelText);

    volumeValueText.setString(
        std::to_string(static_cast<int>(std::round(musicVolume))) + "%");
    auto valueBounds = volumeValueText.getLocalBounds();
    volumeValueText.setOrigin({
        valueBounds.position.x + valueBounds.size.x / 2.f,
        valueBounds.position.y + valueBounds.size.y / 2.f
    });
    volumeValueText.setPosition({ 640.f, 298.f });

    const auto& sfx = SoundManager::instance();
    sfxValueText.setString(std::to_string(static_cast<int>(std::round(sfx.getVolume()))) + "%");
    valueBounds = sfxValueText.getLocalBounds();
    sfxValueText.setOrigin({
        valueBounds.position.x + valueBounds.size.x / 2.f,
        valueBounds.position.y + valueBounds.size.y / 2.f
    });
    sfxValueText.setPosition({ 640.f, 408.f });

    muteText.setString(musicVolume <= 0.f ? "UNMUTE BGM" : "MUTE BGM");
    sfxMuteText.setString(sfx.isMuted() ? "UNMUTE SFX" : "MUTE SFX");
    centerText(muteText, muteButton);
    centerText(sfxMuteText, sfxMuteButton);

    drawCloudButton(window, volumeDownButton, volumeDownText, false, true);
    drawCloudButton(window, volumeUpButton, volumeUpText, false, true);
    drawCloudButton(window, sfxDownButton, sfxDownText, false, true);
    drawCloudButton(window, sfxUpButton, sfxUpText, false, true);
    window.draw(volumeValueText);
    window.draw(sfxValueText);
    drawCloudButton(window, muteButton, muteText, false, true);
    drawCloudButton(window, sfxMuteButton, sfxMuteText, false, true);
    drawCloudButton(window, backButton, backText, false, true);

    // Text-only controls prevent decorative icons from colliding with the new wood buttons.
    window.draw(settingsHintText);

    window.setView(defaultView);
}

void PauseMenu::notifyOpened()
{
    entryAnimClock.restart();
}

void PauseMenu::reset()
{
    settingsOpen = false;
    selectedItem = 0;
}
