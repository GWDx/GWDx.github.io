#include <stdio.h>
#include "incbin.h"

INCTXT(staticOut, "./data/static.out");
INCTXT(dynamic0Out, "./data/dynamic0.out");
INCTXT(dynamic1Out, "./data/dynamic1.out");
INCTXT(dynamic2Out, "./data/dynamic2.out");
INCTXT(dynamic3Out, "./data/dynamic3.out");
INCTXT(dynamic4Out, "./data/dynamic4.out");

char* indexToOut[] = {
    staticOut_data, dynamic0Out_data, dynamic1Out_data, dynamic2Out_data, dynamic3Out_data, dynamic4Out_data,
};

int main() {
    // read currentFile from currentFile
    // if not exist, create it, currentFile = 0
    FILE* fp = fopen("./temp/currentFile", "r");
    int currentFile = 0;
    if (fp != NULL) {
        fscanf(fp, "%d", &currentFile);
        fclose(fp);
    }
    // write currentFile + 1 to currentFile
    fp = fopen("./temp/currentFile", "w");
    fprintf(fp, "%d", currentFile + 1);
    fclose(fp);

    printf("%s", indexToOut[currentFile]);
}
