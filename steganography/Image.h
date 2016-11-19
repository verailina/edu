#pragma once
#include <vector>
#include <string>
#include <fstream>

typedef unsigned char BYTE;

class Image
{
public:
    Image();

    void Invert();
    void Read(const std::string &file_name);
    void Write(const std::string &file_name) const;
    void set(int i, int j, BYTE value);
    BYTE get(int i, int j) const;

    void set(int i, BYTE value);
    BYTE get(int i) const;

    int height() const;
    int width() const;

    void Print() const;

private:

    std::vector<BYTE> pixels_;
    int width_;
    int height_;
    int max_grey_value_;

    void ReadMetadata(std::ifstream &image_stream);
    void ReadPixels(std::ifstream &image_stream);
};




