#pragma once
#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

#include "Guitar.h"

Describe(a_guitar_with_a_fuzzbox)
{
    void SetUp()
    {
        guitar.AddEffect(fuzzbox);
    }

    It(starts_in_clean_mode)
    {
        Assert::That(guitar.Sound(), Equals(Clean));
    }

    Describe(in_distorted_mode)
    {
        void SetUp()
        {
            Parent().fuzzbox.Switch();
        }

        It(sounds_distorted)
        {
            Assert::That(Parent().guitar.Sound(), Equals(Distorted));
        }

        It(sounds_clean_when_I_switch_the_fuzzbox)
        {
            Parent().fuzzbox.Switch();
            Assert::That(Parent().guitar.Sound(), Equals(Clean));
        }
    };

    Fuzzbox fuzzbox;
    Guitar guitar;
};
