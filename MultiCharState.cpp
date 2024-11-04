#include "RegexStates.hpp"

void MultiCharState::AddChar(char c)
{
	chars += c;
}

void MultiCharState::AddState(RegexState* state)
{
	MultiCharState *mstate = static_cast<MultiCharState*>(state);
	if (mstate)
		chars += mstate->chars;
}

bool equalfind(std::string chars, char c)
{
	return chars.find(c) != -1;
}

bool notequalfind(std::string chars, char c)
{
	return chars.find(c) == -1;
}

bool MultiCharState::Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches) const
{
	int cant = 0;
	bool (*f)(std::string, char) = negate ? &notequalfind : &equalfind;
	while (pos < input.size() && cant < max && f(chars, input[pos]))
	{
		pos++;
		cant++;
	}
	return cant >= min;
}