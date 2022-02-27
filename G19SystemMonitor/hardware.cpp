#include "hardware.h"

#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

std::vector<std::wstring> Hardware::getCpuLoadInfo(const unsigned int& coreNumber) {

	// Step 1: --------------------------------------------------
	// Initialize COM. ------------------------------------------

	HRESULT hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres)) {
		std::cout << "Failed to initialize COM library. Error code = 0x" << std::hex << hres << std::endl;
		return {};                  // Program has failed.
	}

	// Step 2: --------------------------------------------------
	// Set general COM security levels --------------------------
	// Note: If you are using Windows 2000, you need to specify -
	// the default authentication credentials for a user by using
	// a SOLE_AUTHENTICATION_LIST structure in the pAuthList ----
	// parameter of CoInitializeSecurity ------------------------

	hres = CoInitializeSecurity(
		NULL,
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities
		NULL                         // Reserved
		);


	if (FAILED(hres)) {
		std::cout << "Failed to initialize security. Error code = 0x" << std::hex << hres << std::endl;
		CoUninitialize();
		return {};                   // Program has failed.
	}

	// Step 3: ---------------------------------------------------
	// Obtain the initial locator to WMI -------------------------

	IWbemLocator *pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hres)) {
		std::cout << "Failed to create IWbemLocator object." << " Err code = 0x" << std::hex << hres << std::endl;
		CoUninitialize();
		return {};                // Program has failed.
	}

	// Step 4: -----------------------------------------------------
	// Connect to WMI through the IWbemLocator::ConnectServer method

	IWbemServices *pSvc = NULL;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (e.g. Kerberos)
		0,                       // Context object
		&pSvc                    // pointer to IWbemServices proxy
		);

	if (FAILED(hres)) {
		std::cout << "Could not connect. Error code = 0x" << std::hex << hres << std::endl;
		pLoc->Release();
		CoUninitialize();
		return {};               // Program has failed.
	}

	// cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;


	// Step 5: --------------------------------------------------
	// Set security levels on the proxy -------------------------

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities
		);

	if (FAILED(hres)) {
		std::cout << "Could not set proxy blanket. Error code = 0x" << std::hex << hres << std::endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return {};                    // Program has failed.
	}

	// Step 6: --------------------------------------------------
	// Use the IWbemServices pointer to make requests of WMI ----

	// For example, get the name of the operating system
	IEnumWbemClassObject *pEnumerator = NULL;
	IWbemClassObject *pclsObj;

	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_PerfFormattedData_PerfOS_Processor"),

		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres)) {
		std::cout << "Query for operating system name failed." << " Error code = 0x" << std::hex << hres << std::endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return {};               // Program has failed.
	}

	// Step 7: -------------------------------------------------
	// Get the data from the query in step 6 -------------------

	ULONG uReturn = 0;
	std::vector<std::wstring> back;
	for (unsigned int iter = 1; pEnumerator && iter <= coreNumber; iter++) {
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if (!SUCCEEDED(hr) || uReturn == NULL) {
			break;
		}

		VARIANT vtProp{};
		// Get the value of the Name property
		hr = pclsObj->Get(L"PercentProcessorTime", 0, &vtProp, 0, 0);
		
		if (SUCCEEDED(hr)) {
			back.push_back(vtProp.bstrVal);
		}

		// Cleanup
		VariantClear(&vtProp);
		pclsObj->Release();
	}

	// Cleanup
	// ========

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();

	return back;   // Program successfully completed.
}

unsigned short Hardware::getVirtualMemoryCurrentlyUsed() {
	MEMORYSTATUSEX statex{};
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	return static_cast<unsigned short>(statex.dwMemoryLoad);
}