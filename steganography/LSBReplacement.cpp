#include "LSBReplacement.h"
#include <random>
#include <set>
#include <algorithm>

void LSBReplacement::EmbedRandomMessage(Image &image, int message_length)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    std::uniform_int_distribution<int> pixels_distribution(0, image.width() * image.height());
    std::uniform_int_distribution<int> bits_distribution(0, 1);

    std::set<int> changed_pixels;

    for (int i = 0; i < message_length; ++i)
    {
        int number = pixels_distribution(generator) % (image.width() * image.height());
        while (changed_pixels.count(number) != 0)
        {
            number = pixels_distribution(generator) % (image.width() * image.height());
        }
        BYTE pixel = image.get(number) ^ bits_distribution(generator);
        image.set(number, pixel);
        changed_pixels.insert(number);
    }
}

void LSBReplacement::EmbedMessage(Image &image, int message_length)
{    
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    int total = image.width() * image.height();
    std::vector<int> positions = GetRandomNumbers(generator, total);
    std::uniform_int_distribution<int> bits_distribution(0, 1);

    std::set<int> changed_pixels;

    for (int i = 0; i < message_length; ++i)
    {
        BYTE pixel = image.get(positions[i]) ^ bits_distribution(generator);
        image.set(positions[i], pixel);
    }
}

std::vector<int> LSBReplacement::GetRandomNumbers(std::default_random_engine & generator, int total)
{
    std::uniform_int_distribution<int> numbers_distribution(0, total - 1);
    std::vector<int> all_numbers(total);
    for (int i = 0; i < total; ++i)
    {
        all_numbers[i] = i;
    }

    for (int i = 0; i < total; ++i)
    {
        int j = numbers_distribution(generator);
        std::swap(all_numbers[i], all_numbers[j]);
    }
    return all_numbers;
}
