#include "Tokenizer.hpp"
#include <stdexcept>

Tokenizer::Tokenizer(std::string fileName) : _prevPos(0)
{
	_file.open(fileName, std::ios::in);
	if (!_file.good())
		throw "File open error";
}
char Tokenizer::getWithoutWhiteSpace()
{
	char c = ' ';
	while ((c == ' ' || c == '\n'))
	{
		_file.get(c);

		if ((c == ' ' || c == '\n') && !_file.good())
			throw std::logic_error("Run out of tokens");
		else if (!_file.good())
			return c;
	}
	return c;
}
Token Tokenizer::getToken()
{
	char c;
	if (_file.eof())
		throw std::exception("Exhausted tokens");
	_prevPos = size_t(_file.tellg());
	c = getWithoutWhiteSpace();

	Token token;
	if (c == '"')
	{
		token.type = TOKEN::STRING;
		token.value = "";
		_file.get(c);
		while (c != '"')
		{
			token.value += c;
			_file.get(c);
		}
	}
	else if (c == '{')
		token.type = TOKEN::CURLY_OPEN;
	else if (c == '}')
		token.type = TOKEN::CURLY_CLOSE;
	else if (c == '-' || (c >= '0' && c <= '9'))
	{
		token.type = TOKEN::NUMBER;
		token.value = "";
		token.value += c;
		std::streampos prevCharPos = _file.tellg();
		while ((c == '-') || (c >= '0' && c <= '9') || c == '.')
		{
			prevCharPos = _file.tellg();
			_file.get(c);

			if (_file.eof())
				break;
			if ((c == '-') || (c >= '0' && c <= '9') || (c == '.'))
				token.value += c;
			else
				_file.seekg(prevCharPos);
		}
	}
	else if (c == 'f')
	{
		for (size_t i = 1; i < 5; ++i)
		{
			_file.get(c);
			if (c != *("false" + i))
				throw std::exception("JSON file corrupted");
		}
		token.type = TOKEN::BOOLEAN;
		token.value = "False";
	}
	else if (c == 't')
	{
		for (size_t i = 1; i < 4; ++i)
		{
			_file.get(c);
			if (c != *("true" + i))
				throw std::exception("JSON file corrupted");
		}
		token.type = TOKEN::BOOLEAN;
		token.value = "True";
	}
	else if (c == 'n')
	{
		for (size_t i = 1; i < 4; ++i)
		{
			_file.get(c);
			if (c != *("null" + i))
				throw std::exception("JSON file corrupted");
		}
		token.type = TOKEN::NULL_TYPE;
	}
	else if (c == '[')
		token.type = TOKEN::ARRAY_OPEN;
	else if (c == ']')
		token.type = TOKEN::ARRAY_CLOSE;
	else if (c == ':')
		token.type = TOKEN::COLON;
	else if (c == ',')
		token.type = TOKEN::COMMA;
	return token;
}

bool Tokenizer::hasMoreTokens()
{
	return !_file.eof();
}

void Tokenizer::rollBackToken()
{
	if (_file.eof())
		_file.clear();
	_file.seekg(_prevPos);
}
