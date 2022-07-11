#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <functional>
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

#include <utility>

class GapInPrimes
{
  inline static std::vector<int> wheelIncrements = {4, 2, 4, 2, 4, 6, 2, 6};

  static bool IsPrime(long long value)
  {
    if(value % 2 == 0) return value == 2;
    if(value % 3 == 0) return value == 3;
    if(value % 5 == 0) return value == 5;

    long long candidate = 7;
    auto wheelIndex = 0;
    while(candidate * candidate <= value)
    {
      if(value % candidate == 0) return value == candidate;
      candidate += wheelIncrements[wheelIndex];
      wheelIndex = (wheelIndex + 1) % 8;
    }
    return true;
  }

  static bool IsPairOfPrimes(long long value, long long other)
  {
    if(!IsPrime(value) || !IsPrime(other))
      return false;

    for(auto gapValue = value + 1; gapValue < other; gapValue++)
      if(IsPrime(gapValue))
        return false;

    return true;
  }

public:
  static std::pair<long long, long long> gap(int gapLength, long long start, long long end)
  {
    for(auto target = start; target <= end; target++)
      if(IsPairOfPrimes(target, target + gapLength))
        return std::pair{target, target + gapLength};
    return std::pair{0, 0};
  }
};


void testequal(std::pair<long long, long long> ans, std::pair<long long, long long> sol)
{
  Assert::That(ans, Equals(sol));
}

void dotest(int g, long long m, long long n, std::pair<long long, long long> expected)
{
  testequal(GapInPrimes::gap(g, m, n), expected);
}

Describe(gap_Tests)
{
  It(Fixed_Tests)
  {
    dotest(2, 100, 110, {101, 103});
    dotest(4, 100, 110, {103, 107});
    dotest(6, 100, 110, {0, 0});
    dotest(8, 300, 400, {359, 367});
    dotest(10, 300, 400, {337, 347});
  }
};
