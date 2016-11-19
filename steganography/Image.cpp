#include "Image.h"
#include <sstream>
#include <iostream>

const std::string kMagicNumber = "P5";
const char kCommentSymbol = '#';
const int kBufferSize = 256;

enum ParserState
{
    MAGIC_NUMBER,
    SIZES,
    MAX_GREY_VALUE,
    PIXELS
};

Image::Image()
{
}

void Image::Read(const std::string &file_name)
{
    std::ifstream image_stream(file_name, std::ifstream::binary);

    ReadMetadata(image_stream);
    ReadPixels(image_stream);

    image_stream.close();
}

void Image::ReadMetadata(std::ifstream &image_stream)
{
    std::string current_line;
    ParserState state = MAGIC_NUMBER;
    while (state != PIXELS && std::getline(image_stream, current_line))
    {
        if (current_line.empty() || current_line[0] == kCommentSymbol)
        {
            continue;
        }

        switch (state)
        {
        case MAGIC_NUMBER:
        {
            if (current_line == kMagicNumber)
            {
                state = SIZES;
            }
            break;
        }
        case SIZES:
        {
            std::stringstream(current_line) >> width_;
            std::stringstream(current_line) >> height_;
            state = MAX_GREY_VALUE;
            break;
        }
        case MAX_GREY_VALUE:
        {
            std::stringstream(current_line) >> max_grey_value_;
            state = PIXELS;
            break;
        }
        }
    }
}

void Image::ReadPixels(std::ifstream &image_stream)
{
    Image image;
    char buffer[kBufferSize] = { 0 };
    image_stream.read(buffer, kBufferSize);
    while (!image_stream.eof())
    {
        pixels_.insert(pixels_.end(), &buffer[0], &buffer[kBufferSize]);
        std::memset(buffer, 0, kBufferSize);
        image_stream.read(buffer, kBufferSize);
    }
}

void Image::Write(const std::string &file_name) const
{
    std::ofstream image_stream(file_name, std::ios_base::binary);
    image_stream << kMagicNumber << std::endl;
    image_stream << width_ << " " << height_ << std::endl;
    image_stream << max_grey_value_ << std::endl;

    //for (int i = 0; i < width_ * height_; ++i)
    //{
    //    image_stream.write((char *)&pixels_[i], 1);
    //}
    for (int i = 0; i < height_; ++i)
    {
        for (int j = 0; j < width_; ++j)
        {
            BYTE p = get(i, j);
            image_stream.write((char *)&p, 1);
        }
    }
    image_stream.close();
}

void Image::Invert()
{
    for (int i = 0; i < pixels_.size(); ++i)
    {
        pixels_[i] ^= 1;
    }
}

void Image::set(int i, int j, BYTE value)
{
    pixels_[i * width_ + j] = value;
}

BYTE Image::get(int i, int j) const
{
    return pixels_[i * width_ + j];
}

void Image::set(int i, BYTE value)
{
    pixels_[i] = value;
}

BYTE Image::get(int i) const
{
    return pixels_[i];
}

int Image::height() const
{
    return height_;
}

int Image::width() const
{
    return width_;
}


void Image::Print() const
{
    std::ofstream out("output.txt");

    for (int i = 0; i < width_; ++i)
    {
        for (int j = 0; j < height_; ++j)
        {
            out << (int)get(i, j) << " ";
        }
        out << std::endl;
    }
    out << std::endl;
    out.close();
}
