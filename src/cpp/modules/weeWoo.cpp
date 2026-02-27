#include "weeWoo.h"

CRGB weeWoo::doWeeWoo() {
    if (COUNT % 100 == 0){
        if (this->currentColour == CRGB::Blue){
            return CRGB::Red;
        } else{
            return CRGB::Blue;
        }
    }
}