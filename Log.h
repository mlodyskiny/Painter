#pragma once

#include <format>

enum ELogLevel : unsigned char
{
	Info,
	Warning,
	Error
};

constexpr const wchar_t* LogLevelToString(const ELogLevel Level)
{
	switch (Level)
	{
	case Info:
	{
		return L"\033[1;94mInfo\033[0m";
	}
	case Warning:
	{
		return L"\033[1;93mWarning\033[0m";
	}
	case Error:
	{
		return L"\033[1;91mError\033[0m";
	}
	default:
	{
		return L"\033[1;90mUnknown\033[0m";
	}
	}
}

void LogImplementation(const std::wstring& Message);

template<typename... Args>
__forceinline void LogFunction(
	const ELogLevel Level,
	const wchar_t* Fmt,
	const wchar_t* File,
	const unsigned int Line,
	const wchar_t* Function,
	Args&&... Arguments
)
{
	const std::wstring OutMessage = std::format(
		L"[{}, {}, {}:{}] {}\n",
		LogLevelToString(Level),
		Function,
		File,
		Line,
		std::vformat(Fmt, std::make_wformat_args(Arguments...))
	);

	LogImplementation(OutMessage);
}

#define Log(Level, Fmt, ...) \
	LogFunction(Level, Fmt, __FILEW__, __LINE__, _CRT_WIDE(__FUNCTION__), __VA_ARGS__)