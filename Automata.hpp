# include <iostream>

class Automata
{
protected:
	int			c_idx;
	int			o_idx;
	void* data;
	std::string* alphabet;
	int			alphabet_length;
	std::string* errors;
	int			errors_length;
	int			(Automata::* state_function)(int, int);
	int			alphabet_idx(std::string* alphabet, char c);
	void		innit();

public:
	Automata();
	Automata(void* data);
	Automata(int (*get_state)(int, int));
	~Automata();
	std::string	str;
	int			state;
	int			ostate;
	virtual int	evaluate();
	int			(*get_state)(int, int);
	void		set_string(const std::string& str);
	void		set_alphabet(size_t size, const std::string* strings);
	void		set_errors(size_t size, const std::string* strings);
	void		print_state();
};