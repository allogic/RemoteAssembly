#include <iostream>
#include <string>

extern "C" __declspec(dllexport) void Execute(std::string const& args)
{
  std::cout << args << std::endl;
}