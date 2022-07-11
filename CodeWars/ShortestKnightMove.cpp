// ReSharper disable CppPossiblyErroneousEmptyStatements
#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <functional>
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

namespace ShortestKnightMove
{
#include <string>
  using namespace std;

  constexpr size_t boardSize = 8;
  vector<vector<int>> distanceCache;
  const vector<pair<int, int>> moves = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

  bool IsValid(int x, int y)
  {
    return x >= 0 && x < static_cast<int>(boardSize) && y >= 0 && y < static_cast<int>(boardSize);
  }

  void InitialiseCache()
  {
    distanceCache = vector<vector<int>>(boardSize);
    for(auto index = 0u; index < boardSize; index ++)
      distanceCache[index] = vector<int>(boardSize);
  }

  int GetDistance(int x, int y, int distance, int xTarget, int yTarget)
  {
    if(!IsValid(x, y))
      return boardSize * boardSize;

    vector<int>::reference tested = distanceCache[x][y];
    if(tested && tested <= distance)
      return boardSize * boardSize;

    tested = distance;

    if(x == xTarget && y == yTarget)
      return distance;

    int result = boardSize * boardSize;
    for(const auto& [dx, dy] : moves)
    {
      const auto stepResult = GetDistance(x + dx, y + dy, distance + 1, xTarget, yTarget);
      if(result > stepResult)
        result = stepResult;
    }
    Assert::That(result > 0);
    return result;
  }


  size_t knight(std::string start, std::string finish)
  {
    InitialiseCache();
    return GetDistance(start[0] - 'a', start[1] - '1', 0, finish[0] - 'a', finish[1] - '1');
  }

  Describe(Sample_Tests)
  {
    It(Tests)
    {
      Assert::That(knight("a1", "c1"), Equals(2));
      Assert::That(knight("a1", "f1"), Equals(3));
      Assert::That(knight("a1", "f3"), Equals(3));
      Assert::That(knight("a1", "f4"), Equals(4));
      Assert::That(knight("a1", "f7"), Equals(5));
    }
  };
}
