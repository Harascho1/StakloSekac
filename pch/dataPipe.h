#ifndef DATAPIPE_H
#define DATAPIPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

void run_executable(char* exePath);
void CreateOnePipe(HANDLE* hPipe);
void ConnectPipe(HANDLE* hPipe);

#ifdef __cplusplus 
}
#endif

#endif