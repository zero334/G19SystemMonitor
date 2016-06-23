#include "configReader.h"

void ConfigReader::removeComment(std::string &line) const {
	if (line.find('#') != line.npos) {
		line.erase(line.find('#'));
	}
}

bool ConfigReader::onlyWhitespace(const std::string &line) const {
	return (line.find_first_not_of(' ') == line.npos);
}

bool ConfigReader::validLine(const std::string &line) const {
	std::string temp = line;
	temp.erase(0, temp.find_first_not_of("\t "));
	if (temp[0] == '=') {
		return false;
	}

	for (size_t i = temp.find('=') + 1; i < temp.length(); i++) {
		if (temp[i] != ' ') {
			return true;
		}
	}
	return false;
}

void ConfigReader::extractKey(std::string &key, size_t const &sepPos, const std::string &line) const {
	key = line.substr(0, sepPos);
	if (key.find('\t') != line.npos || key.find(' ') != line.npos) {
		key.erase(key.find_first_of("\t "));
	}
}

void ConfigReader::extractValue(std::string &value, size_t const &sepPos, const std::string &line) const {
	value = line.substr(sepPos + 1);
	value.erase(0, value.find_first_not_of("\t "));
	value.erase(value.find_last_not_of("\t ") + 1);
}

void ConfigReader::extractContents(const std::string &line) {
	std::string temp = line;
	temp.erase(0, temp.find_first_not_of("\t "));
	const size_t sepPos = temp.find('=');

	std::string key, value;
	extractKey(key, sepPos, temp);
	extractValue(value, sepPos, temp);

	if (!keyExists(key)) {
		contents.insert(std::pair<std::string, std::string>(key, value));
	} else {
		MessageBoxA(0, "CFG: Can only have unique key names!", "Error", MB_ICONWARNING);
	}
}

void ConfigReader::parseLine(const std::string &line, size_t const lineNo) {
	if (line.find('=') == line.npos) {
		const std::string errorMessage = "CFG: Couldn't find separator on line: " + Convert::T_to_string(lineNo);
		MessageBoxA(0, errorMessage.c_str(), "Error", MB_ICONWARNING);
	}

	if (!validLine(line)) {
		const std::string errorMessage = "CFG: Bad format for line: " + Convert::T_to_string(lineNo);
		MessageBoxA(0, errorMessage.c_str(), "Error", MB_ICONWARNING);
	}

	extractContents(line);
}

void ConfigReader::ExtractKeys() {
	std::ifstream file;
	file.open(fName.c_str());
	if (!file) {
		const std::string errorMessage = "CFG: File " + fName + " couldn't be found!\n";
		MessageBoxA(0, errorMessage.c_str(), "Error", MB_ICONWARNING);
	}
			
	std::string line;
	size_t lineNo = 0;
	while (std::getline(file, line)) {
		lineNo++;
		std::string temp = line;

		if (temp.empty()) {
			continue;
		}
			
		removeComment(temp);
		if (onlyWhitespace(temp)) {
			continue;
		}
			
		parseLine(temp, lineNo);
	}

	file.close();
}

ConfigReader::ConfigReader(const std::string &fName) {
	this->fName = fName;
	ExtractKeys();
}

bool ConfigReader::keyExists(const std::string &key) const {
	return contents.find(key) != contents.end();
}