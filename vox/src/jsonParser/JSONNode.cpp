#include "JSONNode.hpp"
#include <stdexcept>
#include <iostream>

JSONObject& JSONNode::getObject()
{
	if (_type == Type::OBJECT)
		return *_values.object;
	throw std::logic_error("Improper return");
}

JSONList& JSONNode::getList()
{
	if (_type == Type::LIST)
		return *_values.list;
	throw std::logic_error("Improper return");
}

std::string& JSONNode::getString()
{
	if (_type == Type::STRING)
		return *_values.s;
	throw std::logic_error("Improper return");
}

float JSONNode::getFloat()
{
	if (_type == Type::NUMBER)
		return _values.fValue;
	throw std::logic_error("Improper return");
}

void JSONNode::setObject(JSONObject* object)
{
	this->_values.object = object;
	_type = Type::OBJECT;
}

void JSONNode::setString(std::string* s)
{
	this->_values.s = s;
	_type = Type::STRING;
}

void JSONNode::setNumber(float f)
{
	this->_values.fValue = f;
	_type = Type::NUMBER;
}

void JSONNode::setList(JSONList* list)
{
	this->_values.list = list;
	_type = Type::LIST;
}

void JSONNode::setBoolean(bool v)
{
	this->_values.bValue = v;
	_type = Type::BOOLEAN;
}

void JSONNode::setNull()
{
	_type = Type::NULL_TYPE;
}

std::string JSONNode::toString(int indentationLevel)
{
	std::string spaceString = std::string(indentationLevel * 2, ' ');
	std::string prevSpaceString = std::string((indentationLevel - 1) * 2, ' ');
	std::string outputString = "";
	switch (_type)
	{
	case Type::STRING:
		outputString += *_values.s;
		break;
	case Type::NUMBER:
		outputString += std::to_string(_values.fValue);
		break;
	case Type::BOOLEAN:
		outputString += _values.bValue ? "true" : "false";
		break;
	case Type::NULL_TYPE:
		outputString += "null";
		break;
	case Type::LIST:
	{
		outputString += "[";
		size_t index = 0;
		for (auto node : (*_values.list))
		{
			outputString += node->toString(indentationLevel + 1);
			if (index < (*_values.list).size() - 1)
				outputString += ", ";
			++index;
		}
		outputString += "]";
		break;
	}
	case Type::OBJECT:
		if (indentationLevel != 1)
			outputString += "\n";
		outputString += prevSpaceString + "{\n";
		for (JSONObject::iterator i = (*_values.object).begin(); i != (*_values.object).end(); ++i)
		{
			outputString += spaceString + i->first + ": ";
			outputString += i->second->toString(indentationLevel + 1);
			JSONObject::iterator next = i;
			++next;
			if ((next) != (*_values.object).end())
				outputString += ",";
			outputString += "\n";
		}
		outputString += prevSpaceString + "}";
		break;
	default:
		break;
	}
	return outputString;
}

void JSONNode::printNode()
{
	std::cout << toString(1);
}
