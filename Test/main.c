//TEST main.c

#define MAX_LENGHT 100

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        printf("Treba samo jedan paramertar i to je putanja do fajla\n");
        exit(-1);
    }

    // printf("Usao sam u Test deo.\n");
    char exePath[] = ".\\StakloRezac_Logic.exe";
    run_executable(exePath);

    HANDLE hPipe;
    CreateOnePipe(&hPipe);

    //printf("pipe je kreiran.\n");

    char putanjaDoFajla[MAX_LENGHT] = "..\\Test\\Primeri\\";
    strcat(putanjaDoFajla, argv[1]);

    FILE *fajl = fopen(putanjaDoFajla, "r");


    char response[MAX_LENGHT] ;
    while(fgets(response, 100, fajl)) {
        DWORD bytesWritten, bytesRead;
        WriteFile(hPipe, response, strlen(response), &bytesWritten, NULL);
        ReadFile(hPipe, response, strlen("RECEIVED\0"), &bytesRead, NULL);
        response[bytesRead] = '\0';
        if (strcmp(response, "RECEIVED") != 0) { 
            printf("Dobio sam poruku koja nije RECEIVED od klijenta vec: %s\n", response);
            exit(0);
        }
    }

    CloseHandle(hPipe);

    //printf("\nUspesno sam zavrsio sam sa prosledjivanjem\n");
    exit(0);
}