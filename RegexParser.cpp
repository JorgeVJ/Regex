# include "RegexParser.hpp"
# include <fstream>

enum s_states
{
	EMPTY,
	ERROR,
	CHAR,
	MCHARS,
	MCHARE,
	ORS,
	STAR,
	GROUPS,
	GROUPE,
	BACKSLASH,
	ESCAPEDCHAR,
	PLUS,
	ACTION,
	ACTINDEX,
};

void RegexParser::innit()
{
	const std::string strings[] = { "[", "]", ",", "|", "(", ")", "~", "0123456789", "*", "+", "\\", "^", " \t\n" };
	const size_t size = sizeof(strings) / sizeof(std::string);

	for (int i = 0; i < 20; i++)
		fsa[i] = NULL;
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			fta[i][j] = NULL;

	state = 0;
	ostate = 0;
	fsa[CHAR] = &RegexParser::character;
	fsa[ORS] = &RegexParser::orfound;
	fsa[MCHARS] = &RegexParser::setstart;
	fsa[MCHARE] = &RegexParser::setend;
	fsa[STAR] = &RegexParser::star;
	fsa[PLUS] = &RegexParser::plus;
	fsa[GROUPS] = &RegexParser::groupstart;
	fsa[GROUPE] = &RegexParser::groupending;
	fta[BACKSLASH][ESCAPEDCHAR] = &RegexParser::backslash;
	fta[MCHARS][ESCAPEDCHAR] = &RegexParser::negate;
	fta[GROUPE][ACTION] = &RegexParser::move;

	for (int i = 0; i < 14; i++)
		fta[ACTION][i] = &RegexParser::action;

	set_alphabet(size, strings);
	AndRState* ands = new AndRState();
	ands->print = true;
	regex = ands;
	groupend = false;

	std::cout << "Automata Inizialized" << std::endl;
}

int getstate(int i, int j)
{
	int state[][14] = {
		//   [   ]   ,   |   (   )   ~  \d   *   +   \  \^   \s  ^
			{3,  4,  2,  5,  7,  8,  2,  2,  6, 11,  9,  2,  2,  2}, // empty			0
			{1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1}, // error			1
			{3,  4,  2,  5,  7,  8,  2,  2,  6, 11,  9,  2,  2,  2}, // character		2
			{3,  4,  2,  5,  7,  8,  2,  2,  6, 11,  9, 10,  2,  2}, // mCharsStart		3
			{3,  4,  2,  5,  7,  8,  2,  2,  6, 11,  9,  2,  2,  2}, // mCharsEnd		4
			{3,  4,  2,  5,  7,  8,  2,  2,  6, 11,  9,  2,  2,  2}, // Or				5
			{3,  4,  2,  5,  7,  8,  2,  2,  6, 11,  9,  2,  2,  2}, // Star			6
			{3,  4,  2,  5,  7,  8,  2,  2,  6, 11,  9,  2,  2,  2}, // GroupS			7
			{3,  4,  2,  5,  7,  8, 12,  2,  6, 11,  9,  2,  2,  2}, // GroupE			8
			{10,10, 10, 10, 10, 10,  2,  2, 10, 10,  9,  2, 10, 10}, // BackSlash		9
			{3,  4,  2,  5,  7,  8,  2,  2,  6, 11,  9,  2,  2,  2}, // escapechar		10
			{3,  4,  2,  5,  7,  8,  2,  2,  6, 11,  9,  2,  2,  2}, // Plus			11
			{3,  4,  2,  5,  7,  8,  2, 13,  6, 11,  9,  2,  2,  2}, // Action			12
			{3,  4,  2,  5,  7,  8,  2, 13,  6, 11,  9,  2,  2,  2}, // Act Index		13
	};
	return (state[i][j]);
}

RegexParser::RegexParser(std::string regex, std::vector<void(*)(std::string)> actions) : Automata(getstate)
{
	std::cout << "Automata constructor called" << std::endl;
	innit();
	Actions = actions;
	str = regex;
	evaluate();
}

bool    read_from(std::ifstream& input_file, const std::string& filename)
{
	input_file.open(filename.c_str());
	if (!input_file.is_open())
	{
		std::cerr << "Error: unable to open input file " << std::endl;
		return false;
	}
	return true;
}

bool startsWith(const std::string& str, const std::string& prefix)
{
	if (str.length() < prefix.length())
		return false;

	return str.compare(0, prefix.length(), prefix) == 0;
}

