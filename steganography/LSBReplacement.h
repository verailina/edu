#pragma once
#include "Image.h"
#include <random>
#include <chrono>
#include <vector>

class LSBReplacement
{
private:
    static std::vector<int> GetRandomNumbers(std::default_random_engine & generator,int total);
public:
    static void EmbedRandomMessage(Image &image, int message_length);
    static void EmbedMessage(Image &image, int message_length);
};

