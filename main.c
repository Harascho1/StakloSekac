//MAIN

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("Koji primer hoces da ucitas?\n");
    char putanjaDoFajla[255];
    scanf("%s", putanjaDoFajla);
    // printf("%s", putanjaDoFajla);
    execl("./StakloRezac_Test","./StakloRezac_Test", putanjaDoFajla, NULL);
    printf("Neuspesno pozivanje drugog programa\n");
    return 0;
}