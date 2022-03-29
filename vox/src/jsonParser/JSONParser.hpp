#pragma once

#include "JSONNode.hpp"
#include "Tokenizer.hpp"
#include <memory>

class JSONParser
{
private:
	std::fstream _file;
	std::shared_ptr<JSONNode> _root;
	std::unique_ptr<JSONNode> _current;
	Tokenizer _tokenizer;

public:
	JSONParser(const std::string filename) : _tokenizer(filename) {}

	void parse();

	std::shared_ptr<JSONNode> parseObject();
	std::shared_ptr<JSONNode> parseString();
	std::shared_ptr<JSONNode> parseNumber();
	std::shared_ptr<JSONNode> parseList();
	std::shared_ptr<JSONNode> parseBoolean();
	std::shared_ptr<JSONNode> parseNull();

	void print();
};
