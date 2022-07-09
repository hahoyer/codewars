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

vector<vector<size_t>> GetPermutations(size_t count)
{
  if(count == 0)
    return {{}};

  vector<vector<size_t>> result;
  const auto permutations = GetPermutations(count - 1);
  for(const auto& permutation : permutations)
    for(auto index = 0u; index < count; index++)
    {
      auto subResult = permutation;
      subResult.insert(subResult.begin() + index, count - 1);
      result.push_back(subResult);
    }

  return result;
}

long long GetSign(vector<size_t> const& permutation)
{
  long long result = 1;
  for(auto index = 1u; index < permutation.size(); index++)
    for(auto otherIndex = 0u; otherIndex < index; otherIndex++)
      if(permutation[otherIndex] > permutation[index])
        result *= -1;
  return result;
}

long long GetProduct(const vector<vector<long long>>& value, vector<size_t> const& permutation)
{
  long long result = GetSign(permutation);
  for(auto index = 0u; index < permutation.size(); index++)
    result *= value[index][permutation[index]];
  return result;
}

long long determinant(vector<vector<long long>> const& value)
{
  const auto count = value.size();
  for(const auto& line : value)
    Assert::That(count, Equals(line.size()));

  long long result = 0;
  int sign = 1;
  const auto permutations = GetPermutations(count);
  for(const auto& permutation : permutations)
  {
    const auto product = GetProduct(value, permutation);
    result += product;
    sign *= -1;
  }

  return result;
}

Describe(your_determinant_function)
{
  It(should_work_for_a_few_simple_square_matrices)
  {
    Assert::That(determinant(vector<vector<long long>>{
                   vector<long long>{2, 5, 3},
                   vector<long long>{1, -2, -1},
                   vector<long long>{1, 3, 4}
                 }), Equals(-20));

    Assert::That(determinant(vector<vector<long long>>{
                   vector<long long>{1}
                 }), Equals(1));
    Assert::That(determinant(vector<vector<long long>>{
                   vector<long long>{1, 3},
                   vector<long long>{2, 5}
                 }), Equals(-1));
    Assert::That(determinant(vector<vector<long long>>{
                   vector<long long>{2, 5, 3},
                   vector<long long>{1, -2, -1},
                   vector<long long>{1, 3, 4}
                 }), Equals(-20));
  }
};
