#include "Color.h"

#include <sstream>
#include <format>

FColor FColor::FromHex(const std::string& Color)
{
    auto HexToByte = [](const std::string& Hex)
    {
        int Byte;
        std::istringstream(Hex) >> std::hex >> Byte;
        return static_cast<unsigned char>(Byte);
    };

    if (Color.size() != 7 || Color[0] != '#')
    {
        throw std::runtime_error("Color must be provided in hex notation like this: #FF0000");
    }

    return FColor
    {
        HexToByte(Color.substr(1, 2)),
        HexToByte(Color.substr(3, 2)),
        HexToByte(Color.substr(5, 2))
    };
}

std::string FColor::ToHex() const
{
    return std::format("#{:02X}{:02X}{:02X}", R, G, B);
}
