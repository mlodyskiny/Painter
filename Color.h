#pragma once

#include <string>

struct FColor
{
	unsigned char R;
	unsigned char G;
	unsigned char B;

	bool operator==(const FColor& Other) const
	{
		return R == Other.R && G == Other.G && B == Other.B;
	}

	bool operator!=(const FColor& Other) const
	{
		return !(*this == Other);
	}

	static FColor FromHex(const std::string& Color);
	std::string ToHex() const;
};