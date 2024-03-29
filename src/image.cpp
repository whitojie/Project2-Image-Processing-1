#include "image.h"
#include <iostream>
#include <fstream>
#include <tuple>

using namespace std;

void image::ReadTGAFile(const string& file_name) {
    Header image_header{};
    ifstream inTGA(file_name, ios::in | ios::binary);
    if (!inTGA.is_open()) {
        cout << "Cannot open file!" << endl;
    } else {
        inTGA.read((char*) &image_header.idLength, 1);
        inTGA.read((char*) &(image_header.colorMapType), 1);
        inTGA.read((char*) &(image_header.dataTypeCode), 1);
        inTGA.read((char*) &(image_header.colorMapOrigin), 2);
        inTGA.read((char*) &(image_header.colorMapLength), 2);
        inTGA.read((char*) &(image_header.colorMapDepth), 1);
        inTGA.read((char*) &(image_header.xOrigin), 2);
        inTGA.read((char*) &(image_header.yOrigin), 2);
        inTGA.read((char*) &(image_header.width), 2);
        inTGA.read((char*) &(image_header.height), 2);
        inTGA.read((char*) &(image_header.bitsPerPixel), 1);
        inTGA.read((char*) &(image_header.imageDescriptor), 1);
    }
    this->header = image_header;

    for (int i = 0; i < (this->header.height * this->header.width); i++) {
        Pixel output_pixel{};
        inTGA.read((char *) &output_pixel.blue, 1);
        inTGA.read((char *) &output_pixel.green, 1);
        inTGA.read((char *) &output_pixel.red, 1);
        this->image_data.push_back(output_pixel);
    }
}

void image::WriteTGAFile(const string& file_name) {
    ofstream outTGA(file_name, ios::out | ios::binary);

    if (!outTGA.is_open()) {
        cout << "Cannot open file!" << endl;
    } else {
        outTGA.clear();
        outTGA.write((char*) &this->header.idLength, 1);
        outTGA.write((char*) &this->header.colorMapType, 1);
        outTGA.write((char*) &this->header.dataTypeCode, 1);
        outTGA.write((char*) &this->header.colorMapOrigin, 2);
        outTGA.write((char*) &this->header.colorMapLength, 2);
        outTGA.write((char*) &this->header.colorMapDepth, 1);
        outTGA.write((char*) &this->header.xOrigin, 2);
        outTGA.write((char*) &this->header.yOrigin, 2);
        outTGA.write((char*) &this->header.width, 2);
        outTGA.write((char*) &this->header.height, 2);
        outTGA.write((char*) &this->header.bitsPerPixel, 1);
        outTGA.write((char*) &this->header.imageDescriptor, 1);

        // iterate through all pixels, write into file
        for (int i = 0; i < (this->header.height * this->header.width); i++ ) {
            outTGA.write((char*) &(this->image_data.at(i).blue), 1);
            outTGA.write((char*) &(this->image_data.at(i).green), 1);
            outTGA.write((char*) &(this->image_data.at(i).red), 1);
        }
    }
    outTGA.close();
}


int Clamp(int input_val) {
    int clamped;
    if (input_val > 255) {
        clamped = 255;
    }
    else if (input_val < 0) {
        clamped = 0;
    }
    else {
        clamped = input_val;
    }

    return clamped;
}


image image::Multiply(image &top_layer, image &multiply) {
    multiply.header = this->header;
    multiply.image_data = this->image_data;
    for (int i = 0; i < multiply.header.width * multiply.header.height; i++) {
        float normalized_red = (float)(top_layer.image_data[i].red) / 255.0f * (float)(this->image_data[i].red) / 255.0f;
        float normalized_green = (float)(top_layer.image_data[i].green) / 255.0f * (float)(this->image_data[i].green) / 255.0f;
        float normalized_blue = (float)(top_layer.image_data[i].blue) / 255.0f * (float)(this->image_data[i].blue) / 255.0f;

        multiply.image_data[i].red = (int)(normalized_red * 255.0f + 0.5f);
        multiply.image_data[i].green = (int)(normalized_green * 255.0f + 0.5f);
        multiply.image_data[i].blue = (int)(normalized_blue * 255.0f + 0.5f);

        multiply.image_data[i].red = Clamp(multiply.image_data[i].red);
        multiply.image_data[i].green = Clamp(multiply.image_data[i].green);
        multiply.image_data[i].blue = Clamp(multiply.image_data[i].blue);
    }
    return multiply;
}

