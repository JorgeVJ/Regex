#include "RegexStates.hpp"

CharRState::CharRState(char c)
{
	character = c;
}

bool CharRState::Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches) const
{
	int cant = 0;
	while (pos < input.size() && cant < max && input[pos] == character)
	{
		pos++;
		cant++;
	}
	return cant >= min;
}