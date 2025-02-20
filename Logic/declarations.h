#ifndef DECLARATIONS_H 
#define DECLARATIONS_H

#include <iostream>
#include <vector>
#include <stdlib.h>

struct Plate {
    int X, Y;
    Plate(int _x, int _y) : X(_x), Y(_y) {}
    void getString(char *tmp1) const {
        itoa(X, tmp1, 10);
        strcat(tmp1, " ");
        char tmp2[5];
        itoa(Y, tmp2, 10);
        strcat(tmp1, tmp2);
    }
};

struct Pices {
    int X, Y;
    bool Placed;
    Pices() = default;
    Pices(int _x, int _y) : X(_x), Y(_y), Placed(false) {}
    void print() const {
        printf("Duzina je %d i sirina je %d\n", X, Y);
    }
};

void print(std::vector<Pices>& listOfPices);
void giliotineCutiing(Plate* plate, std::vector<Pices>& listOfPices);
void sortPices(std::vector<Pices>& listOfPices);
void getDimensions(char* msg, int lenght, int *width, int *height);
void pushPices(std::vector<Pices>& listOfPices, char* msg, int lenght);
void sendMsg(char *msg);

#endif