image image::Subtract(image &top_layer, image &subtract) {
    subtract.header = this->header;
    subtract.image_data = this->image_data;
    for(int i = 0; i < subtract.header.width * subtract.header.height; i++){
        subtract.image_data[i].red = (unsigned char)Clamp(((int)(this->image_data[i].red)) - ((int)top_layer.image_data[i].red));
        subtract.image_data[i].green = (unsigned char)Clamp(((int)(this->image_data[i].green)) - ((int)(top_layer.image_data[i].green)));
        subtract.image_data[i].blue = (unsigned char)Clamp(((int)(this->image_data[i].blue))- ((int)(top_layer.image_data[i].blue)));
    }
    return subtract;
}


image image::Screen(image &top_layer, image &screen) {
    screen.header = top_layer.header;
    for (int i = 0; i < screen.header.width * screen.header.height; i++) {
        float red_blend = 1.0f - (1.0f - (float) top_layer.image_data[i].red / 255.0f) *
                                 (1.0f - (float) screen.image_data[i].red / 255.0f);
        float green_blend = 1.0f - (1.0f - (float) top_layer.image_data[i].green / 255.0f) *
                                   (1.0f - (float) screen.image_data[i].green / 255.0f);
        float blue_blend = 1.0f - (1.0f - (float) top_layer.image_data[i].blue / 255.0f) *
                                  (1.0f - (float) screen.image_data[i].blue / 255.0f);


        screen.image_data[i].red = (unsigned char) Clamp(red_blend * 255.0f + 0.5f);
        screen.image_data[i].green = (unsigned char) Clamp(green_blend * 255.0f + 0.5f);
        screen.image_data[i].blue = (unsigned char) Clamp(blue_blend * 255.0f + 0.5f);
    }

    return screen;
}


image image::Overlay(image &top_layer, image &overlay) {
    overlay.header = top_layer.header;

    for (int i = 0; i < overlay.header.width * overlay.header.height; i++) {
        float np1_red = (float)top_layer.image_data[i].red / 255.0f;
        float np2_red = (float)overlay.image_data[i].red / 255.0f;

        // Red channel blending
        float blended_red;
        if (np2_red <= 0.5f) {
            blended_red = 2.0f * np1_red * np2_red;
        } else {
            blended_red = 1.0f - 2.0f * (1.0f - np1_red) * (1.0f - np2_red);
        }

        float clamped_blended_red = Clamp(blended_red * 255.0f + 0.5f);

        // Green channel blending
        float np1_green = (float)top_layer.image_data[i].green / 255.0f;
        float np2_green = (float)overlay.image_data[i].green / 255.0f;

        float blended_green;
        if (np2_green <= 0.5f) {
            blended_green = 2.0f * np1_green * np2_green;
        } else {
            blended_green = 1.0f - 2.0f * (1.0f - np1_green) * (1.0f - np2_green);
        }

        float clamped_blended_green = Clamp(blended_green * 255.0f + 0.5f);

        // Blue channel blending
        float np1_blue = (float)top_layer.image_data[i].blue / 255.0f;
        float np2_blue = (float)overlay.image_data[i].blue / 255.0f;

        float blended_blue;
        if (np2_blue <= 0.5f) {
            blended_blue = 2.0f * np1_blue * np2_blue;
        } else {
            blended_blue = 1.0f - 2.0f * (1.0f - np1_blue) * (1.0f - np2_blue);
        }

        float clamped_blended_blue = Clamp(blended_blue * 255.0f + 0.5f);

        // Update overlay pixel values
        overlay.image_data[i].red = (unsigned char)clamped_blended_red;
        overlay.image_data[i].green = (unsigned char)clamped_blended_green;
        overlay.image_data[i].blue = (unsigned char)clamped_blended_blue;
    }

    return overlay;
}


