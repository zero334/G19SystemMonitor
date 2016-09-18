#pragma once

#include <string>

class Convert {
public:
	template <typename T> static std::string T_to_string(T const &val) {
		std::ostringstream ostr;
		ostr << val;

		return ostr.str();
	}

	template <typename T> static T string_to_T(std::string const &val) {
		std::istringstream istr(val);
		T returnVal;
		if (!(istr >> returnVal)) {
			MessageBoxA(0, "CFG: Not a valid " + (std::string)typeid(T).name() + " received!\n", "Error", MB_ICONWARNING);
		}

		return returnVal;
	}

	template <> static std::string string_to_T(std::string const &val) {
		return val;
	}
};