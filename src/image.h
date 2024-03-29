#include <vector>
#include <string>
using namespace std;

struct Header {
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};

struct Pixel {
    int red;
    int green;
    int blue;
};

class image {
public:
    Header header;
    vector<Pixel> image_data;
    void ReadTGAFile(const string& file_name);
    void WriteTGAFile(const string& file_name);
    image Multiply(image &top_layer, image &multiply);
    image Subtract(image &top_layer, image &subtract);
    static image Screen(image &top_layer, image &screen);
    static image Overlay(image &top_layer, image &overlay);
    image AddByChannel(int addend, int channel);
    image MultiplyByChannel(int factor, int channel);
    tuple<image, image, image> SeparateChannels();
    void CombineChannels(tuple<image, image, image> channels);
    image FlipImage();
};

