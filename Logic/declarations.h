#ifndef DECLARATIONS_H 
#define DECLARATIONS_H

#include <iostream>
#include <vector>
#include <stdlib.h>

#include <stdlib.h>
#include <stdio.h>

struct Pices {
    int Width, Height;
    Pices(int width, int height) : Width(width), Height(height) {}
};

struct Plate {
    int Width, Height;
    Plate(int width, int height) : Width(width), Height(height) {}
    void getString(char* buffer) {
        sprintf(buffer, "%d %d", Width, Height);
    }
};

// Constants
const int g_next_right = 1;
const int g_next_down = 2;

// Structs
typedef struct rect_t {
    int X, Y, Width, Height;
} rect_t;

typedef struct pice_t {
    int X, Y;
    int Width, Height;
    bool Set;
} pice_t;

// Variables
int MAX_AREA = 0;
int FULL_AREA = 0;
int PICES_NUM;
int WIDTH;
int HEIGHT;

// Declarations
void algo(pice_t *pices, int depth, rect_t *free_pos, int count);
int get_area(pice_t pice);
void print(pice_t *array);
void print_all(pice_t *array);
void print_pos(rect_t *free_pos, int count);
void rotate(pice_t *pice);
bool is_square(pice_t pice);
void sort();
void copy_to_array(pice_t *copy_to, pice_t *copy_from);
void copy_to_pos(rect_t *copy_to, rect_t* copy_from);
bool out_of_bounds(rect_t free_pos, pice_t pice);
int get_full_area(pice_t *pices_array, int count);
void sort_array(pice_t *array, int count);
void max_area(pice_t *pices_array);
bool is_pices_equa(pice_t pice1, pice_t pice2);
int evaluate(pice_t *pices_array);
void make_string(char* buffer, pice_t pice);
void getDimensions(char* msg, int lenght, int *width, int *height);
void pushPices(std::vector<Pices>& listOfPices, char* msg, int lenght);
void sendMsg(char *msg);

#endif