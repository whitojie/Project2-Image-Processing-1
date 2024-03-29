#include "image.h"
#include <tuple>
using namespace std;
void taskOne(){
        image layer1;
        layer1.ReadTGAFile("input/layer1.tga");
        image pattern1;
        pattern1.ReadTGAFile("input/pattern1.tga");

        image task1;
        pattern1.Multiply(layer1,task1);
        task1.WriteTGAFile("output/part1.tga");
}

void taskTwo(){
    image layer2;
    layer2.ReadTGAFile("input/layer2.tga");
    image car;
    car.ReadTGAFile("input/car.tga");
    image task2;

    car.Subtract(layer2,task2);
    task2.WriteTGAFile("output/part2.tga");
}

void taskThree(){
    image layer1;
    layer1.ReadTGAFile("input/layer1.tga");
    image pattern2;
    pattern2.ReadTGAFile("input/pattern2.tga");
    image text;
    text.ReadTGAFile("input/text.tga");

    image task3;
    image screened;
    screened = pattern2.Multiply(layer1,task3);
    task3 = image::Screen(text, screened);
    task3.WriteTGAFile("output/part3.tga");

}

void taskFour(){
    image layer2;
    layer2.ReadTGAFile("input/layer2.tga");
    image pattern2;
    pattern2.ReadTGAFile("input/pattern2.tga");
    image circles;
    circles.ReadTGAFile("input/circles.tga");

    image subtracted;
    subtracted = circles.Multiply(layer2, circles);
    image task4 = subtracted.Subtract(pattern2, subtracted);
    task4.WriteTGAFile("output/part4.tga");

}

void taskFive(){
    image layer1;
    layer1.ReadTGAFile("input/layer1.tga");
    image pattern1;
    pattern1.ReadTGAFile("input/pattern1.tga");

    image task5;
    task5 = task5.Overlay(layer1, pattern1);
    task5.WriteTGAFile("output/part5.tga");

}

void taskSix(){
    image car;
    car.ReadTGAFile("input/car.tga");

    image task6;
    task6 = car.AddByChannel(200, 1);
    task6.WriteTGAFile("output/part6.tga");

}

void taskSeven(){
    image car;
    car.ReadTGAFile("input/car.tga");

    image task7;
    image multi;
    multi = car.MultiplyByChannel(4, 2);
    task7 = multi.MultiplyByChannel(0, 0);
    task7.WriteTGAFile("output/part7.tga");

}

void taskEight() {
    image car;
    car.ReadTGAFile("input/car.tga");

    std::tuple<image, image, image> output_channels = car.SeparateChannels();


    image blue_channel = std::get<0>(output_channels);
    image green_channel = std::get<1>(output_channels);
    image red_channel = std::get<2>(output_channels);

    red_channel.WriteTGAFile("./output/part8_r.tga");
    green_channel.WriteTGAFile("./output/part8_g.tga");
    blue_channel.WriteTGAFile("./output/part8_b.tga");
}

void taskNine(){
    image layer_blue;
    layer_blue.ReadTGAFile("input/layer_blue.tga");
    image layer_green;
    layer_green.ReadTGAFile("input/layer_green.tga");
    image layer_red;
    layer_red.ReadTGAFile("input/layer_red.tga");

    image combined;
    tuple<image, image, image> input_channels = {layer_blue, layer_green, layer_red};
    combined.CombineChannels(input_channels);
    combined.WriteTGAFile("./output/part9.tga");
}

void taskTen(){
    image text2;
    text2.ReadTGAFile("input/text2.tga");

    image flipped;
    flipped = text2.FlipImage();
    flipped.WriteTGAFile("./output/part10.tga");

}

int main() {
    taskOne();
    taskTwo();
    taskThree();
    taskFour();
    taskFive();
    taskSix();
    taskSeven();
    taskEight();
    taskNine();
    taskTen();

    return 0;
}
