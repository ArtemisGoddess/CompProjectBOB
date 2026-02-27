#pragma once

#include "constants.h"

class weeWoo {
    private:
        CRGB currentColour = CRGB::Red;
    public:
        // Rapidly changes the led between red and blue to make a weewoo
        CRGB doWeeWoo();
};