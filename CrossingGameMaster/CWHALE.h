#pragma once
#include "CANIMAL.h"

class CWHALE : public CANIMAL
{
public:
    CWHALE(const sf::Texture& texture, float speed);

    void Tell() override;
};