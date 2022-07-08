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

  class DirReduction
  {
  public:
    static bool IsPair(string const& one, string const& other)
    {
      if(one == "NORTH") return other == "SOUTH";
      if(one == "WEST") return other == "EAST";
      if(one == "SOUTH") return other == "NORTH";
      if(one == "EAST") return other == "WEST";
      return false;
    }

    static std::vector<std::string> dirReduc(std::vector<std::string>& arr)
    {
      auto result = decltype(dirReduc(arr))();
      for(const auto& item : arr)
      {
        if(!result.empty() && IsPair(item, result.back()))
          result.pop_back();
        else
          result.push_back(item);
      }
      return result;
    }
  };


  void testequal(std::vector<std::string> ans, std::vector<std::string> sol)
  {
    Assert::That(ans, Equals(sol));
  }

  Describe(dirReduc_Tests)
  {
    It(Fixed_Tests)
    {
      std::vector<std::string> d1 = {"NORTH", "SOUTH", "SOUTH", "EAST", "WEST", "NORTH", "WEST"};
      std::vector<std::string> ans1 = DirReduction::dirReduc(d1);
      std::vector<std::string> sol1 = {"WEST"};
      testequal(ans1, sol1);

      std::vector<std::string> d2 = {"NORTH", "SOUTH", "SOUTH", "EAST", "WEST", "NORTH", "NORTH"};
      std::vector<std::string> ans2 = DirReduction::dirReduc(d2);
      std::vector<std::string> sol2 = {"NORTH"};
      testequal(ans2, sol2);
    }
  };
}
