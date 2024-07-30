#include "Winapi.hpp"

#include <windows.h>
#include <iostream>
#include <exception>

int ExecuteProgram(const char *program_path, char *args)
{
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    HANDLE h_read, h_write;
    if(!CreatePipe(&h_read, &h_write, &sa, 0))
    {
        std::cout << "Wasn't able to create anonymous pipe\n";
        TerminateAdmin();
    }

    PROCESS_INFORMATION pi;
    STARTUPINFOA si;

    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));

    si.cb = sizeof(STARTUPINFOA);
    si.hStdOutput = h_write;
    si.dwFlags |= STARTF_USESTDHANDLES;

    if(!CreateProcessAsUserA(
        NULL,
        program_path,
        args,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    ))
    {
        std::cout << "Wasn't able to create child process in order to open \"" << program_path << "\" with error " << GetLastError() << '\n';
        TerminateAdmin();
    }

    //WaitForSingleObject(&pi, INFINITE);
    CloseHandle(h_write);

    char buffer[4096];
    DWORD bytesRead;
    while(ReadFile(h_read, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0)
        std::cout.write(buffer, bytesRead);
    
    long unsigned int exit_code = 0; // what is 'long unsigned int'???? It's not uint32_t and not uint64_t
    GetExitCodeProcess(pi.hProcess, &exit_code);

    CloseHandle(h_read);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    return exit_code;
}

void GetScreenResolution(int &width, int &height)
{
    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);
}

void TerminateAdmin()
{
    std::wcout << L"Termination\n"
                  L"Try opening with administrator rights\n";
    std::terminate();
}