#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <functional>
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

namespace DigitalRoot
{
  int querSumme(int n)
  {
    auto result = 0;
    for(; n > 0; n /= 10)
      result += n % 10;
    return result;
  }


  int digital_root(int n)
  {
    for(; n >= 10; n = querSumme(n));
    return n;
  }

  Describe(Fixed_tests)
  {
    It(Digital_root)
    {
      Assert::That(digital_root(16), Equals(7));
      Assert::That(digital_root(195), Equals(6));
      Assert::That(digital_root(992), Equals(2));
      Assert::That(digital_root(167346), Equals(9));
      Assert::That(digital_root(0), Equals(0));
    }
  };
}
