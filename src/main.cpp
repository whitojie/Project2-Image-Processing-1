#include "image.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <tuple>

using namespace std;

bool CheckInFilePath(string& filePath) {
    ifstream inFile(filePath, ios::in | ios::binary);
    return inFile.is_open();
}

bool CheckOutFilePath(string& filePath){
    ofstream outFile(filePath, ios::in | ios::binary);
    return outFile.is_open();
}



int StringToInt(string str) {
    int integer = -1;
    try {
        integer = stoi(str);
    } catch (invalid_argument) {
        cout << "Invalid argument, expected number." << endl;
    }
    return integer;
}


int main(int argc, char* argv[]) {
    int blue = 0;
    int green = 1;
    int red = 2;

    vector<string> args;

    for (int i = 1; i < argc; i++) {
        args.push_back(string(argv[i]));
    }

    if (args.size() == 0) {
        cout << "Project 2: Image Processing, Spring 2024" << endl;
        cout << "\nUsage:" << endl;
        cout << "\t./project2.out [output] [firstImage] [method] [...]" << endl;
        return 0;
    } else if (string(args[0]) == "--help") {
        cout << "Project 2: Image Processing, Spring 2024" << endl;
        cout << "\nUsage:" << endl;
        cout << "\t./project2.out [output] [firstImage] [method] [...]" << endl;
        return 0;
    }


    if (args.size() == 1) {
        string output_file_path = args.at(0);
        if (!CheckOutFilePath(output_file_path)) {
            cout << "Invalid file name." << endl;
            return 0;
        }

        string input_file_path = args.at(1);
        if (!CheckInFilePath(input_file_path)) {
            cout << "File does not exist." << endl;
            return 0;
        }
    }

    if (args.size() > 1) {
        string input_file_path = args.at(1);
        if (input_file_path.size() >= 4 && (input_file_path.substr(input_file_path.size() - 4) != ".tga")) {
            cout << "Invalid file name." << endl;
            return 0;
        }

        if (!CheckInFilePath(input_file_path)) {
            cout << "File does not exist." << endl;
            return 0;
        }

        string output_file_path = args.at(0);
        if (output_file_path.size() >= 4 && output_file_path.substr(output_file_path.size() - 4) != ".tga") {
            cout << "Invalid file name." << endl;
            return 0;
        }

        if (!CheckInFilePath(input_file_path)) {
            cout << "File does not exist." << endl;
            return 0;
        }
    }



    image track;
    track.ReadTGAFile(args[1]);

    for (int i = 2; i < args.size(); i++) {
        cout << "beginning " << i << endl;
        if (args[i] == "multiply") {
            if (args.size() < 4) {
                cout << "Missing argument." << endl;
                return 0;
            }
            cout << "In multiply" << endl;
            image layer1;
            image circles;
            if (i == 2){
                cout << "Multiply 2 " << i << endl;
                layer1.ReadTGAFile(args[1]);
                circles.ReadTGAFile(args[3]);

                track = circles.Multiply(layer1, track);
                track.WriteTGAFile(args[0]);
                i++;
            }
            else {
                cout << "Multiply not 2 " << i << endl;
                layer1.ReadTGAFile(args[0]);
                circles.ReadTGAFile(args[i + 1]);

                track = circles.Multiply(layer1, track);
                track.WriteTGAFile(args[0]);
                i++;
            }



            cout << "Out multiply" << endl;

        }
        else if (args[i] == "flip") {
            cout << "In flip" << endl;
            image layer1;
            if(i == 2){
                cout << "Flip 2 " << i << endl;
                layer1.ReadTGAFile(args[1]);
                track = layer1.FlipImage();
                track.WriteTGAFile(args[0]);
            }
            else {
                cout << "Flip not 2 " << i << endl;
                layer1.ReadTGAFile(args[0]);
                track = layer1.FlipImage();
                track.WriteTGAFile(args[0]);
            }

            cout << "out flip" << endl;
            cout << "After flipping" << i << endl;

        }
        else if (args[i] == "subtract") {
            cout << "In subtract" << endl;
            if (i == 2){
                image layer1;
                layer1.ReadTGAFile(args[1]);
                image layer2;
                layer2.ReadTGAFile(args[3]);

                track = layer2.Subtract(layer1, track);
                track.WriteTGAFile(args[0]);
                i++;
            }
            else{
                image layer1;
                layer1.ReadTGAFile(args[i + 1]);
                image layer2;
                layer2.ReadTGAFile(args[0]);

                track = layer2.Subtract(layer1, track);
                i++;
            }

            cout << "Out subtract" << endl;
        }
        else if (args[i] == "overlay") {
            cout << "In overlay" << endl;
            if(i == 2) {
                image layer1;
                layer1.ReadTGAFile(args[1]);
                image pattern1;
                pattern1.ReadTGAFile(args[3]);

                track = image::Overlay(layer1, pattern1);
                track.WriteTGAFile(args[0]);
                i++;
            }
            else{
                cout << "In overlay" << endl;
                image layer1;
                layer1.ReadTGAFile(args[0]);
                image pattern1;
                pattern1.ReadTGAFile(args[i+1]);

                track = image::Overlay(layer1, pattern1);
                track.WriteTGAFile(args[0]);
                i++;
            }

            cout << "Out overlay" << endl;
        }
        else if (args[i] == "scalered") {
            cout << "In scale red" << endl;
            int factor = StringToInt(args.at(i + 1));
            if (factor == -1) {
                return 0;
            }

            if (i == 2) {
                image scalered;
                scalered.ReadTGAFile(args[1]);
                track = track.MultiplyByChannel(factor, red);
                track.WriteTGAFile(args[0]);
                i++;
                cout << "Out scale blue" << endl;
            }
            else{
                image scalered;
                scalered.ReadTGAFile(args[0]);
                track = track.MultiplyByChannel(factor, red);
                track.WriteTGAFile(args[0]);
                i++;
                cout << "Out scale red" << endl;
            }
        }
        else if (args[i] == "scalegreen") {
            cout << "In scale green" << endl;
            int factor = StringToInt(args.at(i + 1));
            if (factor == -1) {
                return 0;
            }

            if (i == 2) {
                image scalegreen;
                scalegreen.ReadTGAFile(args[1]);
                track = track.MultiplyByChannel(factor, green);
                track.WriteTGAFile(args[0]);
                i++;
                cout << "Out scale green" << endl;
            }
            else{
                image scalegreen;
                scalegreen.ReadTGAFile(args[0]);
                track = track.MultiplyByChannel(factor, green);
                track.WriteTGAFile(args[0]);
                i++;
                cout << "Out scale green" << endl;
            }

        }
        else if (args[i] == "scaleblue") {
            if (args.size() < 4) {
                cout << "Missing argument." << endl;
                return 0;
            }

            int factor = StringToInt(args.at(i + 1));
            if (factor == -1) {
                return 0;
            }

            cout << "In scale blue" << endl;
            if (i == 2) {
                image scaleblue;
                scaleblue.ReadTGAFile(args[1]);
                track = track.MultiplyByChannel(factor, blue);
                track.WriteTGAFile(args[0]);
                i++;
                cout << "Out scale blue" << endl;
            }
            else{
                image scaleblue;
                scaleblue.ReadTGAFile(args[0]);
                track = track.MultiplyByChannel(factor, blue);
                track.WriteTGAFile(args[0]);
                i++;
                cout << "Out scale blue" << endl;
            }
        }
        else if (args[i] == "screen") {
            cout << "In screen" << endl;
            if (i==2){
                image screenPic;
                image pattern2;
                pattern2.ReadTGAFile(args[1]);
                screenPic.ReadTGAFile(args[3]);

                track = screenPic.Screen(screenPic, track);
                track.WriteTGAFile(args[0]);
            }
            else{
                image screenPic;
                image pattern2;
                pattern2.ReadTGAFile(args[0]);
                screenPic.ReadTGAFile(args[i+1]);
                track = screenPic.Screen(screenPic, track);
                track.WriteTGAFile(args[0]);
            }


            i++;
            cout << "Out screen" << endl;

        }
        else if (args[i] == "combine") {
            cout << "In combine" << endl;
            image layer_blue;
            layer_blue.ReadTGAFile(args[i + 2]);

            image layer_green;

            layer_green.ReadTGAFile(args[i + 1]);

            track.ReadTGAFile(args[0]);

            tuple<image, image, image> input_channels = {layer_blue, layer_green, track};
            track.CombineChannels(input_channels);
            track.WriteTGAFile(args[0]);


            i += 2;
            cout << "Out combine" << endl;
        }
        else if (args[i] == "addred"){
            cout << "In addred" << endl;
            int addend = StringToInt(args.at(i + 1)) ;
            if (addend == -1) {
                return 0;
            }

            if(i == 2){
                image part18;
                part18.ReadTGAFile(args[1]);
                track = part18.AddByChannel(addend, red);
                track.WriteTGAFile(args[0]);
                i++;
                cout << "out addred" << endl;
            }
            else{
                image part18;
                part18.ReadTGAFile(args[0]);
                track = part18.AddByChannel(addend, red);
                track.WriteTGAFile(args[0]);
                i++;
                cout << "out addred" << endl;
            }
        }
        else if (args[i] == "addblue"){
            cout << "In addblue" << endl;
            int addend = StringToInt(args.at(i + 1)) ;
            if (addend == -1) {
                return 0;
            }

            if(i == 2){
                image part18;
                part18.ReadTGAFile(args[1]);
                track = part18.AddByChannel(addend, blue);
                track.WriteTGAFile(args[0]);
                i++;
                cout << "Out addblue" << endl;
            }
            else{
                image part18;
                part18.ReadTGAFile(args[0]);
                track = part18.AddByChannel(addend, blue);
                track.WriteTGAFile(args[0]);
                i++;
                cout << "Out addblue" << endl;
            }

        }
        else if (args[i] == "addgreen"){
            cout << "In addgreen" << endl;

            int addend = StringToInt(args.at(i + 1)) ;
            if (addend == -1) {
                return 0;
            }
            if(i == 2){
                image part18;
                part18.ReadTGAFile(args[1]);
                track = part18.AddByChannel(addend, green);
                track.WriteTGAFile(args[0]);
                i++;
                cout << "Out addgreen" << endl;
            }
            else{
                image part18;
                part18.ReadTGAFile(args[0]);
                track = part18.AddByChannel(addend, green);
                track.WriteTGAFile(args[0]);
                i++;
                cout << "Out addgreen" << endl;
            }

        }
        else if(args[i] == "onlyred"){
            cout << "In onlyred" << endl;

            if (i == 2){
                image car;
                car.ReadTGAFile(args[1]);

                tuple<image, image, image> separated_channels = car.SeparateChannels();

                track = get<2>(separated_channels);
                track.WriteTGAFile(args[0]);

                cout << "Out onlyred" << endl;
                i++;
            }
            else{
                image car;
                car.ReadTGAFile(args[0]);

                tuple<image, image, image> separated_channels = car.SeparateChannels();

                track = get<2>(separated_channels);
                track.WriteTGAFile(args[0]);

                cout << "Out onlyred" << endl;
                i++;
            }

        }
        else if(args[i] == "onlygreen"){
            cout << "In onlygreen" << endl;
            if (i == 2){
                image car;
                car.ReadTGAFile(args[1]);

                tuple<image, image, image> separated_channels = car.SeparateChannels();
                track = get<1>(separated_channels);
                track.WriteTGAFile(args[0]);
                i++;
            }
            else{
                image car;
                car.ReadTGAFile(args[0]);

                tuple<image, image, image> separated_channels = track.SeparateChannels();
                track = get<1>(separated_channels);
                track.WriteTGAFile(args[0]);

                i++;
            }
            cout << "Out onlygreen" << endl;
        }
        else if(args[i] == "onlyblue"){
            cout << "In onlyblue" << endl;
            if (i == 2){
                image car;
                car.ReadTGAFile(args[1]);

                tuple<image, image, image> separated_channels = car.SeparateChannels();
                track = get<0>(separated_channels);
                track.WriteTGAFile(args[0]);
                i++;
            }
            else{
                image car;
                car.ReadTGAFile(args[0]);

                tuple<image, image, image> separated_channels = car.SeparateChannels();
                track = get<0>(separated_channels);
                track.WriteTGAFile(args[0]);

                i++;
            }
            cout << "Out onlyblue" << endl;
        }
        else {
            cout << "Invalid method name." << endl;
            return 0;
        }
    }
    track.WriteTGAFile(args[0]);
    return 0;

}
