#include "Helper.h"

// ReSharper disable CppPossiblyErroneousEmptyStatements
#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <functional>
#include <igloo/igloo_alt.h>

#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data


using namespace igloo;

namespace BurrowsWheelerTransformation
{
#include <string>
#include <utility>

  using namespace std;

  std::string decode(const std::string& s, int n)
  {
    if(s.empty())
      return "";
    vector<string> rows;
    for(size_t i = 0; i < s.size(); ++i)
      rows.emplace_back();

    for(size_t j = 0; j < s.size(); ++j)
    {
      for(size_t i = 0; i < s.size(); ++i)
        rows[i] = s[i] + rows[i];
      sort(rows.begin(), rows.end());
    }

    return rows[n];
  }

  std::pair<std::string, int> encode(const std::string& s)
  {
    //OutputDebug("----------------encode:\n");
    vector<string> matrix;
    string target = s;
    for(size_t index = 0; index < s.length(); index++)
    {
      matrix.push_back(target);
      target = target.substr(1) + target[0];
    }
    sort(matrix.begin(), matrix.end());

    size_t resultIndex = 0;
    string resultString;
    for(size_t index = 0; index < s.length(); index++)
    {
      auto row = matrix[index];
      //OutputDebug(to_string(index) + " " + row);
      resultString += row.back();
      if(row == s)
      {
        resultIndex = index;
        //  OutputDebug(" <=");
      }
      //OutputDebug("\n");
    }

    //OutputDebug("result: {" + to_string(resultIndex) + " " + resultString + "}\n");
    //OutputDebug("----------------encode ends\n");
    return {resultString, static_cast<int>(resultIndex)};
  }

  void encodeDecode(const std::string& s)
  {
    const auto p = encode(s);
    const auto ss = decode(p.first, p.second);
    Assert::That(ss, Equals(s));
  }

  using psi = std::pair<std::string, int>;

  Describe(Sample_Tests)
  {
    It(Tests)
    {
      encodeDecode("");
      encodeDecode("aaba");
      encodeDecode("a");

      encodeDecode("aa");
      encodeDecode("ab");
      encodeDecode("ba");

      encodeDecode("aaa");
      encodeDecode("aab");
      encodeDecode("aba");
      encodeDecode("baa");
      encodeDecode("abb");
      encodeDecode("bab");
      encodeDecode("bba");
      encodeDecode("abc");
      encodeDecode("acb");
      encodeDecode("bac");
      encodeDecode("bca");
      encodeDecode("cab");
      encodeDecode("cba");

      encodeDecode("aaaa");
      encodeDecode("aaba");
      encodeDecode("abaa");
      encodeDecode("baaa");
      encodeDecode("abba");
      encodeDecode("baba");
      encodeDecode("bbaa");
      encodeDecode("abca");
      encodeDecode("acba");
      encodeDecode("baca");
      encodeDecode("bcaa");
      encodeDecode("caba");
      encodeDecode("cbaa");

      Assert::That(encode("bananabar"), Equals(psi{"nnbbraaaa", 4}));
      Assert::That(encode("banananar"), Equals(psi{"bnnnraaaa", 4}));
      Assert::That(encode("Humble Bundle"), Equals(psi{"e emnllbduuHB", 2}));
      Assert::That(encode("Mellow Yellow"), Equals(psi{"ww MYeelllloo", 1}));

      Assert::That(decode("nnbbraaaa", 4), Equals("bananabar"));
      Assert::That(decode("nnbbraaaa", 4), Equals("bananabar"));
      Assert::That(decode("e emnllbduuHB", 2), Equals("Humble Bundle"));
      Assert::That(decode("ww MYeelllloo", 1), Equals("Mellow Yellow"));
    }
  };
}
