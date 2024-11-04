#ifndef REGEXPARSER_HPP
# define REGEXPARSER_HPP

# include "RegexStates.hpp"
# include "Automata.hpp"
# include <vector>
# include <stack>

class RegexParser : public Automata
{
	private:
		bool groupend;
		RegexState* regex;
		std::vector<RegexState*> groups;
		void innit();
		void character();
		void orfound();
		void setstart();
		void setend();
		void star();
		void plus();
		void groupstart();
		void groupending();
		void backslash();
		void negate();
		void move();
		void action();
		void (RegexParser::* fsa[20])(void);
		void (RegexParser::* fta[20][20])(void);

	public:
		std::vector<void (*)(std::string)> Actions;
		bool Match(std::string str);
		std::vector<std::string> Search(std::string str);
		RegexParser(std::string regex, std::vector<void(*)(std::string)> actions);
		int evaluate();
};
#endif