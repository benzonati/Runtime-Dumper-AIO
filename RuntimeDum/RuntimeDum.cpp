#include <iostream>
#include "ProcessManager/Manager.hpp"
#include "Scanner/Scanner.hpp"
#include "ExternalOffsets/External.hpp"
#include <print>
#include <fstream>
// made by volxphy


int main()
{
    Process proc = ProcessManager::SetupProcess("Roblox");
    if (!proc.pid) {
        printf("Failed to attach to process! Is roblox open gang?\n");
        std::cin.get();
        exit(EXIT_FAILURE);
    }
    printf("/*\n");
    printf("    PID: %d\n", proc.pid);
    printf("    WindowHandle: %p\n", proc.windowhandle);
    printf("    ProcessHandle: %p\n", proc.processhandle);
    for (const auto& [name, address] : proc.Modules) {
        printf("    %s - 0x%llx\n", name.c_str(), (unsigned long long)address);
    }
    printf("\n    DUMPED BY VOLXPHY\n*/");
    std::cout << "\n\n\n";
    ProcessManager::SuspendProcess(proc); // stop roblox from encrypting functions !! BE INGAME LIKE "NATURAL DISASTER SURVIVAL" !!
	

    ExternalDump::DumpXternal(proc);
    ExternalDump::DumpEx(proc);
    printf("\n\n// EXTERNAL OFFSETS\n");
	std::ofstream outputFile("Offsets/offsets.hpp");

	if (outputFile.is_open()) {
		outputFile << "#pragma once\n\nnamespace Offsets {"
	}
    // Check if the file was opened successfully
    for (const auto& addr : ObtainedXAddresses) {
        if (addr.value == 0x0) {
			if (outputFile.is_open()) {
		        outputFile << ("  inline uintptr_t %s = 0xDEADBEEFDEADC0DE; // FALED TO DUMP\n", addr.name.c_str());
		    } else {
		        std::cout << ("  inline uintptr_t %s = 0xDEADBEEFDEADC0DE; // FALED TO DUMP\n", addr.name.c_str());
		    }
        }
        else {
			if (outputFile.is_open()) {
            	outputFile << ("  inline uintptr_t %s = 0x%llX;\n", addr.name.c_str(), addr.value);
			}else{
				std::cout << ("  inline uintptr_t %s = 0x%llX;\n", addr.name.c_str(), addr.value);
			}
        }
    }
	if (outputFile.is_open()) {
		outputFile << "}";
		outputFile.close();
	}
    std::cin.get();
    TerminateProcess(proc.processhandle, NULL);
    CloseHandle(proc.processhandle);
    exit(EXIT_SUCCESS);
}
