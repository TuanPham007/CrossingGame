#pragma once
#include "CANIMAL.h"

class CBIRD : public CANIMAL
{
public:
    CBIRD(const sf::Texture& texture, float speed);

    void Tell() override;
};