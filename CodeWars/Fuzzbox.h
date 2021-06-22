#pragma once
#include "SoundType.h"

class Fuzzbox
{
public:
    SoundType Sound = Clean;

    void Switch()
    {
        switch (Sound)
        {
        case Distorted: Sound = Clean;
            break;
        default: Sound = Distorted;
            break;
        }
    };
};
