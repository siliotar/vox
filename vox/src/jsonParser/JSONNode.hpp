#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>

class JSONNode;

using JSONObject = std::map<std::string, std::shared_ptr<JSONNode> >;
using JSONList = std::vector<std::shared_ptr<JSONNode> >;

class JSONNode
{
private:
	enum class Type
	{
		OBJECT,
		LIST,
		STRING,
		NUMBER,
		BOOLEAN,
		NULL_TYPE
	};
	union Values
	{
		JSONObject* object;
		JSONList* list;
		std::string* s;
		float fValue;
		bool bValue;
	} _values;
	Type _type;

public:
	JSONNode() : _values(), _type() {}
	JSONNode(Type t) : _values(), _type(t) {}
	~JSONNode() = default;

	JSONObject& getObject();
	JSONList& getList();
	std::string& getString();
	float getFloat();

	void setObject(JSONObject* object);
	void setString(std::string* s);
	void setNumber(float f);
	void setList(JSONList* list);
	void setBoolean(bool v);
	void setNull();

	std::string toString(int indentationLevel);

	void printNode();
};