image image::AddByChannel(int addend, int channel) {
    image added;
    added.header = this->header;
    added.image_data = this->image_data;

    for (int i = 0; i < (this->header.width) * (this->header.height); i++) {
        // add to the blue channel
        if (channel == 0) {
            added.image_data.at(i).blue = Clamp(this->image_data.at(i).blue + addend);
            added.image_data.at(i).green = this->image_data.at(i).green;
            added.image_data.at(i).red = this->image_data.at(i).red;
            // add to the green channel
        } else if (channel == 1) {
            added.image_data.at(i).blue = this->image_data.at(i).blue;
            added.image_data.at(i).green = Clamp(this->image_data.at(i).green + addend);
            added.image_data.at(i).red = this->image_data.at(i).red;
            // add to the red channel
        } else if (channel == 2) {
            added.image_data.at(i).blue = this->image_data.at(i).blue;
            added.image_data.at(i).green = this->image_data.at(i).green;
            added.image_data.at(i).red = Clamp(this->image_data.at(i).red + addend);
        }
    }
    return added;
}


image image::MultiplyByChannel(int factor, int channel) {
    image multiplied;
    multiplied.header = this->header;
    multiplied.image_data = this->image_data;
    // iterate through all pixels
    for (int i = 0; i < (this->header.width) * (this->header.height); i++) {
        // multiply the blue channel
        if (channel == 0) {
            multiplied.image_data.at(i).blue = Clamp(this->image_data.at(i).blue * factor);
            multiplied.image_data.at(i).green = this->image_data.at(i).green;
            multiplied.image_data.at(i).red = this->image_data.at(i).red;
        } else if (channel == 1) {
            multiplied.image_data.at(i).blue = this->image_data.at(i).blue;
            multiplied.image_data.at(i).green = Clamp(this->image_data.at(i).green * factor);
            multiplied.image_data.at(i).red = this->image_data.at(i).red;
            // multiply the red channel, copy the others
        } else if (channel == 2) {
            multiplied.image_data.at(i).blue = this->image_data.at(i).blue;
            multiplied.image_data.at(i).green = this->image_data.at(i).green;
            multiplied.image_data.at(i).red = Clamp(this->image_data.at(i).red * factor);
        }
    }
    return multiplied;
}



tuple<image, image, image> image::SeparateChannels() {
    image blue_channel ;
    blue_channel.header = this->header ;
    blue_channel.image_data = this->image_data ;

    image green_channel ;
    green_channel.header = this->header ;
    green_channel.image_data = this->image_data ;

    image red_channel ;
    red_channel.header = this->header ;
    red_channel.image_data = this->image_data ;

    for (int i = 0; i < (this->header.width) * (this->header.height); i++) {
        blue_channel.image_data.at(i).blue = this->image_data.at(i).blue ;
        blue_channel.image_data.at(i).green = this->image_data.at(i).blue ;
        blue_channel.image_data.at(i).red = this->image_data.at(i).blue ;

        green_channel.image_data.at(i).blue = this->image_data.at(i).green ;
        green_channel.image_data.at(i).green = this->image_data.at(i).green ;
        green_channel.image_data.at(i).red = this->image_data.at(i).green ;

        red_channel.image_data.at(i).blue = this->image_data.at(i).red ;
        red_channel.image_data.at(i).green = this->image_data.at(i).red ;
        red_channel.image_data.at(i).red = this->image_data.at(i).red ;
    }
    return {blue_channel, green_channel, red_channel} ;
}



void image::CombineChannels(tuple<image, image, image> channels) {
    this->header = get<0>(channels).header ;
    this->image_data = get<0>(channels).image_data ;
    for (int i = 0; i < (this->header.width) * (this->header.height); i++) {
        this->image_data.at(i).blue = get<0>(channels).image_data.at(i).blue ;
        this->image_data.at(i).green = get<1>(channels).image_data.at(i).green ;
        this->image_data.at(i).red = get<2>(channels).image_data.at(i).red ;
    }
}

image image::FlipImage() {
    image flipped;
    flipped.header = this->header;
    flipped.image_data = this->image_data;
    int j = (this->header.width) * (this->header.height) - 1;
    vector<Pixel> tempPixels;
    int count;
    for (int i = 0; i < (this->header.width) * (this->header.height); i++) {

        flipped.image_data.at(j).blue = this->image_data.at(i).blue;
        flipped.image_data.at(j).green = this->image_data.at(i).green;
        flipped.image_data.at(j).red = this->image_data.at(i).red;

        j--;
    }
    return flipped;
}