#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

using namespace std;


class TribonacciCalculator final
{
    const vector<int> Signature;
public :
    vector<int> Result;

    TribonacciCalculator(const vector<int>& signature)
        : Signature(signature)
    {
    }

    TribonacciCalculator& Ensure(const int count)
    {
        while (Result.size() < count)
            AddValue();
        return *this;
    }

private:
    void AddValue()
    {
        Result.push_back(GetNewValue());
    }

    int GetLowerValue() const
    {
        return Signature[Result.size()];
    }

    int GetHigherValue() const
    {
        const auto offset = Result.size() - Signature.size();
        auto result = 0;
        for (size_t index = 0; index < Signature.size(); index++)
            result += Result[offset + index];
        return result;
    }

    int GetNewValue() const
    {
        return Result.size() < Signature.size() ? GetLowerValue() : GetHigherValue();
    }
};

vector<int> tribonacci(std::vector<int> signature, int count)
{
    return TribonacciCalculator(signature).Ensure(count).Result;
}

Describe(Basic_tests)
{
    It(Test_1)
    {
        std::vector<int> signature = {1, 1, 1};
        std::vector<int> expected = {1, 1, 1, 3, 5, 9, 17, 31, 57, 105};
        Assert::That(tribonacci(signature, 10), Equals(expected));
    }

    It(Test_2)
    {
        std::vector<int> signature = {0, 0, 1};
        std::vector<int> expected = {0, 0, 1, 1, 2, 4, 7, 13, 24, 44};
        Assert::That(tribonacci(signature, 10), Equals(expected));
    }

    It(Test_3)
    {
        std::vector<int> signature = {0, 1, 1};
        std::vector<int> expected = {0, 1, 1, 2, 4, 7, 13, 24, 44, 81};
        Assert::That(tribonacci(signature, 10), Equals(expected));
    }

    It(Test_4)
    {
        std::vector<int> signature = {1, 0, 0};
        std::vector<int> expected = {1, 0, 0, 1, 1, 2, 4, 7, 13, 24};
        Assert::That(tribonacci(signature, 10), Equals(expected));
    }

    It(Test_5)
    {
        std::vector<int> signature = {1, 2, 3};
        std::vector<int> expected = {1, 2, 3, 6, 11, 20, 37, 68, 125, 230};
        Assert::That(tribonacci(signature, 10), Equals(expected));
    }

    It(Test_6)
    {
        std::vector<int> signature = {1, 2, 3};
        std::vector<int> expected = {1, 2};
        Assert::That(tribonacci(signature, 2), Equals(expected));
    }

    It(Test_7)
    {
        int third = rand() % 10;
        std::vector<int> signature = {1, 2, third};
        std::vector<int> expected = {};
        Assert::That(tribonacci(signature, 0), Equals(expected));
    }
};
