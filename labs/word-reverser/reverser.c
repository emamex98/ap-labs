#include <stdio.h>

int main(){

    while(1) {

        int i = 0;
        char input[250]; 

        while((input[i] = getchar())!='\n'){
            i++;
            if(input[i] == EOF)
                return 0;
        }
        
        for(i=i; i>=0; i--){
            printf("%c", input[i]);
        }
        
        printf("\n");
    }
} 