std::string trim(const std::string& str)
{
	size_t first = 0;
	size_t last = str.length();

	while (first < last && std::isspace(str[first]))
		++first;

	while (last > first && std::isspace(str[last - 1]))
		--last;

	return str.substr(first, last - first);
}

void RegexParser::character()
{
	if (groups.size() > 0)
		groups.back()->AddChar(str[c_idx]);
	else
		regex->AddChar(str[c_idx]);
}

RegexState *CreateOrCondition(RegexState *target)
{
	if (dynamic_cast<OrRState*>(target))
	{
		target->AddState(new AndRState());
		return target;
	}
	else
	{
		OrRState* orState = new OrRState();
		orState->print = true;
		orState->AddState(target);
		orState->AddState(new AndRState());
		return orState;
	}
}

void RegexParser::orfound()
{
	if (groups.size() > 0)
	{
		OrRState* orState = new OrRState();
		//if (dynamic_cast<AndRState*>(groups.back()))
		//{
		//	orState->AddState(groups.back());
		//	groups.pop_back();
		//	groups.push_back(orState);
		//}
		groups.back()->AddState(orState);
		//RegexState *newState = CreateOrCondition(groups.back());
		//groups.pop_back();
		//groups.push_back(newState);
	}
	else
		regex = CreateOrCondition(regex);
}

void RegexParser::setstart()
{
	groups.push_back(new MultiCharState());
}

void RegexParser::setend()
{
	RegexState* group = groups.back();
	groups.pop_back();
	if (groups.size() > 0)
		groups.back()->AddState(group);
	else
		regex->AddState(group);
}

void RegexParser::star()
{
	if (groups.size() > 0)
		groups.back()->AddQuantity(0, INT32_MAX);
	else
		regex->SetQuantity(0, INT32_MAX);
}

void RegexParser::plus()
{
	if (groups.size() > 0)
		groups.back()->AddQuantity(1, INT32_MAX);
	else
		regex->SetQuantity(1, INT32_MAX);
}

void RegexParser::groupstart()
{
	groups.push_back(new GroupRState());
}

void RegexParser::groupending()
{
	regex->AddState(groups.back());
	groups.pop_back();
}

MultiCharState* PreSets(std::string chars, bool negate = false)
{
	MultiCharState* mchar = new MultiCharState();
	mchar->chars = chars;
	mchar->negate = negate;
	return mchar;
}

/// <summary>
/// Se utiliza para indicar el comportamiento luego de un '\'.
/// </summary>
void RegexParser::backslash()
{
	RegexState* target = groups.size() > 0 ? groups.back() : regex;
	if (str[c_idx] == 's')
		target->AddState(PreSets("\n \r\t"));
	else if (str[c_idx] == 'w')
		target->AddState(PreSets("aeiouAEIOU_bcdfghjklmnpqrstvxwyzBCDFGHJKLMNPQRSTVXWYZ"));
	else if (str[c_idx] == 'd')
		target->AddState(PreSets("0123456789"));
	else
		target->AddChar(str[c_idx]);
}

void RegexParser::negate()
{
	RegexState *target = groups.size() > 0 ? groups.back() : regex;
	MultiCharState *mchars = dynamic_cast<MultiCharState*>(target);
	if (mchars)
		mchars->negate = true;
}

void RegexParser::move()
{
	o_idx = c_idx;
}

void RegexParser::action()
{
	std::string strn = str.substr(o_idx + 1, c_idx - o_idx);
	int index = std::atoi(strn.c_str());
	if (index >= 0 && index < Actions.size())
		regex->AddAction(Actions[index]);
}

bool RegexParser::Match(std::string str)
{
	size_t pos = 0;
	std::vector<std::string> matches;
	return regex->Matches(str, pos, matches);
}

std::vector<std::string> RegexParser::Search(std::string str)
{
	size_t pos = 0;
	size_t tpos = 0;
	std::vector<std::string> matches;
	while (pos < str.length())
	{
		tpos = pos;
		bool found = regex->Matches(str, tpos, matches);
		if (found)
			pos = pos == tpos ? pos + 1 : tpos;
		else
			pos++;
	}
	return matches;
}

int RegexParser::evaluate()
{
	this->o_idx = 0;
	this->c_idx = -1;

	while (++c_idx < (int)str.length())
	{
		this->state = this->get_state(this->state, alphabet_idx(this->alphabet, this->str[this->c_idx]));
		if (fta[ostate][state] != NULL)
			(*this.*fta[ostate][state])();
		if (fsa[state] != NULL)
			(*this.*fsa[state])();
		this->ostate = this->state;
	}
	return (this->state);
}