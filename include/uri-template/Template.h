#pragma once

#include "Operator.h"
#include "Variable.h"

namespace URI {
namespace Template {

class Literal
{
public:
    static const std::unordered_set<char> kNotAllowedChars;

    Literal(std::string&& lit_string);

    Literal(const Literal&) = default;
    Literal& operator=(const Literal&) = default;
    Literal(Literal&&) noexcept = default;
    Literal& operator=(Literal&&) noexcept = default;

    ~Literal() = default;

    std::size_t Size() const;
    const std::string& String() const;

private:
    std::string lit_string_;
};

class Expression
{
public:
    Expression(std::shared_ptr<Operator>&& oper, std::vector<Variable>&& variables);

    Expression(const Expression&) = default;
    Expression& operator=(const Expression&) = default;
    Expression(Expression&&) noexcept = default;
    Expression& operator=(Expression&&) noexcept = default;

    ~Expression() = default;

    const Operator& Oper() const;
    const std::vector<Variable>& Vars() const;

private:
    std::shared_ptr<Operator> oper_;
    std::vector<Variable> var_list_;
};

enum class PartType
{
    LITERAL,
    EXPRESSION
};

class Part
{
public:
    template <class... Args>
    Part(Args&&... args)
        : part_(std::forward<Args>(args)...)
    {
        if (std::holds_alternative<Literal>(part_)) {
            type_ = PartType::LITERAL;
        } else {
            type_ = PartType::EXPRESSION;
        }
    }

    Part(Part&) = default;
    Part(const Part&) = default;
    Part& operator=(Part&) = default;
    Part& operator=(const Part&) = default;
    Part(Part&&) noexcept = default;
    Part& operator=(Part&&) noexcept = default;

    ~Part() = default;

    template <class T>
    T& Get()
    {
        return std::get<T>(part_);
    }

    template <class T>
    const T& Get() const
    {
        return std::get<T>(part_);
    }

    inline PartType Type() const
    {
        return type_;
    }

private:
    PartType type_;
    std::variant<Literal, Expression> part_;
};

class Template
{
public:
    Template() = default;

    Template(const Template&) = default;
    Template& operator=(const Template&) = default;
    Template(Template&&) noexcept = default;
    Template& operator=(Template&&) noexcept = default;

    ~Template() = default;

    template <class... Args>
    Part& EmplaceBack(Args&&... args)
    {
        return parts_.emplace_back(std::forward<Args>(args)...);
    }

    bool IsTemplated() const;
    std::size_t Size() const;
    std::vector<Part>& Parts();
    const std::vector<Part>& Parts() const;
    Part& operator[](std::size_t pos);
    const Part& operator[](std::size_t pos) const;

private:
    std::vector<Part> parts_;
};

} // namespace Template
} // namespace URI
