// ReSharper disable CppPossiblyErroneousEmptyStatements
#pragma warning( disable : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#include <functional>
#include <windows.h>
#define _CRT_SECURE_NO_WARNINGS

#include <igloo/igloo_alt.h>
#pragma warning( default : 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#pragma warning(disable : 4996)


using namespace igloo;

namespace SimplerInteractiveInterpreter
{
#include <cmath>
#include <string>
#include <map>
#include <memory>
#include <exception>

  std::map<std::string, double> Variables;

  class Token
  {
  public:
    enum TokenType { Unknown, Start, Number, Name, Sign, Product, Sum, Assign, LeftBracket, RightBracket, End };

    TokenType const type;

    int GetPriority() const
    {
      switch(type)
      {
      case Number:
      case Name:
      case Sign:
        return 100;
      case Assign:
        return 15;
      case Product:
        return 30;
      case Sum:
        return 20;
      case LeftBracket:
        return 140;
      case RightBracket:
        return 10;
      case End:
        return 0;
      default:
        throw std::runtime_error("unexpected token.");
      }
    };

    int getPriorityOnStack() const
    {
      switch(type)
      {
      case Start:
        return 1;
      case Number:
      case Name:
      case Sign:
        return 99;
      case Assign:
        return 14;
      case Product:
        return 31;
      case Sum:
        return 21;
      case LeftBracket:
        return 10;
      case RightBracket:
        return 120;
      default:
        throw std::runtime_error("unexpected token on stack.");
      }
    };
    std::string const text;

    Token(): type(Start) {}

    Token(TokenType type, std::string text)
      : type(type), text(std::move(text)) {};
  };

  class SimpleExpression
  {
    class ExpressionPart
    {
    public:
      std::shared_ptr<SimpleExpression> const left;
      Token token;

      ExpressionPart(std::shared_ptr<SimpleExpression> left, const Token& token)
        : left(std::move(left)), token(token) {}

      ExpressionPart() {}

      int GetPriority() const { return token.getPriorityOnStack(); };
    };

    std::shared_ptr<SimpleExpression> left;
    Token token;
    std::shared_ptr<SimpleExpression> right;

    static Token::TokenType ScanType(const std::string& target, size_t& position)
    {
      const auto first = target[position++];

      if(isdigit(first))
      {
        while(position < target.length() && isdigit(target[position]))
          position++;
        if(position < target.length() && target[position] == '.')
        {
          position++;
          while(position < target.length() && isdigit(target[position]))
            position++;
        }
        return Token::Number;
      }

      if(isalpha(first) || first == '_')
      {
        while(position < target.length() && (isalnum(target[position]) || target[position] == '_'))
          position++;
        return Token::Name;
      }

      switch(first)
      {
      case '+':
      case '-':
        return Token::Sum;
      case '*':
      case '/':
      case '%':
        return Token::Product;
      case '=':
        return Token::Assign;
      case '(':
        return Token::LeftBracket;
      case ')':
        return Token::RightBracket;
      default:
        return Token::Unknown;
      }
    }

    static Token GetNextToken(const std::string& target, size_t& position, bool& signOption)
    {
      SkipWhiteSpace(target, position);

      if(position >= target.length())
        return {Token::End, ""};

      auto startPosition = position;

      auto type = ScanType(target, position);

      if(signOption && type == Token::Sum)
        type = Token::Sign;

      switch(type)
      {
      case Token::Number:
      case Token::Name:
      case Token::RightBracket:
        signOption = false;
        break;
      case Token::Sign:
      case Token::Product:
      case Token::Sum:
      case Token::Assign:
      case Token::LeftBracket:
        signOption = true;
        break;
      }

      return {
        type,
        target.substr(startPosition, position - startPosition)
      };
    }

    static std::shared_ptr<SimpleExpression> Combine(std::stack<ExpressionPart>::value_type left,
                                                     std::shared_ptr<SimpleExpression> result)
    {
      return std::make_shared<SimpleExpression>(left.left, left.token, result);
    };

    static void SkipWhiteSpace(const std::string& target, size_t& position)
    {
      while(position < target.length() && std::isblank(target[position]))
        position++;
    }

  public:
    explicit SimpleExpression
    (const std::shared_ptr<SimpleExpression>& left
     , const Token& token
     , const std::shared_ptr<SimpleExpression>& right)
      : left(left), token(token), right(right) {}

    static std::shared_ptr<SimpleExpression> Create(const std::string& target)
    {
      std::stack<ExpressionPart> stack;
      stack.push({});

      size_t position = 0;
      auto signOption = true;

      do
      {
        std::shared_ptr<SimpleExpression> result = nullptr;
        auto token = GetNextToken(target, position, signOption);
        do
        {
          auto topItem = stack.top();
          const auto relation = topItem.GetPriority() - token.GetPriority();

          if(relation > 0)
            result = Combine(topItem, result);

          if(relation >= 0)
            stack.pop();

          if(stack.empty())
            return result->right;

          if(relation <= 0)
          {
            stack.push({result, token});
            result = nullptr;
          }
        }
        while(result);
      }
      while(true);
    }


    mutable double valueAtRuntime;

    static std::string Format(double target)
    {
      char buffer[2000];
      // ReSharper disable once CppDeprecatedEntity
      auto length = std::sprintf(buffer, "%.2f", target);

      return std::string(buffer, length);
    }

    std::string GetTrace() const
    {
      std::string result = "";
      if(left)
      {
        if(token.type == Token::Assign)
          result += left->token.text;
        else result += Format(left->valueAtRuntime);
      }
      result += token.text;
      if(right)
        result += Format(right->valueAtRuntime);

      result += " ==> ";
      result += Format(valueAtRuntime);
      return result;
    }

