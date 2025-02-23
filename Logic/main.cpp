// MAIN LOGIC

#include "declarations.h"
#include <fstream>
#include <sstream>
#include <algorithm>

pice_t *pices_array = NULL;
pice_t *final_array = NULL;
bool initializePlate = false;
HANDLE hPipe;

int main() {
    Plate* plate;
    std::vector<Pices> listOfPices;

    ConnectPipe(&hPipe);

    char buffer[1024];
    DWORD bytesRead, bytesWritten;
    while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
        buffer[bytesRead] = '\0';
        WriteFile(hPipe, "RECEIVED", strlen("RECEIVED\0"), &bytesWritten, NULL);
        if (initializePlate)
            pushPices(listOfPices, buffer, bytesRead);
        else {
            int width, height;
            getDimensions(buffer, bytesRead, &width, &height);
            plate = new Plate(width, height);
            initializePlate = true;
        }
    }
    CloseHandle(hPipe);

    PICES_NUM = listOfPices.size();
    WIDTH = plate->Width;
    HEIGHT = plate->Height;

    final_array = (pice_t*)malloc(sizeof(pice_t)*PICES_NUM);
    pices_array = (pice_t*)malloc(sizeof(pice_t)*PICES_NUM);
    for (int i = 0; i < PICES_NUM; i++) {
        pices_array[i] = (pice_t){0, 0, listOfPices[i].Width, listOfPices[i].Height, false};
    }

    FULL_AREA = get_full_area(pices_array, PICES_NUM);
    print_all(pices_array);
    copy_to_array(final_array, pices_array);

    rect_t *free_pos = (rect_t*)malloc(sizeof(rect_t) * (PICES_NUM + 2));
    free_pos[0] = (rect_t){0, 0, WIDTH, HEIGHT};

    algo(pices_array, PICES_NUM, free_pos, 1);

    printf("Ukupna povrsina je: %d\n", evaluate(final_array));
    print(final_array);

    char exePath[50] = ".\\StakloRezac_GUI.exe "; 
    char sizeofPlate[10];
    plate->getString(sizeofPlate);
    strcat(exePath, sizeofPlate);

    run_executable(exePath);
    CreateOnePipe(&hPipe);

    for (int i = 0; i < PICES_NUM; i++) {
        if (final_array[i].Set == false) {
            continue;
        }
        char buffer[1024];
        make_string(buffer, final_array[i]);
        sendMsg(buffer);
    }

    delete plate;
    free(pices_array);
    free(final_array);
    free(free_pos);
    exit(0);
}

void print_all(pice_t *array) {
    printf("-----------------------------------\n");
    printf("Niz svih parcica:\n\n");
    for (int i = 0; i < PICES_NUM; i++) {
        printf("Parce: [%d, %d], povrsina je %d\n", array[i].Width, array[i].Height, get_area(array[i]));
    }
    printf("-----------------------------------\n");
}

void make_string(char* buffer, pice_t pice) {
    sprintf(buffer, "%d %d %d %d", pice.X, pice.Y, pice.Width, pice.Height);
}

void getDimensions(char* msg, int lenght, int *width, int *height) {
    const char* sep = strstr(msg, " ");
    int lengthPrveReci = sep - msg;

    std::string prvaRec(msg, lengthPrveReci);
    std::string drugaRec(sep + 1);
    
    *width = std::stoi(prvaRec);
    *height = std::stoi(drugaRec);
}

void pushPices(std::vector<Pices>& listOfPices, char* msg, int lenght) {
    int width, height;
    getDimensions(msg, lenght, &width, &height);
    Pices pice(width, height);
    listOfPices.emplace_back(pice);
}

void sendMsg(char *response) {
    DWORD bytesWritten, bytesRead;
    WriteFile(hPipe, response, strlen(response), &bytesWritten, NULL);
    ReadFile(hPipe, response, strlen("RECEIVED\0"), &bytesRead, NULL);
    response[bytesRead] = '\0';
    if (strcmp(response, "RECEIVED") != 0) { 
        printf("Dobio sam poruku koja nije RECEIVED od klijenta vec: %s\n", response);
        exit(0);
    }
}

int get_area(pice_t pice) {
    return pice.Width * pice.Height;
}

int evaluate(pice_t *pices) {
    int sum = 0;
    for (int i = 0; i < PICES_NUM; i++) {
        if (pices[i].Set == true) {
            sum += get_area(pices_array[i]);
        }
    }
    return sum;
}

int successors(pice_t *pice, rect_t *free_pos, int *count, int* p, pice_t *pices) {
    int tmp = *count;
    if (pice->Set == true) {
        return -1;
    }
    for (int i = *p; i < tmp; i++) {

        if (out_of_bounds(free_pos[i], *pice) == true) {
            // !printf("Parce [%d, %d] je out of bounds na poziciji (%d, %d), jer je slobodna dimenzija [%d, %d]\n", 
            // !    pice->Width, pice->Height, free_pos[i].X, free_pos[i].Y, free_pos[i].Width, free_pos[i].Height);
            continue;
        }

        pice->Set = true;
        pice->X = free_pos[i].X;
        pice->Y = free_pos[i].Y;
        int old_x = free_pos[i].X;
        int old_y = free_pos[i].Y;
        int x = pice->X;
        int y = pice->Y;
        int width = pice->Width;
        int height = pice->Height;


        if (free_pos[i].Width - width != 0) {
            free_pos[(*count)++] = (rect_t){old_x + width, old_y, free_pos[i].Width - width, free_pos[i].Height};
        }
        if (free_pos[i].Height - height != 0) {
            free_pos[(*count)++]= (rect_t){old_x, old_y + height, width, free_pos[i].Height - height};
        }
        free_pos[i] = free_pos[--(*count)];

        (*p) = i + 1;
        return 1;
    }
    // printf("Nema vise slobodnih mesta za postavljanje parceta\n");
    return -1;
}

