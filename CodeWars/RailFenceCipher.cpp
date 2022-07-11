// ReSharper disable CppPossiblyErroneousEmptyStatements
#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <functional>
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

namespace RailFenceCipher
{
#include <string>
  using namespace std;

  string Shuffle(const string& first, const string& other)
  {
    Assert::That(first.length() >= other.length());
    Assert::That(first.length() <= other.length() + 1);

    string result;
    for(auto index = 0u; index < first.length(); index++)
    {
      result += first[index];
      if(index < other.length())
        result += other[index];
    }
    return result;
  }

  std::pair<string, string> DeShuffle(const string& target)
  {
    string first;
    string other;
    for(auto index = 0u; index < target.length(); index++)
    {
      if(index % 2)other += target[index];
      else first += target[index];
    }

    return {first, other};
  }

  std::string encode_rail_fence_cipher(const std::string& str, size_t n)
  {
    const auto count = (n - 1) * 2;
    vector<string> results(count);
    for(auto index = 0u; index < str.length(); index++)
      results[index % count] += str[index];

    auto result = results[0];
    for(auto index = 1u; index < n - 1; index++)
      result += Shuffle(results[index], results[(2 * (n - 1) - index)]);
    result += results[n - 1];
    return result;
  }

  std::string decode_rail_fence_cipher(const std::string& str, size_t n)
  {
    const auto count = (n - 1) * 2;
    vector<size_t> lengthsPlus(count);
    for(auto index = 0u; index < str.length(); index++)
      lengthsPlus[index % count] ++;

    vector<string> parts(count);
    size_t start = 0;

    parts[0] = str.substr(start, lengthsPlus[0]);
    start += lengthsPlus[0];

    for(size_t index = 1; index < n - 1; index++)
    {
      const auto otherIndex = 2 * (n - 1) - index;
      const auto pair = DeShuffle(str.substr(start, lengthsPlus[index] + lengthsPlus[otherIndex]));
      parts[index] = pair.first;
      parts[otherIndex] = pair.second;
      start += lengthsPlus[index] + lengthsPlus[otherIndex];
    }
    parts[n - 1] = str.substr(start, lengthsPlus[n - 1]);
    start += lengthsPlus[n - 1];
    Assert::That(start, Equals(str.length()));

    auto result = str;

    for(size_t index = 0; index < parts.size(); index++)
      for(size_t position = 0; position < parts[index].length(); position++)
        result[index + position * parts.size()] = parts[index][position];
    return result;
  }

  Describe(Sample_Tests)
  {
    It(Tests)
    {
      Assert::That(encode_rail_fence_cipher("Hello, World!", 4), Equals("H !e,Wdloollr"));
      Assert::That(encode_rail_fence_cipher("ABC", 3), Equals("ABC"));
      Assert::That(encode_rail_fence_cipher("ABCD", 3), Equals("ABDC"));
      Assert::That(encode_rail_fence_cipher("ABCDE", 3), Equals("AEBDC"));
      Assert::That(encode_rail_fence_cipher("WEAREDISCOVEREDFLEEATONCE", 3), Equals("WECRLTEERDSOEEFEAOCAIVDEN"));
      Assert::That(encode_rail_fence_cipher("Hello, World!", 3), Equals("Hoo!el,Wrdl l"));
      Assert::That(encode_rail_fence_cipher("Hello, World!", 4), Equals("H !e,Wdloollr"));
      Assert::That(encode_rail_fence_cipher("", 3), Equals(""));

      Assert::That(decode_rail_fence_cipher("H !e,Wdloollr", 4), Equals("Hello, World!"));
      Assert::That(decode_rail_fence_cipher("WECRLTEERDSOEEFEAOCAIVDEN", 3), Equals("WEAREDISCOVEREDFLEEATONCE"));
      Assert::That(decode_rail_fence_cipher("", 3), Equals(""));
    }
  };
}
