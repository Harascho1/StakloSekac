// MAIN LOGIC

#include "declarations.h"
#include <fstream>
#include <sstream>
#include <algorithm>

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

    char exePath[50] = ".\\StakloRezac_GUI.exe "; 
    char sizeofPlate[10];
    plate->getString(sizeofPlate);
    strcat(exePath, sizeofPlate);

    run_executable(exePath);
    CreateOnePipe(&hPipe);

    sortPices(listOfPices);
    giliotineCutiing(plate, listOfPices);
    delete plate;
    exit(0);
}

void print(std::vector<Pices>& listOfPices) {
    for (const Pices& p : listOfPices) {
        p.print();
    }
}

typedef struct
{
    int X, Y, Width, Height;
} rect;
typedef struct
{
    int X, Y;
} indencies;

/*
*   free_rect cuva sve pozicije i koliko mesta ima na toj poziciji (x, y) kordinate gornje-leve tacke i (width, height) velicina slobodnog mesta
*   busy_indencies pamti sve pozicije koje su zauzete jer kako je konstruisan free_rect da gubi te podatke zato se cuva u zasebnom baferu
*/
//TODO: kad se doda 'rect' treba da se proveri da li nesto preseca ako preseca treba da se 'free_rect'-ovi koji su presecani da promene width i height

bool add_free_rect(rect* free_rect, int *free_rect_count, indencies *busy_indencies, int *busy_indencies_count, rect* rect, const Pices& pice, Plate* p) {
    bool placed = false;
    if (*free_rect_count >= 100) {
        printf("prepuniso se memorijski prostor planiran za free_rect\n");
        return false;
    }
    if (*busy_indencies_count >= 100) {
        printf("prepuniso se memorijski prostor planiran za busy_rect\n");
        return false;
    }
    for (int i = 0; i < *busy_indencies_count; i++) {
        if (busy_indencies[i].X == rect->X && busy_indencies[i].Y == rect->Y) {
            std::cout << "Ne moze da ga stavi :)\n";
            return false;
        }
    }
    free_rect[(*free_rect_count)++] = {rect->X + pice.X, rect->Y, rect->Width - pice.X, rect->Height};
    free_rect[(*free_rect_count)++] = {rect->X, rect->Y + pice.Y, rect->Width, rect->Height - pice.Y};
    if (*busy_indencies_count == 0) {
        free_rect[(*free_rect_count)++] = {rect->X + pice.X, rect->Y, rect->Width - pice.X, rect->Height};
        free_rect[(*free_rect_count)++] = {rect->X, rect->Y + pice.Y, rect->Width, rect->Height - pice.Y};

    }
    int x = rect->X, y = rect->Y;
    busy_indencies[*busy_indencies_count] = {x,y};
    *(busy_indencies_count) += 1;
    // for (int counter = 0; counter < *busy_indencies_count; counter++) {
    //     printf("%d %d\n", busy_indencies[counter].X, busy_indencies[counter].Y);
    // }
    
    return true;
}

void giliotineCutiing(Plate* plate, std::vector<Pices>& listOfPices) {

    rect free_rect[100];
    indencies busy_rect[100];
    free_rect[0] = {0, 0, plate->X, plate->Y};
    int free_rect_count = 1;
    int busy_rect_count = 0;
    
    printf("\n");
    for (Pices& pice : listOfPices) {
        for (int rotation = 0; rotation < 2; rotation++) {
            for (int i = 0; i < free_rect_count; i++) {

                if (pice.X > free_rect[i].Width || pice.Y > free_rect[i].Height) {
                    continue;
                }
                
                if (add_free_rect(free_rect, &free_rect_count, busy_rect, &busy_rect_count, &free_rect[i], pice, plate) == false) {
                    continue;
                }

                // Napravi nam string
                char buf[255];
                snprintf(buf, sizeof(buf), "%d %d %d %d", free_rect[i].X, free_rect[i].Y, pice.X, pice.Y);
                // printf("%d %d %d %d\n", free_rect[i].X, free_rect[i].Y, pice.X, pice.Y);
                sendMsg(buf);

                free_rect[i] = free_rect[--free_rect_count];
                pice.Placed = true;
                break;
            }
            if (pice.Placed) {
                break;
            }
            std::swap(pice.X, pice.Y);
        }
    }
    printf("broj zauzetih mesta je: %d", busy_rect_count);
    for (int i = 0; i < busy_rect_count; i++) {
        printf("zauzeta mesta su: (%d, %d)\n", busy_rect[i].X, busy_rect[i].Y);
    }
}

void sortPices(std::vector<Pices>& listOfPices) {
    std::sort(listOfPices.begin(), listOfPices.end(), [] (const Pices& a, const Pices& b) {
        return a.X*a.Y > b.X*b.Y;
    });
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