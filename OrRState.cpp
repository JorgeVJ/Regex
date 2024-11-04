#include "RegexStates.hpp"

void OrRState::AddChar(char c)
{
	if (dynamic_cast<GroupRState*>(states.back()))
		states.push_back(new AndRState());
	states.back()->AddChar(c);
}

void OrRState::AddState(RegexState* state)
{
	if (dynamic_cast<AndRState*>(state))
		states.push_back(state);
	else
		states.back()->AddState(state);
}

void OrRState::AddAction(void(*f)(std::string))
{
	states.back()->AddAction(f);
}

void OrRState::AddQuantity(int min, int max)
{
	states.back()->AddQuantity(min, max);
}

void OrRState::SetQuantity(int min, int max)
{
	states.back()->AddQuantity(min, max);
}

bool OrRState::Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches) const
{
	size_t start = pos;
	for (auto state = states.begin(); state != states.end(); ++state)
	{
		bool result = (*state)->Matches(input, start, matches);
		if (result)
		{
			if (print)
				matches.push_back(input.substr(pos, start - pos));
			pos = start;
			return true;
		}
	}
	return false;
}
