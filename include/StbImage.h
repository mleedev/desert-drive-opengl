#ifndef STB_IMAGE_H
#define STB_IMAGE_H

#define STB_IMAGE_IMPLEMENTATION
#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX15.5.sdk/usr/include/c++/v1/string"

class StbImage
{
private:
    int width, height, bpp;
    unsigned char* data = nullptr;

public:
    StbImage();
    ~StbImage();

    void loadFromFile(const std::string& filepath);

    int getWidth() const;
    int getHeight() const;
    int getBpp() const;
    unsigned char* getData() const;
};

#endif // STB_IMAGE_H