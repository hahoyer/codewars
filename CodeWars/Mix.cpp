// ReSharper disable CppPossiblyErroneousEmptyStatements
#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <functional>
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

#include <string>
#include <iostream>
#include <vector>

using namespace std;


vector<int> GetCounts(const string& s1)
{
  auto result = vector<int>('z' - 'a' + 1);
  for(const auto position : s1)
    if(position >= 'a' && position <= 'z')
      result[position - 'a']++;
  return result;
}

vector<size_t> GetOrderedElements(const vector<int>& first, const vector<int>& other)
{
  auto heads = vector<char>('z' - 'a' + 1);
  auto maximalCounts = vector<int>('z' - 'a' + 1);
  for(auto index = 0u; index < maximalCounts.size(); index++)
  {
    heads[index] = first[index] > other[index] ? '1' : first[index] < other[index] ? '2' : '=';
    maximalCounts[index] = max(first[index], other[index]);
  }

  auto result = vector<size_t>('z' - 'a' + 1);
  for(auto index = 0u; index < result.size(); ++index)
    result[index] = index;
  sort(result.begin(), result.end(), [&](size_t firstIndex, size_t otherIndex)
  {
    const auto countDifference = maximalCounts[firstIndex] - maximalCounts[otherIndex];
    if(countDifference != 0)
      return countDifference > 0;
    if(heads[firstIndex] != heads[otherIndex])
      return heads[firstIndex] < heads[otherIndex];
    return firstIndex < otherIndex;
  });

  auto index = 0u;
  for(; index < result.size() && maximalCounts[result[index]] > 1; ++index);
  result.erase(result.begin() + index, result.end());
  return result;
}

string GetCode(char character, int countS1, int countS2)
{
  string head = "=";
  auto counts = countS1;
  if(countS1 > countS2)
    head = "1";
  else if(countS1 < countS2)
  {
    head = "2";
    counts = countS2;
  }
  auto characters = string();
  while(counts-- > 0)
    characters += character;

  return head + ":" + characters;
}

class Mix
{
public:
  static std::string mix(const std::string& s1, const std::string& s2)
  {
    const auto countsS1 = GetCounts(s1);
    const auto countsS2 = GetCounts(s2);
    const auto orderedElements = GetOrderedElements(countsS1, countsS2);

    auto result = string();
    string delimiter;
    for(const auto element : orderedElements)
    {
      result += delimiter + GetCode(static_cast<char>('a' + element), countsS1[element], countsS2[element]);
      delimiter = "/";
    }

    return result;
  }
};


void testequal(std::string ans, std::string sol)
{
  Assert::That(ans, Equals(sol));
}

void dotest(const std::string& s1, const std::string& s2, std::string expected)
{
  testequal(Mix::mix(s1, s2), expected);
}

Describe(mix_Tests)
{
  It(Fixed_Tests)
  {
    std::string sol = "2:eeeee/2:yy/=:hh/=:rr";
    dotest("Are they here", "yes, they are here", sol);
    sol = "1:ooo/1:uuu/2:sss/=:nnn/1:ii/2:aa/2:dd/2:ee/=:gg";
    dotest("looping is fun but dangerous", "less dangerous than coding", sol);
  }
};
