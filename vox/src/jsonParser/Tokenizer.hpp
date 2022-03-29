#pragma once

#include <fstream>

enum class TOKEN
{
	CURLY_OPEN,
	CURLY_CLOSE,
	COLON,
	STRING,
	NUMBER,
	ARRAY_OPEN,
	ARRAY_CLOSE,
	COMMA,
	BOOLEAN,
	NULL_TYPE
};

struct Token
{
	std::string value;
	TOKEN type;
};

class Tokenizer
{
private:
	std::fstream _file;
	size_t _prevPos;

public:
	Tokenizer(std::string fileName);
	~Tokenizer() = default;

	char getWithoutWhiteSpace();
	Token getToken();
	bool hasMoreTokens();
	void rollBackToken();
};
