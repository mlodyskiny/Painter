#include "Painter.h"

#include <cmath>
#include <thread>

#include <cpr/cpr.h>
#include "Log.h"

#undef max // FUCK YOU BILL GATES

FColor NPainter::FindNearestColor(FColor Color)
{
    double MinDistance = std::numeric_limits<double>::max();
    FColor NearestColor{};

    for (const auto& UsableColor : ColorPalette)
    {
        double Distance = std::pow(Color.R - UsableColor.R, 2) +
            std::pow(Color.G - UsableColor.G, 2) +
            std::pow(Color.B - UsableColor.B, 2);

        if (Distance < MinDistance)
        {
            MinDistance = Distance;
            NearestColor = UsableColor;
        }
    }

    return NearestColor;
}

NPainter::NPainter(const std::string& Cookie) : Cookie(Cookie)
{
    if (Cookie.empty())
    {
        throw std::runtime_error("Cookie cannot be empty!");
    }

    if (!Draw({ 0, 0 }, ColorPalette[0]))
    {
        throw std::runtime_error("Couldn't place a test pixel! The cookie is invalid, you're on cooldown or you're banned!");
    }
}

void NPainter::DrawList(const std::vector<std::pair<FColor, FPos>>& DrawPositions, const FPos& StartPos)
{
    constexpr auto SleepTime = std::chrono::milliseconds(3100);

    for (size_t i = 0; i < DrawPositions.size(); ++i)
    {
        const FColor Color = FindNearestColor(DrawPositions[i].first);
        const FPos& Position = DrawPositions[i].second;
        const FPos PlacePosition = StartPos + Position;
        const bool Result = Draw(PlacePosition, Color);

        if (Result)
        {
            Log(Info, L"Placed pixel at {}x{} (off: {}x{}, col: {}, {}, {})", PlacePosition.X, PlacePosition.Y, Position.X, Position.Y, Color.R, Color.G, Color.B);
        }
        else
        {
            Log(Error, L"Couldn't place pixel!");
            --i;
        }

        std::this_thread::sleep_for(SleepTime);
    }
}

bool NPainter::Draw(const FPos& Position, FColor Color)
{
    const std::string Body = "{" + std::format(R"("x":{},"y":{},"color":"{}")", Position.X, Position.Y, Color.ToHex()) + "}";
    const auto Response = cpr::Post(
        cpr::Url{ "https://canvas.osucord.moe/api/place" },
        cpr::Header
        {
            { "accept", "application/json" },
            { "accept-encoding", "gzip, deflate, br, zstd" },
            { "accept-language", "pl-PL,pl;q=0.9,en-US;q=0.8,en;q=0.7" },
            { "origin", "https://canvas.osucord.moe" },
            { "priority", "u=1, i" },
            { "referer", "https://canvas.osucord.moe/" },
            { "sec-ch-ua", R"("Not(A:Brand";v="99", "Google Chrome";v="133", "Chromium";v="133")" },
            { "sec-ch-ua-mobile", "?0" },
            { "sec-ch-ua-platform", R"("Windows")" },
            { "sec-fetch-dest", "empty" },
            { "sec-fetch-mode", "cors" },
            { "sec-fetch-site", "same-origin" },
            { "Content-Type", "application/json" }
        },
        cpr::UserAgent{ "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/134.0.0.0 Safari/537.36" },
        cpr::Cookie{ "nuxt-session", Cookie, "canvas.osucord.moe", false, "/", true },
        cpr::Body{ Body }
    );

#if defined(_DEBUG)
    Log(
        Info, 
        L"cookie: {}, body: {}, resp: {} - {}", 
        std::wstring(Cookie.begin(), Cookie.end()), 
        std::wstring(Body.begin(), Body.end()), 
        Response.status_code, 
        std::wstring(Response.text.begin(), Response.text.end())
    );
#endif
    return Response.status_code == 200;
}
