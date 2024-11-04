# include "RegexParser.hpp"

void Method(std::string match)
{
	std::cout << match << std::endl;
}

void Location(std::string match)
{
	std::cout << match << std::endl;
}

void Headers(std::string match)
{
	std::cout << match << std::endl;
}

int main()
{
	std::vector<void(*)(std::string)> actions;
	actions.push_back(Method);
	actions.push_back(Location);
	actions.push_back(Headers);

	RegexParser regex("(GET|POST)~0 ([^ ]+)~1 (HTTP/1.1)\\s+(\\w+: [\\w\\d./\\*]+\\s+)~2*", actions);

	std::vector<std::string> matches = regex.Search("GET /.well-known/acme-challenge/abc123 HTTP/1.1\r\n"
							"Host: examplecom\r\n"
							"User_Agent: Mozilla50\r\n"
							"Accept: */*\r\n"
							"Algo: nada\r\n");
	return 0;
}