void algo(pice_t *pices, int depth, rect_t *free_pos, int count) {

    if (MAX_AREA == FULL_AREA) {
        return;
    }

    if (depth == 0) {
        max_area(pices);
        return;
    }

    for (int i = 0; i < PICES_NUM; i++) {
        if (pices[i].Set == true) {
            continue;
        }   
        //! printf("(%d, %d) i [%d, %d]\n", pices[i].X, pices[i].Y, pices[i].Width, pices[i].Height);
        int tmp = count;
        pice_t *tmp_pices;
        tmp_pices = (pice_t*)malloc(sizeof(pice_t) * PICES_NUM);
        rect_t *tmp_pos;
        tmp_pos = (rect_t*)malloc(sizeof(rect_t) * PICES_NUM + 1);

        copy_to_array(tmp_pices, pices);
        copy_to_pos(tmp_pos, free_pos);
        // Try placing the piece without rotation
        int p = 0;
        while (successors(&pices[i], free_pos, &count, &p, pices) != -1) {
            //! printf("pozvao sam algo sa depthom = %d\n", depth-1);
            algo(pices, depth - 1, free_pos, count);
            count = tmp;
            p = 0;
        }
        // Try placing the piece with rotation
        if (is_square(pices[i]) == false) {
            copy_to_array(pices, tmp_pices);
            copy_to_pos(free_pos, tmp_pos);

            rotate(&pices[i]);
            while (successors(&pices[i], free_pos, &count, &p, pices) != -1) {
                //! printf("pozvao sam algo sa depthom = %d\n", depth-1);
                algo(pices, depth - 1, free_pos, count);
                count = tmp;
                p = 0;
            }
            // Restore the original state
            rotate(&pices[i]); // Rotate back to original orientation
        }
        copy_to_array(pices, tmp_pices);
        copy_to_pos(free_pos, tmp_pos);
        //copy_to_cut(cuts, tmp_cuts);
        free(tmp_pices);
        free(tmp_pos);
    }
    max_area(pices);
    return;
}

void sort_array(pice_t *array, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (get_area(array[j]) > get_area(array[i])) {
                pice_t tmp = array[j];
                array[j] = array[i];
                array[i] = tmp;
            }
        }
    }
}

void max_area(pice_t *pices) {
    int tmp = evaluate(pices);
    if (tmp > MAX_AREA) {
        MAX_AREA = tmp;
        copy_to_array(final_array, pices);
        printf("Nadjena je nova max povrsina = %d\n", MAX_AREA);
    }
}

void print(pice_t *array) {
    printf("-----------------------------------\n");
    printf("Niz postavljenih parcica:\n\n");
    for (int i = 0; i < PICES_NUM; i++) {
        if (array[i].Set == true) {
            printf("Parce: [%d, %d], postavljeno je (%d, %d) i povrsina je %d\n", 
                array[i].Width, array[i].Height, array[i].X, array[i].Y, get_area(array[i]));
        }
    }
    printf("-----------------------------------\n");
}

bool is_square(pice_t pice) {
    return pice.Height == pice.Width;
}

void rotate(pice_t *pice) {
    int tmp = pice->Width;
    pice->Width = pice->Height;
    pice->Height = tmp;
}

void copy_to_array(pice_t *copy_to, pice_t *copy_from) {
    for (int i = 0; i < PICES_NUM; i++) {
        copy_to[i] = copy_from[i];
    }
}

int get_full_area(pice_t *pices_array, int count) {
    int sum = 0;
    for (int i = 0; i < count; i++) {
        sum += get_area(pices_array[i]);
    }
    return sum;
}

void copy_to_pos(rect_t *copyTo, rect_t *copyFrom) {
    for (int i = 0; i < PICES_NUM; i++) {
        copyTo[i] = copyFrom[i];
    }
}

void sort() {
    for (int i = 0; i < PICES_NUM- 1; i++) {
        for (int j = i + 1; j < PICES_NUM; j++) {
            if (get_area(pices_array[j]) > get_area(pices_array[j])) {
                pice_t tmp = pices_array[j];
                pices_array[j] = pices_array[i];
                pices_array[i] = tmp;
            }
        }
    }
}

bool out_of_bounds(rect_t free_pos, pice_t pice) {
    return (free_pos.Width < pice.Width || free_pos.Height < pice.Height);
}

void print_pos(rect_t *free_pos, int count) {
    printf("-----------------------------------\n");
    printf("Niz slobodni mesta:\n\n");
    for (int i = 0; i < count; i++) {
        printf("Pozicija je: (%d, %d), i slobodno mesto [%d, %d]\n", 
            free_pos[i].X, free_pos[i].Y, free_pos[i].Width, free_pos[i].Height);
    }
    printf("-----------------------------------\n");
}

bool is_pices_equa(pice_t pice1, pice_t pice2) {
    return pice1.X == pice2.X && pice1.Y == pice2.Y && 
           pice1.Width == pice2.Width && pice1.Height == pice2.Height && 
           pice1.Set == pice2.Set;
}