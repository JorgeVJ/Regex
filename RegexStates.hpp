	#ifndef REGEXSTATES_HPP
	#define REGEXSTATES_HPP

	# include <stdexcept>
	# include <iostream>
	# include <iomanip>
	# include <utility>
	# include <vector>
	# include <string>
	# include <map>
	# include <stack>
	# include <memory>

	class RegexState
	{
		public:
			int min;
			int max;
			~RegexState();
			virtual void AddChar(char c);
			virtual void AddAction(void (*f)(std::string));
			virtual void AddState(RegexState *state);
			virtual void AddQuantity(int min, int max);
			virtual void SetQuantity(int min, int max);
			virtual bool Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches) const;

	protected:
		RegexState();
	};

	class AndRState : public RegexState
	{
		public:
			bool print;
			std::vector<RegexState*> states;
			AndRState();
			AndRState(AndRState& other);
			~AndRState();
			void AddChar(char c) override;
			void AddState(RegexState* state) override;
			void AddAction(void (*f)(std::string)) override;
			void AddQuantity(int min, int max) override;
			void SetQuantity(int min, int max) override;
			bool Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches) const override;
	};

	class CharRState : public RegexState
	{
		public:
			CharRState(char c);
			char character;
			bool Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches) const override;
	};

	class OrRState : public RegexState
	{
		public:
			bool print;
			std::vector<RegexState*> states;
			void AddChar(char c) override;
			void AddState(RegexState* state) override;
			void AddAction(void (*f)(std::string)) override;
			void AddQuantity(int min, int max) override;
			void SetQuantity(int min, int max) override;
			bool Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches) const override;
	};

	class MultiCharState : public RegexState
	{
		public:
			bool		negate;
			std::string chars;
			void AddChar(char c) override;
			void AddState(RegexState* state) override;
			bool Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches) const override;
	};

	class GroupRState : public RegexState
	{
		private:
			void (*OnValidate)(std::string);
		public:
			RegexState *conditions;
			GroupRState();
			void AddChar(char c) override;
			void AddAction(void (*f)(std::string)) override;
			void AddQuantity(int min, int max) override;
			void AddState(RegexState* state) override;
			bool Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches) const override;
	};

	#endif