    static bool trace;

    double getValue() const
    {
      valueAtRuntime = getValue1();
      if(trace)std::cout << GetTrace() << "\n";
      return valueAtRuntime;
    }

    double getValue1() const
    {
      switch(token.type)
      {
      case Token::Number:
        return stod(token.text);
      case Token::Name:
        if(Variables.count(token.text))
          return Variables[token.text];
        throw std::runtime_error(std::string("unknown variable: \"") + token.text + "\".");

      case Token::Assign:
        if(left && left->token.type == Token::Name)
        {
          if(!right)
            throw std::runtime_error("missing right operand (=)");
          const auto value = right->getValue();
          Variables[left->token.text] = value;
          return value;
        }
        throw std::runtime_error("invalid target for assignment.");

      case Token::RightBracket:
        if(!left)
          throw std::runtime_error("empty brackets");
        return left->getValue();
      case Token::Sign:
        switch(token.text[0])
        {
        case '+':
          return right->getValue();
        case '-':
          return -right->getValue();
        }
        break;
      case Token::Product:
      case Token::Sum:
      {
        if(!left)
          throw std::runtime_error(std::string("missing left operand (") + token.text + ")");
        if(!right)
          throw std::runtime_error(std::string("missing right operand (") + token.text + ")");

        switch(token.text[0])
        {
        case '+':
          return left->getValue() + right->getValue();
        case '-':
          return left->getValue() - right->getValue();
        case '*':
          return left->getValue() * right->getValue();
        case '/':
          return left->getValue() / right->getValue();
        case '%':
          return std::fmod(left->getValue(), right->getValue());
        }
      }
      break;
      }
      throw std::runtime_error("unexpected token in expression.");
    };
  };

  bool SimpleExpression::trace = false;

  double interpret(std::string expression)
  {
    std::cout << "Assert::That(interpret(\"" + expression + "\")";
    if(auto result = SimpleExpression::Create(expression))
    {
      auto value = result->getValue();

      std::cout << ", EqualsWithDeltaConstraint(" << value << ", 0.0001));\n";
      return (value);
    }
    throw std::runtime_error("empty expression.");
  }

  Describe(Sample_Tests)
  {
    It(Tests)
    {
      SimpleExpression::trace = true;

      Assert::That(interpret("kyg + 58 * (-68 / 80 - -30) / -43"), Equals(0));

      Assert::That(interpret("kyg = 58 * (-68 / 80 - -30) / -43"), EqualsWithDeltaConstraint(-39.3186, 0.0001));
      Assert::That(interpret("xyhji = -36 % -(-58 / 90 * 73) / 54"), EqualsWithDeltaConstraint(-0.666667, 0.0001));
      Assert::That(interpret("uqhzt = 5 + -(-84 / 48 % 60) / 41"), EqualsWithDeltaConstraint(5.04268, 0.0001));
      Assert::That(interpret("xbvh = -10 + (66 % 74 - -82) / 10"), EqualsWithDeltaConstraint(4.8, 0.0001));
      Assert::That(interpret("azrhe = 10 + (83 % 63 - -50) * -42"), EqualsWithDeltaConstraint(-2930, 0.0001));
      Assert::That(interpret("-kyg / uqhzt + -xyhji - -azrhe * xbvh"), EqualsWithDeltaConstraint(-14055.5, 0.0001));

      Assert::That(interpret("43 * -81 - 71 / 2 % -97 - -67 + -45 + 66"), Equals(-3430.5));
      Assert::That(interpret("47 + -36"), Equals(11));
      Assert::That(interpret("47 + -36 / 6 + -10 % -32 + -66 + 5 % -80"), Equals(-30));
      Assert::That(interpret("-36"), Equals(-36));
      Assert::That(interpret(""), Equals(0));
      Assert::That(interpret("    "), Equals(0));

      Assert::That(interpret("x"), Equals(0));
      Assert::That(interpret("(1 + 2) * 3"), Equals(9));
      Assert::That(interpret("5 - 2 - 3"), Equals(0));
      Assert::That(interpret("5 + 2 - 3"), Equals(4));
      Assert::That(interpret("5 - 2 + 3"), Equals(6));
      Assert::That(interpret("1 + 2 + 3"), Equals(6));
      Assert::That(interpret("1 + 2 * 3"), Equals(7));
      Assert::That(interpret("x = (1 + 2 * 3)"), Equals(7));
      Assert::That(interpret("x"), Equals(7));
      Assert::That(interpret("x = 1 + 2 * 3"), Equals(7));
      Assert::That(interpret("x"), Equals(7));
      Assert::That(interpret("1 + 2 * 3"), Equals(7));
      Assert::That(interpret("1 + 1"), Equals(2));
      Assert::That(interpret("1 + 1"), Equals(2));
      Assert::That(interpret("1 + 1"), Equals(2));
      Assert::That(interpret("1 + 1"), Equals(2));

      Assert::That(interpret("1 + 1"), Equals(2));
      Assert::That(interpret("2 - 1"), Equals(1));
      Assert::That(interpret("2 * 3"), Equals(6));
      Assert::That(interpret("8 / 4"), Equals(2));
      Assert::That(interpret("7 % 4"), Equals(3));

      Assert::That(interpret("x = 1"), Equals(1));
      Assert::That(interpret("x"), Equals(1));
      Assert::That(interpret("x + 3"), Equals(4));

      try
      {
        interpret("y");
        Assert::That(0, Equals(1));
      }
      catch(...)
      {
        Assert::That(1, Equals(1));
      }
    }
  };
}
