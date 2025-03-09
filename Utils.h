#pragma once

#include <vector>

namespace Utils
{
    template <typename T>
    std::vector<std::vector<T>> SplitVector(const std::vector<T>& Input, size_t NumParts)
    {
        std::vector<std::vector<T>> Result;
        size_t ChunkSize = Input.size() / NumParts;
        size_t Remainder = Input.size() % NumParts;

        auto Iterator = Input.begin();
        for (size_t i = 0; i < NumParts; ++i)
        {
            size_t CurrentSize = ChunkSize + (i < Remainder ? 1 : 0);
            Result.emplace_back(Iterator, Iterator + CurrentSize);
            Iterator += CurrentSize;
        }

        return Result;
    }
}