#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mergeInt(void* arr, int l, int m, int r, int type){

    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    int T[r+1], L[n1], R[n2]; 
    char sT[r+1][30], sL[n1][30], sR[n2][30];

    if(type == 1) {

        memcpy(T, arr, sizeof(int)*4);

        for (i = 0; i < n1; i++) 
            L[i] = T[l + i]; 
        for (j = 0; j < n2; j++) 
            R[j] = T[m + 1 + j]; 

        i = 0;
        j = 0;
        k = l;

        while (i < n1 && j < n2) { 
            if (L[i] <= R[j]) { 
                T[k] = L[i]; 
                i++; 
            } 
            else { 
                T[k] = R[j]; 
                j++; 
            } 
            k++; 
        } 

        while (i < n1) { 
            T[k] = L[i]; 
            i++; 
            k++; 
        }

        while (j < n2) { 
            T[k] = R[j]; 
            j++; 
            k++; 
        } 

        memcpy(arr, T, sizeof(int)*4);

    } else {

        memcpy(sT, arr, sizeof(char)*30);

        for (i = 0; i < n1; i++){
            strcpy(sL[i],sT[l + i]);
        }
        for (j = 0; j < n2; j++){
            strcpy(sR[j],sT[m + 1 + j]);
        }

        i = 0;
        j = 0;
        k = l;

        while(i<n1 && j<n2) {

            if(strcmp(sL[i],sR[j]) < 0){
                strcpy(sT[k],sL[i]);
                i++;
            }
            else {
                strcpy(sT[k++],sR[j++]);
                j++;
            }
            k++;

        }

        while(i<n1){
            strcpy(sT[k],sL[i]);
            i++;
            k++;
        }

        while(j<n2){
            strcpy(sT[k],sR[j]);
            j++;
            k++;
        }

        memcpy(arr, sT, sizeof(char)*30);

    }

}

void mergeSortInt(void *arr, int l, int r, int type) 
{    
    if (l < r) 
    { 
        int m = l+(r-l)/2; 
  
        mergeSortInt(arr, l, m, type); 
        mergeSortInt(arr, m+1, r, type); 
        mergeInt(arr, l, m, r, type); 
    } 
} 


int main(int argc, char *argv[])
{

    int nlines;
    int numeric;
    
    if(strcmp(argv[1],"-n") == 0)
        numeric = 1;
    else if(strcmp(argv[1], "-s") == 0)
        numeric = 0;

    if(atoi(argv[2]) < 0 || atoi(argv[2]) > 20)
        printf("Illegal length of input.\n");
    else
        nlines = atoi(argv[2]);

    int numbers[nlines];
    char words[nlines][30];

    if(numeric){
        printf("Enter one number per line: \n");
        for(int i=0; i<nlines; i++){
            scanf("%d", &numbers[i]);
        }
    } else {
        printf("Enter one string per line: \n");
        for(int i=0; i<nlines; i++){
            scanf("%s", words[i]);
        }
    }

    if(numeric){
        mergeSortInt(numbers, 0, nlines - 1, numeric); 
    } else {
        mergeSortInt(words, 0, nlines - 1, numeric); 
    }

    if(numeric){
        for(int i=0; i<nlines; i++){
            printf("%d ", numbers[i]);
        }
    } else {
        for(int i=0; i<nlines; i++){
            printf("%s ", words[i]);
        }
    }

    printf("\n");

    return 0;
}