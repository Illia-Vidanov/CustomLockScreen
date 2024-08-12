#ifndef WIN_API_HPP
#define WIN_API_HPP

// Start process specified by path, with arguments and wait till it finished, exit code is returned. All arguments must be passed in utf8 encoding
auto ExecuteProgram(const char *program_path, char *args) -> int;
void TerminateAdmin();
void GetScreenResolution(int &width, int &height);

auto ToWideChar(const char *str) -> wchar_t *;

#endif // WIN_API_HPP