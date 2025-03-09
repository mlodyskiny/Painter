#include <Windows.h>
#include <string>
#include <fstream>
#include <thread>
#include <filesystem>

#include "Color.h"
#include "Parser.h"
#include "Log.h"
#include "Painter.h"
#include "Utils.h"

int main(int argc, char* argv[])
{
	try
	{
		if (argc < 5 || argc > 6)
		{
			std::wstring ModuleFileName{};
			ModuleFileName.resize(MAX_PATH);

			const DWORD Result = GetModuleFileNameW(
				*reinterpret_cast<HMODULE*>(__readgsqword(0x60) + 0x10),
				ModuleFileName.data(),
				ModuleFileName.size()
			);

			if (!Result)
			{
				ModuleFileName = L"Painter";
			#if defined(_DEBUG) // i tak jako debug to guwno sie nie kompiluje Xddd
				ModuleFileName += L"-Debug.exe";
			#else
				ModuleFileName += L"-Release.exe";
			#endif
			}
			else
			{
				ModuleFileName = ModuleFileName.substr(
					ModuleFileName.find_last_of(L'\\') + 1
				);
			}

			Log(Error, L"Usage: {} <image path> <cookies path> <start X> <start Y> <exclude color hex | optional> (example: {} test.png cookies.txt 0 0 #FF0000)", ModuleFileName, ModuleFileName);
			return 4;
		}

		NParser Parser = argc == 2 ? NParser{} : NParser{ FColor::FromHex(argv[5]) };
		const std::vector<std::pair<FColor, FPos>> ParsedDrawPositions = Parser.ParseFromFile(argv[1]);

		const std::string CookiesPath = argv[2];
		if (!std::filesystem::exists(CookiesPath))
		{
			throw std::runtime_error("Cookies file doesn't exist!");
		}

		std::ifstream InputStream(CookiesPath);
		if (!InputStream.is_open())
		{
			throw std::runtime_error("Couldn't open the cookies input stream!");
		}

		std::string Line{};
		std::vector<std::string> Cookies;
		while (std::getline(InputStream, Line))
		{
			Cookies.emplace_back(Line);
		}

		if (Cookies.empty())
		{
			throw std::runtime_error("No cookies in the cookies file!");
		}

		Log(Info, L"Got {} cookies!", Cookies.size());

		const FPos StartPos{ std::stoi(argv[3]), std::stoi(argv[4]) };

		if (Cookies.size() == 1)
		{
			NPainter(Cookies[0]).DrawList(ParsedDrawPositions, StartPos);
		}
		else
		{
			const auto SplitDrawPositions = Utils::SplitVector(ParsedDrawPositions, Cookies.size());

			// nwm wale jako ptr bo nie wiem czy bez sie zjebie XDDDDD
			std::vector<std::pair<NPainter*, std::thread*>> Painters{};
			for (size_t i = 0; i < Cookies.size(); ++i)
			{
				NPainter* Painter = new NPainter(Cookies[i]);
				Painters.emplace_back(
					Painter, new std::thread(&NPainter::DrawList, Painter, SplitDrawPositions[i], StartPos)
				);
			}

			for (auto& [Painter, Thread] : Painters)
			{
				Thread->join();
				delete Thread;
				delete Painter;
			}
		}

		Log(Info, L"Drawing process done!");
	}
	catch (const std::runtime_error& RuntimeError)
	{
		std::string ErrorA = RuntimeError.what();
		std::wstring ErrorW(ErrorA.begin(), ErrorA.end());
		Log(Error, L"A runtime error was caught: {}", ErrorW);
		return 1;
	}
	catch (const std::exception& Exception)
	{
		std::string ExceptionA = Exception.what();
		std::wstring ExceptionW(ExceptionA.begin(), ExceptionA.end());
		Log(Error, L"An exception was caught: {}", ExceptionW);
		return 2;
	}
	catch (...)
	{
		Log(Error, L"An unknown exception has occured!");
		return 3;
	}

	return 0;
}
