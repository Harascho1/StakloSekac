
#include <limits.h>
#include <stdbool.h>

typedef struct pice_t {
    int Width, Height;
    char Set = 0;
    struct pice_t *next;
} pice_t;

typedef struct rect_t {
    int X, Y, Width, Height;
} rect_t;

pice_t *array;
int pices;

int is_square(pice_t rect) {
    return rect.Width == rect.Height;
}

int evaluate(pice_t rect) {
    return rect.Width * rect.Height;
}

void succsesors_pice(pice_t* rect, pice_t *succ, int *count) {
    rect->Set = 1;
    *count = 0;
    for (int i = 0; i < pices; i++) {
        if (array[i].Set == 0) { // nije postavljena
            succ[*(count)++] = array[i];
        }
    }
}

void succesors_free_rect(rect_t rect, rect_t *free_rect, int *count) {
    
}
