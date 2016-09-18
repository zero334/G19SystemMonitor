#pragma once
#include <string>


class Cast {
public:
	static std::wstring toWstring(const std::string &s) {
		return std::wstring(s.begin(), s.end());
	}
};