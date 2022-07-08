// ReSharper disable CppPossiblyErroneousEmptyStatements
#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <functional>
#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data

using namespace igloo;

#include <windows.h>


namespace HummingNumber
{
#include <cstdint>
#include <vector>
  using namespace std;

  static void OutputDebug(const string& text)
  {
    if(IsDebuggerPresent())
      OutputDebugStringA(text.c_str());
    else
      printf("%s", text.c_str());
  }


  vector<uint64_t> cache = {1};

  bool IsHammingNumber(uint64_t target)
  {
    auto value = target;
    while(value % 2 == 0)value /= 2;
    while(value % 3 == 0)value /= 3;
    while(value % 5 == 0)value /= 5;
    return value == 1;
  }

  void EnsureCache(uint64_t n)
  {
    uint64_t result = cache.back();
    for(auto index = cache.size(); index <= n; index++)
    {
      do
        result++;
      while(!IsHammingNumber(result));
      cache.push_back(result);
    }
  }

  uint64_t hamberSlow(int n)
  {
    EnsureCache(n);
    return cache[n];
  }

  class Triple
  {
  public:
    uint64_t value = 1;
    int exponent2 = 0;
    int exponent3 = 0;
    int exponent5 = 0;

    static bool less(Triple const& one, Triple const& other)
    {
      return one.value < other.value;
    }

    Triple Times(u_int divider) const
    {
      auto result = *this;
      if(divider == 2)
        result.exponent2++;
      else if(divider == 3)
        result.exponent3++;
      else if(divider == 5)
        result.exponent5++;
      else throw exception();
      result.value *= divider;
      // overflow check
      if(result.value / divider != value)
        result.value = 0;

      return result;
    }
  };

  void push_back(vector<Triple>& results, Triple const& node)
  {
    if(node.value == 0)
      return;
    size_t index = 0;
    while(index < results.size() && results[index].value < node.value)
      index++;
    results.insert(results.begin() + static_cast<int>(index), node);
  };

  uint64_t hamber1(int n)
  {
    uint64_t result = 1;
    vector<Triple> results = {Triple()};

    for(auto index = 1; index < n; index++)
    {
      auto node = results.front();
      if(node.value > result)
        throw exception();
      while(!results.empty() && results.front().value == result)
        results.erase(results.begin());

      push_back(results, node.Times(2));
      push_back(results, node.Times(3));
      push_back(results, node.Times(5));

      //sort(results.begin(), results.end(), Triple::less);
      result = results.front().value;

      //OutputDebug("index=" + to_string(index) + " result=" + to_string(result) + " results=" + to_string(results.size()) + "\n");
    }

    return result;
  }

  uint64_t hamber(int n)
  {
    std::vector<uint64_t> h = {1};
    uint64_t x2 = 2, x3 = 3, x5 = 5;
    size_t i = 0, j = 0, k = 0;

    while(h.size() < n)
    {
      h.push_back(min(x2, min(x3, x5)));

      if(x2 == h.back())
        x2 = 2 * h[++i];
      if(x3 == h.back())
        x3 = 3 * h[++j];
      if(x5 == h.back())
        x5 = 5 * h[++k];
    }

    return h[n - 1];
  }

  Describe(Sample_Test)
  {
    It(should_return_the_nth_hamber)
    {
      Assert::That(hamber(12689), Equals(9183300480000000000));
      Assert::That(hamber(7), Equals(8));
      Assert::That(hamber(3), Equals(3));
      Assert::That(hamber(1), Equals(1));
      Assert::That(hamber(2), Equals(2));
      Assert::That(hamber(4), Equals(4));
      Assert::That(hamber(5), Equals(5));
      Assert::That(hamber(6), Equals(6));
      Assert::That(hamber(8), Equals(9));
      Assert::That(hamber(11), Equals(15));
      Assert::That(hamber(14), Equals(20));
      Assert::That(hamber(17), Equals(27));
      Assert::That(hamber(20), Equals(36));
      Assert::That(hamber(23), Equals(48));
      Assert::That(hamber(26), Equals(60));
      Assert::That(hamber(29), Equals(75));
      Assert::That(hamber(32), Equals(90));
      Assert::That(hamber(35), Equals(108));
      Assert::That(hamber(38), Equals(128));
      Assert::That(hamber(41), Equals(150));
      Assert::That(hamber(44), Equals(180));
      Assert::That(hamber(47), Equals(216));
      Assert::That(hamber(50), Equals(243));
      Assert::That(hamber(53), Equals(270));
      Assert::That(hamber(56), Equals(320));
      Assert::That(hamber(59), Equals(375));
      Assert::That(hamber(62), Equals(405));
    }
  };
}

/*
0 0 0 1

1 0 0 2
0 1 0 3
0 0 1 5



1 0 0 2
2 0 0 4
0 1 0 
3 4 

*/
