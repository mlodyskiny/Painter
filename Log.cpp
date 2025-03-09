#include "Log.h"

#include <iostream>

void LogImplementation(const std::wstring& Message)
{
	std::wcout << Message;
}