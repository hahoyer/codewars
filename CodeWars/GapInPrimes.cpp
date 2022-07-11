#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <functional>
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

#include <utility>

class GapInPrimes
{
public:
    static std::pair<long long, long long> gap(int g, long long m, long long n) {
        return std::pair {0, 0};
  }
};

                                                        
void testequal(std::pair <long long, long long> ans, std::pair <long long, long long> sol) {
  snowhouse::Assert::That(ans, snowhouse::Equals(sol));
}
void dotest(int g, long long m, long long n, std::pair <long long, long long> expected)
{
    testequal(GapInPrimes::gap(g, m, n), expected);
}

Describe(gap_Tests)
{
    It(Fixed_Tests)
    {
      dotest(2,100,110, {101, 103});
      dotest(4,100,110, {103, 107});
      dotest(6,100,110, {0, 0});
      dotest(8,300,400, {359, 367});
      dotest(10,300,400, {337, 347});
    }
};
