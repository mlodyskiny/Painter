#pragma once

#include <string>
#include <vector>

#include "Color.h"
#include "Pos.h"

class NParser
{
public:
	NParser();
	NParser(const FColor& ExcludeColor);
	~NParser() = default;

public:
	std::vector<std::pair<FColor, FPos>> ParseFromFile(const std::string& ImagePath);
	std::vector<std::pair<FColor, FPos>> ParseFromBuffer(const std::vector<uint8_t>& ImageBuffer);

private:
	bool bExclude = false;
	FColor ExcludeColor;
};