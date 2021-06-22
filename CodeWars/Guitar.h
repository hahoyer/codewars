#pragma once
#include <vector>

#include "Fuzzbox.h"
#include "SoundType.h"

class Guitar
{
public:
    const Fuzzbox Default;
    const Fuzzbox* Effect = &Default;
    void AddEffect(const Fuzzbox& fuzzbox){Effect = &fuzzbox;};
    SoundType Sound(){return Effect->Sound;};
};
