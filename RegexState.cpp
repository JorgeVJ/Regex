#include "RegexStates.hpp"

RegexState::RegexState()
{
	this->min = 1;
	this->max = 1;
}

RegexState::~RegexState() { }

void RegexState::AddChar(char c) { }

void RegexState::AddAction(void(*f)(std::string)) { }

void RegexState::AddState(RegexState* c) { }

void RegexState::AddQuantity(int min, int max)
{
	this->min = min;
	this->max = max;
}

void RegexState::SetQuantity(int min, int max)
{
	this->min = min;
	this->max = max;
}

bool RegexState::Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches) const
{
	return false;
}