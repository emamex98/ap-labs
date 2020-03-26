#include <stdlib.h>

int mystrlen(char *str){

    int length = 0;
    int i;

    for(i = 0; str[i] != '\0'; i++){
        length++;
    }

    return length;
}

char *mystradd(char *origin, char *addition){

    char *ccStr;

    int oLen = mystrlen(origin);
    int aLen = mystrlen(addition);
    int tLen = oLen + aLen;
    int i = 0, j = 0;

    ccStr = malloc(sizeof(char) * (tLen + 1));

    for(i = 0; i < oLen; i++){
        ccStr[i] = origin[i];
    }

    for(j = 0; j < aLen; j++){
        ccStr[i] = addition[j];
        i++;
    }

    ccStr[tLen] = '\0';
    return ccStr;
}

int mystrfind(char *origin, char *substr){

    int oLen = mystrlen(origin);
    int ssLen = mystrlen(substr);
    int match = 0;
    int i = 0, j = 0;
    
    if (ssLen == 0 || ssLen > oLen){
        return 0;
    }

    for(i = 0; i < ((oLen + 1) - ssLen); i++){

        // Check if first letter marches
        if (origin[i] == substr[0]){

            match = 1;

            // Return true if substring is of size 1
            if(ssLen == 1){
                return 1;
            }

            // Continue looking for strinf, break if any char is different from sequence
            for(j = 1; j < ssLen; j++){
                if(origin[i+j] != substr[j]){
                    match = 0;
                    break;
                }
            }
        }
    }

    if (match == 1){
        return 1;
    } else {
        return 0;
    }
}
