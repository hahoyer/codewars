// ReSharper disable CppPossiblyErroneousEmptyStatements
#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <functional>
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

namespace BigNumberAdd
{
#include <string>

  using namespace std;

  int getDigit(const std::string& a, size_t index)
  {
    return index >= a.length() ? 0 : a[a.length() - index - 1] - '0';
  }

  std::string add(const std::string& a, const std::string& b)
  {
    auto result = string();
    auto carry = 0;
    for(size_t index = 0; index < a.length() || index < b.length(); index++)
    {
      const auto sum = getDigit(a, index) + getDigit(b, index) + carry;
      const auto digit = static_cast<char>(static_cast<int>('0') + sum % 10);
      result = digit + result;
      carry = sum / 10;
    }

    return (carry ? "1" : "") + result;
  }

  Describe(Sample_Tests)
  {
    It(Tests)
    {
      Assert::That(add("99", "2"), Equals("101"));
      Assert::That(add("123", "456"), Equals("579"));
      Assert::That(add("0", "0"), Equals("0"));
      Assert::That(add("10", "35679"), Equals("35689"));
      Assert::That(add("", "5"), Equals("5"));
      Assert::That(add("192", ""), Equals("192"));
      Assert::That(add("9999", "1111"), Equals("11110"));
    }
  };
}
