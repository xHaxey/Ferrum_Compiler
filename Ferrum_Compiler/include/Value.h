#pragma once
#include <string>
#include <variant>

struct Value
{
	std::variant<int, float, char, std::string, bool> m_data;

	Value(Value&) = default;
	Value(int i) : m_data(i) {}
	Value(float f) : m_data(f) {}
	Value(char c) : m_data(c) {}
	Value(std::string s) : m_data(s) {}
	Value(bool b) : m_data(b) {}
	Value(Value&&)	noexcept = default;
};