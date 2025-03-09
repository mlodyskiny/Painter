#pragma once

#include <array>
#include <vector>

#include "Color.h"
#include "Pos.h"

class NPainter
{
private:
	static constexpr std::array<FColor, 33> ColorPalette
	{
		FColor(0, 0, 0),
		FColor(82, 82, 82),
		FColor(136, 141, 144),
		FColor(213, 214, 216),
		FColor(255, 255, 255),
		FColor(107, 1, 25),
		FColor(189, 0, 55),
		FColor(255, 69, 0),
		FColor(254, 168, 0),
		FColor(255, 212, 53),
		FColor(254, 248, 137),
		FColor(1, 162, 103),
		FColor(9, 204, 118),
		FColor(126, 236, 87),
		FColor(2, 117, 109),
		FColor(0, 157, 170),
		FColor(0, 204, 190),
		FColor(36, 79, 164),
		FColor(55, 144, 234),
		FColor(82, 232, 243),
		FColor(72, 57, 191),
		FColor(105, 91, 255),
		FColor(148, 179, 255),
		FColor(128, 29, 159),
		FColor(180, 73, 191),
		FColor(228, 171, 253),
		FColor(221, 17, 126),
		FColor(254, 55, 129),
		FColor(254, 153, 169),
		FColor(109, 70, 47),
		FColor(155, 105, 38),
		FColor(254, 180, 112),
		FColor(244, 206, 190)
	};

	static FColor FindNearestColor(FColor Color);

public:
	NPainter() = delete;
	NPainter(const std::string& Cookie);

	void DrawList(const std::vector<std::pair<FColor, FPos>>& DrawPositions, const FPos& StartPos);

private:
	bool Draw(const FPos& Position, FColor Color);

	std::string Cookie{};
};