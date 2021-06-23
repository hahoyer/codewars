#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

using namespace std;

bool IsSmiley(const string& value)
{
    auto hasEyes = false;
    auto hasMouth = false;
    for(auto character : value)
        switch(character)
        {
        case ':':
        case ';': hasEyes = true;
            break;
        case ')':
        case 'D': hasMouth = true;
            break;
        case '-':
        case'~': break;
        default:
            return false;
        }

    return hasEyes && hasMouth;
}

int countSmileys(std::vector<std::string> target)
{
    auto result = 0;
    for(const auto& element : target)
        if(IsSmiley(element))
            result++;

    return result;
}

#if 0
Rules for a smiling face:

Each smiley face must contain a valid pair of eyes. Eyes can be marked as : or ;
A smiley face can have a nose but it does not have to. Valid characters for a nose are - or ~
Every smiling face must have a smiling mouth that should be marked with either ) or D
No additional characters are allowed except for those mentioned.


#endif


Describe(Tests)
{
    It(Single)
    {
        Assert::That(countSmileys({":D"}), Equals(1));
        Assert::That(countSmileys({":~)"}), Equals(1));
        Assert::That(countSmileys({";~)"}), Equals(1));
        Assert::That(countSmileys({":)"}), Equals(1));
        Assert::That(countSmileys({":)"}), Equals(1));
        Assert::That(countSmileys({":("}), Equals(0));
        Assert::That(countSmileys({":D"}), Equals(1));
        Assert::That(countSmileys({":O"}), Equals(0));
        Assert::That(countSmileys({":;"}), Equals(0));
        Assert::That(countSmileys({";]"}), Equals(0));
        Assert::That(countSmileys({":["}), Equals(0));
        Assert::That(countSmileys({";*"}), Equals(0));
        Assert::That(countSmileys({":$"}), Equals(0));
        Assert::That(countSmileys({";-D"}), Equals(1));
    }

    It(Basic_testing)
    {
        Assert::That(countSmileys({}), Equals(0));
        Assert::That(countSmileys({":D", ":~)", ";~)", ":)"}), Equals(4));
        Assert::That(countSmileys({":)", ":(", ":D", ":O", ":;"}), Equals(2));
        Assert::That(countSmileys({";]", ":[", ";*", ":$", ";-D"}), Equals(1));
    }
};
