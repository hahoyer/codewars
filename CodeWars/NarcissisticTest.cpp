#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

#include <cmath>
using namespace std;

const auto DefaultBase = 10;

vector<int> GetDigits(int value, int base)
{
    auto result = vector<int>();
    for (; value > 0; value /= base)
        result.push_back(value % base);
    return result;
}

bool narcissistic(int value)
{
    auto digits = GetDigits(value, DefaultBase);
    auto result = 0;
    for (auto digit : digits)
        result += static_cast<int>(pow(digit, digits.size()));
    return result == value;
}


Describe(basic_tests)
{
    It(test1)
    {
        Assert::That(narcissistic(7), Equals(true));
    }

    It(test2)
    {
        Assert::That(narcissistic(371), Equals(true));
    }

    It(test3)
    {
        Assert::That(narcissistic(122), Equals(false));
    }

    It(test4)
    {
        Assert::That(narcissistic(4887), Equals(false));
    }
};
