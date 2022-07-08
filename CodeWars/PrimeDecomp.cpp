// ReSharper disable CppPossiblyErroneousEmptyStatements
#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <functional>
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

namespace PrimeDecomposition
{
#include <string>
  using namespace std;

  class PrimeDecomp
  {
  public:
    static std::string factors(int value)
    {
      auto result = string();

      for(auto factor = 2; factor <= value; factor++)
      {
        auto count = 0;
        for(; value % factor == 0; count++, value /= factor);
        if(count > 0)
        {
          result += "(";
          result += to_string(factor);
          if(count > 1)
            result += "**" + to_string(count);
          result += ")";
        }
      }

      return result;
    }
  };

  void testequal(std::string ans, std::string sol)
  {
    Assert::That(ans, Equals(sol));
  }

  Describe(factors_Tests)
  {
    It(Fixed_Tests)
    {
      testequal(PrimeDecomp::factors(7775460), "(2**2)(3**3)(5)(7)(11**2)(17)");
      testequal(PrimeDecomp::factors(7919), "(7919)");
    }
  };
}
