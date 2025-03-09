#pragma once

struct FPos
{
	int X;
	int Y;

	bool operator==(const FPos& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	bool operator!=(const FPos& Other) const
	{
		return !(*this == Other);
	}

	FPos operator+(const FPos& Other) const
	{
		return FPos{ X + Other.X, Y + Other.Y };
	}
	FPos operator-(const FPos& Other) const
	{
		return FPos{ X - Other.X, Y - Other.Y };
	}
	FPos operator*(const FPos& Other) const
	{
		return FPos{ X * Other.X, Y * Other.Y };
	}
	FPos operator/(const FPos& Other) const
	{
		return FPos{ X / Other.X, Y - Other.Y };
	}
};