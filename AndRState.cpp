#include "RegexStates.hpp"

bool AndRState::Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches) const
{
	size_t start = pos;
	for (auto state = states.begin(); state != states.end(); ++state)
	{
		bool result = (*state)->Matches(input, start, matches);
		if (!result)
			return false;
	}
	if(print)
		matches.push_back(input.substr(pos, start - pos));
	pos = start;
	return true;
}

void AndRState::AddState(RegexState* state)
{
	states.push_back(state);
}

void AndRState::AddAction(void(*f)(std::string))
{
	states.back()->AddAction(f);
}

void AndRState::AddQuantity(int min, int max)
{
	states.back()->SetQuantity(min, max);
}

void AndRState::SetQuantity(int min, int max)
{
	states.back()->SetQuantity(min, max);
}

AndRState::AndRState()
{
	this->print = false;
}

AndRState::AndRState(AndRState& other)
{
	this->print = other.print;
	this->states = std::vector<RegexState*>(other.states);
}

AndRState::~AndRState()
{
	for (RegexState* state : states)
		delete state;
	states.clear();
}

void AndRState::AddChar(char c)
{
	if (!states.empty()
		&& dynamic_cast<OrRState*>(states.back()))
		states.back()->AddChar(c);
	else
		states.push_back(new CharRState(c));
}