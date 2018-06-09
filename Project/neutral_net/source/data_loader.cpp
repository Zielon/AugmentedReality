#include <iostream>
#include <fstream>
#include "../headers/data_loader.h"

using namespace std;

int DataLoader::reverseInt(int i) {
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 255;
    ch2 = (i >> 8) & 255;
    ch3 = (i >> 16) & 255;
    ch4 = (i >> 24) & 255;
    return ((int) ch1 << 24) + ((int) ch2 << 16) + ((int) ch3 << 8) + ch4;
}

vector<Digit *> DataLoader::load(string images, string labels, unsigned long size) {
    vector<vector<int>> rawImages = loadRawImages(images, size);
    vector<int> rawLabels = loadRawLabels(labels, size);
    return mapToDigit(rawImages, rawLabels);
}

vector<vector<int>> DataLoader::loadRawImages(string path, unsigned long images) {
    vector<vector<int>> array(images, vector<int>(784));

    ifstream file(path, ios::binary);

    if (file.is_open()) {
        int magic_number = 0;
        int number_of_images = 0;
        int n_rows = 0;
        int n_cols = 0;
        file.read((char *) &magic_number, sizeof(magic_number));
        magic_number = reverseInt(magic_number);
        file.read((char *) &number_of_images, sizeof(number_of_images));
        number_of_images = reverseInt(number_of_images);
        file.read((char *) &n_rows, sizeof(n_rows));
        n_rows = reverseInt(n_rows);
        file.read((char *) &n_cols, sizeof(n_cols));
        n_cols = reverseInt(n_cols);
        for (int i = 0; i < number_of_images; ++i) {
            for (int r = 0; r < n_rows; ++r) {
                for (int c = 0; c < n_cols; ++c) {
                    unsigned char temp = 0;
                    file.read((char *) &temp, sizeof(temp));
                    array[i][(n_rows * r) + c] = (int) temp;
                }
            }
        }

        return array;

    } else {
        throw runtime_error("Unable to open file `" + path + "`!");
    }
}

vector<Digit *> DataLoader::mapToDigit(vector<vector<int>> images, vector<int> labels) {
    vector<Digit *> output;
    for (int i = 0; i < images.size(); i++) {
        output.emplace_back(new Digit(images[i], labels[i]));
    }
    return output;
}

vector<int> DataLoader::loadRawLabels(string path, unsigned long labels) {
    ifstream file(path, ios::binary);
    if (file.is_open()) {
        int magic_number = 0;
        file.read((char *) &magic_number, sizeof(magic_number));
        magic_number = reverseInt(magic_number);

        file.read((char *) &labels, sizeof(labels)), labels = reverseInt(labels);

        vector<int> output(labels);
        for (int i = 0; i < labels; i++) file.read((char *) &output[i], 1);

        return output;

    } else {
        throw runtime_error("Unable to open file `" + path + "`!");
    }
}
