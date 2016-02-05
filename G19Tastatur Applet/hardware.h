#include <vector>
#include <string>

class Hardware {

public:
	static std::vector<std::wstring> getCpuLoadInfo(const unsigned int& coreNumber);
	inline static unsigned short getVirtualMemoryCurrentlyUsed();
};