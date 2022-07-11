// ReSharper disable CppPossiblyErroneousEmptyStatements
#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

namespace TheBooleanOrder
{
#include <string>
#include <vector>

  using namespace std;

  vector<int64_t> countsCache;

  int64_t GetCombinationCounts(size_t operationCount)
  {
    if(operationCount < countsCache.size())
      return countsCache[operationCount];
    int64_t result = 0;
    if(operationCount == 0)
      result = 1;
    for(size_t index = 0; index < operationCount; ++index)
      result += GetCombinationCounts(index) * GetCombinationCounts(operationCount - index - 1);
    countsCache.push_back(result);
    return result;
  }

  int64_t Calculate(int64_t first, int64_t countFirst, char operation,
                    int64_t other, int64_t countOther)
  {
    switch(operation)
    {
    case '&':
      return first * other;
    case '|':
      return countFirst * other + first * countOther - first * other;
    case '^':
      return countFirst * other + first * countOther - 2 * first * other;
    default:
      return {};
    }
  };

  int64_t Calculate(const std::string& constants, const std::string& operations, size_t start, size_t end);

  int64_t CalculateStep(const std::string& constants, const std::string& operations, size_t start, size_t index,
                        size_t end)
  {
    const auto operation = operations[index];
    const auto countFirst = GetCombinationCounts(index - start);
    const auto countOther = GetCombinationCounts(end - index - 1);
    const auto firstResults = Calculate(constants, operations, start, index);
    const auto otherResults = Calculate(constants, operations, index + 1, end);
    return Calculate(firstResults, countFirst, operation, otherResults, countOther);
  }

  int64_t CalculateStepSmart(const std::string& constants, const std::string& operations, size_t start, size_t index,
                             size_t end)
  {
    const auto operation = operations[index];
    const auto countFirst = GetCombinationCounts(index - start);
    const auto countOther = GetCombinationCounts(end - index - 1);

    if(countFirst < countOther)
    {
      const auto firstResults = Calculate(constants, operations, start, index);
      switch(operation)
      {
      case '&':
        if(firstResults == 0)
          return 0;
        break;
      case '|':
        if(firstResults == countFirst)
          return countFirst * countOther;
        break;
      default: ;
      }

      const auto otherResults = Calculate(constants, operations, index + 1, end);
      return Calculate(firstResults, countFirst, operation, otherResults, countOther);
    }
    const auto otherResults = Calculate(constants, operations, index + 1, end);
    switch(operation)
    {
    case '&':
      if(otherResults == 0)
        return 0;
      break;
    case '|':
      if(otherResults == countOther)
        return countFirst * countOther;
      break;
    default: ;
    }

    const auto firstResults = Calculate(constants, operations, start, index);
    return Calculate(firstResults, countFirst, operation, otherResults, countOther);
  }

  int64_t Calculate(const std::string& constants, const std::string& operations, size_t start,
                    size_t end)
  {
    if(start == end)
      return constants[start] == 'f' ? 0 : 1;

    int64_t results = 0;
    for(auto index = start; index < end; index++)
    {
      const auto calculateStep = CalculateStepSmart(constants, operations, start, index, end);
      //Assert::That(calculateStep, Equals(CalculateStep(constants, operations, start, index, end)));
      results += calculateStep;
    }
    return results;
  }

  int64_t solve(const std::string& constants, const std::string& operations)
  {
    auto result = Calculate(constants, operations, 0, operations.length());
    return result;
  }

  // TODO: Replace examples and use TDD development by writing your own tests

  Describe(sample_tests)
  {
    It(basic_tests)
    {
      Assert::That(solve("ttftff", "|&^&&"), Equals(16));
      Assert::That(solve("tft", "^&"), Equals(2));
      Assert::That(solve("ttftff", "|&^&&"), Equals(16));
      Assert::That(solve("ttftfftf", "|&^&&||"), Equals(339));
      Assert::That(solve("ttftfftft", "|&^&&||^"), Equals(851));
      Assert::That(solve("ttftfftftf", "|&^&&||^&"), Equals(2434));
    }
  };
}
