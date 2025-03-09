#include "Parser.h"

#include <fstream>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Log.h"

NParser::NParser() : bExclude(false), ExcludeColor({})
{
}

NParser::NParser(const FColor& ExcludeColor) : bExclude(true), ExcludeColor(ExcludeColor)
{
	Log(Info, L"Exclude color: R - {}, G - {}, B - {}.", ExcludeColor.R, ExcludeColor.G, ExcludeColor.B);
}

std::vector<std::pair<FColor, FPos>> NParser::ParseFromFile(const std::string& ImagePath)
{
	if (!ImagePath.ends_with(".png"))
	{
		throw std::runtime_error("Image file isn't a .png file!");
	}

	if (!std::filesystem::exists(ImagePath))
	{
		throw std::runtime_error("Image file doesn't exist!");
	}

	std::ifstream InputStream(ImagePath, std::ios::binary);
	if (!InputStream.is_open())
	{
		throw std::runtime_error("Couldn't open the image input stream!");
	}

	InputStream.seekg(0, std::ios::end);
	const size_t ImageSize = InputStream.tellg();
	Log(Info, L"Image size: {}.", ImageSize);
	InputStream.seekg(0, std::ios::beg);

	std::vector<uint8_t> ImageBuffer(ImageSize);
	InputStream.read(reinterpret_cast<char*>(ImageBuffer.data()), ImageSize);
	InputStream.close();

	return ParseFromBuffer(ImageBuffer);
}

std::vector<std::pair<FColor, FPos>> NParser::ParseFromBuffer(const std::vector<uint8_t>& ImageBuffer)
{
	int Width, Height, Channels;
	unsigned char* ImageData = stbi_load_from_memory(ImageBuffer.data(), static_cast<int>(ImageBuffer.size()), &Width, &Height, &Channels, 3);
	if (!ImageData)
	{
		throw std::runtime_error("Couldn't load the image!");
	}

	Log(
		Info, 
		L"Image loaded successfully! Image size: {}x{} ({} pixels).", 
		Width, 
		Height, 
		Width * Height
	);

	std::vector<std::pair<FColor, FPos>> Out{};
	for (int Y = 0; Y < Height; ++Y)
	{
		for (int X = 0; X < Width; ++X)
		{
			const int i = (Y * Width + X) * 3;
			const FColor Color{ ImageData[i], ImageData[i + 1], ImageData[i + 2] };
			if (bExclude && Color == ExcludeColor)
			{
				continue;
			}

			Out.emplace_back(Color, FPos{ X, Y });
		}
	}

	if (bExclude)
	{
		Log(Info, L"Excluded {} pixels.", Width * Height - Out.size());
	}

	stbi_image_free(ImageData);
	return Out;
}
