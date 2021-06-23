#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;


#ifdef Comment

Write a function that takes in a string of one or more words,
and returns the same string, but with all five or more letter words reversed
(like the name of this kata).

Strings passed in will consist of only letters and spaces.
Spaces will be included only when more than one word is present.
Examples:

spinWords("Hey fellow warriors") => "Hey wollef sroirraw" 
spinWords("This is a test") => "This is a test" 
spinWords("This is another test") => "This is rehtona test"

#endif

using namespace std;

const auto MaxLetters = 4;

std::string Reverse(const std::string& target)
{
    auto result = std::string();
    for (auto character : target)
        result = character + result;
    return result;
}

vector<std::string> WordSplit(const string& target)
{
    auto result = vector<string>();
    auto start = 0;
    for (auto index = 0; index < target.size(); index++)
        if (target[index] == ' ')
        {
            result.push_back(target.substr(start, index - start));
            start = index + 1;
        }

    result.push_back(target.substr(start, target.size() - start));
    return result;
}

std::string spinWords(const std::string& target)
{
    auto result = std::string();
    auto delimiter = "";
    for (auto word : WordSplit(target))
    {
        result += delimiter;
        delimiter = " ";
        result += word.size() <= MaxLetters ? word : Reverse(word);
    }
    return result;
}


Describe(Sample_Tests)
{
    It(Single_Word)
    {
        Assert::That(spinWords("to"), Equals("to"));
        Assert::That(spinWords("Welcome"), Equals("emocleW"));
        Assert::That(spinWords("CodeWars"), Equals("sraWedoC"));
        Assert::That(spinWords("qwe"), Equals("qwe"));
        Assert::That(spinWords("qwer"), Equals("qwer"));
        Assert::That(spinWords("qwert"), Equals("trewq"));
    }

    It(Mulitple_Words)
    {
        Assert::That(spinWords("Hey fellow warriors"), Equals("Hey wollef sroirraw"));
        Assert::That(spinWords("Burgers are my favorite fruit"), Equals("sregruB are my etirovaf tiurf"));
        Assert::That(spinWords("Pizza is the best vegetable"), Equals("azziP is the best elbategev"));
    }
};
