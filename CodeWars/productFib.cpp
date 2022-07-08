// ReSharper disable CppPossiblyErroneousEmptyStatements
#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <functional>
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

namespace DirectionReduction
{
#include <string>
#include <vector>

  using namespace std;
  typedef unsigned long long ull;

  class ProdFib
  {
  public:
    static std::vector<ull> productFib(ull prod)
    {
      if(prod == 0)return {0, 1, true};
      if(prod == 1)return {1, 1, true};

      ull last = 0, current = 1;

      while(last * current < prod)
      {
        const auto newLast = current;
        current += last;
        last = newLast;
      }

      return {last, current, last * current == prod};
    }
  };


  void testequal(std::vector<ull> ans, std::vector<ull> sol)
  {
    Assert::That(ans, Equals(sol));
  }

  void dotest(ull prod, std::vector<ull> expected)
  {
    testequal(ProdFib::productFib(prod), expected);
  }

  Describe(productFib_Tests)
  {
    It(Fixed__Tests)
    {
      dotest(4895, {55, 89, true});
      dotest(5895, {89, 144, false});
      dotest(74049690, {6765, 10946, 1ULL});
      dotest(84049690, {10946, 17711, 0ULL});
    }
  };
}
