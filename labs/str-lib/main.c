#include <stdio.h>
#include "strlib.c"

int main(int argc, char *argv[]) {

    if(argc != 4){
        printf("Error: Missing arguments: <OriginalString> <AddString> <SubString>");
        exit(1);
    }

    char* oStr = argv[1];
    char* ccStr = argv[2];
    char* sbStr = argv[3];
    char* newStr = mystradd(oStr, ccStr);

    printf("Initial lenght      : %d\n", mystrlen(oStr));
    printf("New string          : %s\n", newStr);

    if (mystrfind(newStr, sbStr)==1){
        printf("Substring was found : yes\n");
    } else {
        printf("Substring was found : no\n");
    }

    return 0;
}
