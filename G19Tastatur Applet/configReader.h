#pragma once

#include "Convert.h"
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <Windows.h>


class ConfigReader {
private:
	std::map<std::string, std::string> contents;
	std::string fName;

	void removeComment(std::string &line) const;
	bool onlyWhitespace(const std::string &line) const;
	bool validLine(const std::string &line) const;
	void extractKey(std::string &key, size_t const &sepPos, const std::string &line) const;
	void extractValue(std::string &value, size_t const &sepPos, const std::string &line) const;
	void extractContents(const std::string &line);
	void parseLine(const std::string &line, size_t const lineNo);
	void ExtractKeys();

public:
	ConfigReader(const std::string &fName);

	bool keyExists(const std::string &key) const;

	template <typename ValueType>
	ValueType getValueOfKey(const std::string &key, ValueType const &defaultValue = ValueType()) const {
		if (!keyExists(key))
			return defaultValue;

		return Convert::string_to_T<ValueType>(contents.find(key)->second);
	}
};