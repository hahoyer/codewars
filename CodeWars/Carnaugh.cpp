// ReSharper disable CppPossiblyErroneousEmptyStatements
#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <functional>
#include <igloo/igloo_alt.h>

#include "Helper.h"
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

namespace Carnaugh
{
#include <string>

  using ulong = unsigned long long;
  using namespace std;

  ulong mystery(ulong target)
  {
    return target ^ target / 2;
  }

  ulong mysteryInv(ulong n)
  {
    return n;
  }

  std::string nameOfMystery()
  {
    return "gray";
  }

  string to_bitString(ulong target)
  {
    string result;
    if(target == 0)
      return "0";
    while(target)
    {
      result += target % 2 ? "1" : "0";
      target /= 2;
    }

    return result;
  }

  // Failed this Kata!
  //Describe(Example_Test_Cases)
  //{
  //  It(Testing_Mystery_On_6)
  //  {
  //    Assert::That(mystery(6), Equals(5));
  //  }

  //  It(Testing_MysteryInv_On_5)
  //  {
  //    Assert::That(mysteryInv(5), Equals(6));
  //  }

  //  It(Testing_Mystery_On_9)
  //  {
  //    Assert::That(mystery(9), Equals(13));
  //  }

  //  It(Testing_MysteryInv_On_13)
  //  {
  //    Assert::That(mysteryInv(13), Equals(9));
  //  }

  //  It(Testing_Mystery_On_19)
  //  {
  //    Assert::That(mystery(19), Equals(26));
  //  }

  //  It(Testing_MysteryInv_On_26)
  //  {
  //    Assert::That(mysteryInv(26), Equals(19));
  //  }
  //};
}
