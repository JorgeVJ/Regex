#include "RegexStates.hpp"

GroupRState::GroupRState()
{
	conditions = new AndRState();
}

void GroupRState::AddChar(char c)
{
	conditions->AddChar(c);
}

void GroupRState::AddAction(void (*f)(std::string))
{
	OnValidate = f;
}

void GroupRState::AddQuantity(int min, int max)
{
	conditions->AddQuantity(min, max);
}

void GroupRState::AddState(RegexState* state)
{
	if (dynamic_cast<AndRState*>(conditions)
		&& dynamic_cast<OrRState*>(state))
	{
		state->AddState(conditions);
		state->AddState(new AndRState());
		conditions = state;
	}
	else if (dynamic_cast<OrRState*>(state))
		conditions->AddState(new AndRState());
	else
		conditions->AddState(state);
}

bool GroupRState::Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches) const
{
	int cant = 0;
	size_t start = pos;
	while (conditions->Matches(input, start, matches) && cant < max)
		cant++;
	if (cant >= min)
	{
		if (OnValidate)
			OnValidate(input.substr(pos, start - pos));
		//matches.push_back(input.substr(pos, start - pos));
		pos = start;
	}
	return cant >= min;
}