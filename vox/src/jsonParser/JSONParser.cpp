#include "JSONParser.hpp"

void JSONParser::parse()
{
	std::string key = "";
	while (_tokenizer.hasMoreTokens())
	{
		Token token;
		try
		{
			token = _tokenizer.getToken();
			switch (token.type)
			{
			case TOKEN::CURLY_OPEN:
			{
				std::shared_ptr<JSONNode> parsedObject = parseObject();
				if (!_root)
					_root = parsedObject;
				break;
			}
			case TOKEN::ARRAY_OPEN:
			{
				std::shared_ptr<JSONNode> parsedList = parseList();
				if (!_root)
					_root = parsedList;
				break;
			}
			case TOKEN::STRING:
			{
				_tokenizer.rollBackToken();
				std::shared_ptr<JSONNode> parsedString = parseString();
				if (!_root)
					_root = parsedString;
				break;
			}
			case TOKEN::NUMBER:
			{
				_tokenizer.rollBackToken();
				std::shared_ptr<JSONNode> parsedNumber = parseNumber();
				if (!_root)
					_root = parsedNumber;
				break;
			}
			case TOKEN::BOOLEAN:
			{
				_tokenizer.rollBackToken();
				std::shared_ptr<JSONNode> parsedBoolean = parseBoolean();
				break;
			}
			}
		}
		catch (std::logic_error e)
		{
			break;
		}
	}
}

std::shared_ptr<JSONNode> JSONParser::parseObject()
{
	std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
	JSONObject* keyObjectMap = new JSONObject();
	bool hasCompleted = false;
	while (!hasCompleted)
	{
		if (_tokenizer.hasMoreTokens())
		{
			Token nextToken = _tokenizer.getToken();
			std::string key = nextToken.value;
			_tokenizer.getToken();
			nextToken = _tokenizer.getToken();
			std::shared_ptr<JSONNode> node;
			switch (nextToken.type)
			{
			case TOKEN::STRING:
			{
				_tokenizer.rollBackToken();
				(*keyObjectMap)[key] = parseString();
				break;
			}
			case TOKEN::ARRAY_OPEN:
			{
				(*keyObjectMap)[key] = parseList();
				break;
			}
			case TOKEN::NUMBER:
			{
				_tokenizer.rollBackToken();
				(*keyObjectMap)[key] = parseNumber();
				break;
			}
			case TOKEN::CURLY_OPEN:
			{
				(*keyObjectMap)[key] = parseObject();
				break;
			}
			case TOKEN::BOOLEAN:
			{
				_tokenizer.rollBackToken();
				(*keyObjectMap)[key] = parseBoolean();
				break;

			}
			case TOKEN::NULL_TYPE:
			{
				(*keyObjectMap)[key] = parseNull();
				break;
			}
			}
			nextToken = _tokenizer.getToken();
			if (nextToken.type == TOKEN::CURLY_CLOSE)
			{
				hasCompleted = true;
				break;
			}
		}
		else
			throw std::logic_error("No more tokens");
	}
	node->setObject(keyObjectMap);
	return node;
}

std::shared_ptr<JSONNode> JSONParser::parseString()
{
	std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
	Token token = _tokenizer.getToken();
	std::string* sValue = new std::string(token.value);
	node->setString(sValue);
	return node;
}

std::shared_ptr<JSONNode> JSONParser::parseNumber()
{
	std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
	Token nextToken = _tokenizer.getToken();
	std::string value = nextToken.value;
	float fValue = std::stof(value);
	node->setNumber(fValue);
	return node;
}

std::shared_ptr<JSONNode> JSONParser::parseList()
{
	std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
	JSONList* list = new JSONList();
	bool hasCompleted = false;
	while (!hasCompleted)
	{
		if (!_tokenizer.hasMoreTokens())
			throw std::logic_error("No more tokens");
		else
		{
			Token nextToken = _tokenizer.getToken();
			std::shared_ptr<JSONNode> node;
			switch (nextToken.type)
			{
			case TOKEN::ARRAY_OPEN:
			{
				node = parseList();
				break;
			}
			case TOKEN::CURLY_OPEN:
			{
				node = parseObject();
				break;
			}
			case TOKEN::STRING:
			{
				_tokenizer.rollBackToken();
				node = parseString();
				break;
			}
			case TOKEN::NUMBER:
			{
				_tokenizer.rollBackToken();
				node = parseNumber();
				break;
			}
			case TOKEN::BOOLEAN:
			{
				_tokenizer.rollBackToken();
				node = parseBoolean();
				break;
			}
			case TOKEN::NULL_TYPE:
			{
				node = parseNull();
				break;
			}
			}
			list->push_back(node);
			nextToken = _tokenizer.getToken();
			if (nextToken.type == TOKEN::ARRAY_CLOSE)
				hasCompleted = true;
		}
	}
	node->setList(list);
	return node;
}

std::shared_ptr<JSONNode> JSONParser::parseBoolean()
{
	std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
	Token nextToken = _tokenizer.getToken();
	node->setBoolean(nextToken.value == "True" ? true : false);
	return node;
}

std::shared_ptr<JSONNode> JSONParser::parseNull()
{
	std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
	node->setNull();
	return node;
}

void JSONParser::print()
{
	if (_root)
		_root->printNode();
}
