#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;


enum SoundType {None, Clean, Distorted};

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

class Guitar
{
public:
    const Fuzzbox Default;
    const Fuzzbox* Effect = &Default;
    void AddEffect(const Fuzzbox& fuzzbox){Effect = &fuzzbox;};
    SoundType Sound(){return Effect->Sound;};
};

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
