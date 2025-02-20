#include "dataPipe.h"

void run_executable(char* exePath) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcess(
        NULL,
        exePath,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    ) == 0) {
        printf("Proces nije uspeo svoju inicijalizaciju\n");
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void CreateOnePipe(HANDLE* hPipe) {
    *hPipe = CreateNamedPipe(
        TEXT("\\\\.\\pipe\\Podaci"),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1,
        1024,
        1024,
        0,
        NULL
    );

    if (*hPipe == INVALID_HANDLE_VALUE) {
        printf("Nastala je greska prilikom kreiranja pipe-a\n");
        exit(-1);
    }

    if (!ConnectNamedPipe(*hPipe, NULL)) {
        CloseHandle(*hPipe);
        printf("Nije uspelo polezivanje sa drugim krajem pipe-a\n");
        exit(-1);
    }

    //printf("pipe je kreiran.\n");
}

void ConnectPipe(HANDLE* hPipe) {
    char pipeName[] = "\\\\.\\pipe\\Podaci";

    while (1) {
        *hPipe = CreateFile(
            pipeName,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (*hPipe != INVALID_HANDLE_VALUE) {
            break;
        }

        if (GetLastError() != ERROR_PIPE_BUSY) {
            printf("Ne moze da otvori pipe.\n");
            exit(-1);
        }

        // Ako je pipe zauzet, čekaj
        if (!WaitNamedPipe(pipeName, 2000)) {
            printf("Isteklo vreme za konektovanje i konektovanje nije uspelo.\n");
            return;
        }
    }
    // printf("Uspesno sam se konektovao na pipe.\n");
}