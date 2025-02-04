#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b)    ((a<b)?a:b)


typedef struct{
    int * arr;
    int size;
    int capacity;
}Array;

void createArray(Array *a, int capacity){
    a->arr = (int *) malloc(capacity * sizeof(int));
    a->capacity = capacity;
    a->size = 0;
}

void deleteArray(Array *a){
    free(a->arr);
    a->capacity = 0;
    a->size = 0;
}

void resizeArray(Array *a, int newSize){
    int *newArray = (int *)malloc(newSize * sizeof(int));
    for (int i=0; i< MIN(newSize, a->size); i++){
        newArray[i] = a->arr[i];
    }
    free(a->arr);
    a->arr = newArray;
    a->capacity = newSize;
    if (a->size > a->capacity)
        a->size = a->capacity;
}

void addtoArray(Array* a, int data) {
    // resize if size is small
    if (a->size < 0.3 * a->capacity){
        int newSize = a->capacity * 0.3;
        resizeArray(a, newSize);
    }
    
    // full
    if (a->size == a->capacity) {
        int newSize = a->capacity * 1.5 + 1;
        resizeArray(a, newSize);
    }
    
    a->arr[a->size++] = data;
}

void mergeArray(int * arr, int start, int mid, int end){
    int n1 = mid - start +1;
    int n2 = end - mid;
    int *a = (int *)malloc(n1 * sizeof(int));
    int *b = (int *)malloc(n2 * sizeof(int));

    for (int i=0; i< n1; i++){
        a[i] = arr[start + i];
    }
    for (int i=0; i< n2; i++){
        b[i] = arr[mid + 1 + i];
    }
    int i=0,j=0,k=start;
    while (i<n1 && j<n2){
        if (a[i] < b[j]){
            arr[k++] = a[i++];
        }
        else {
            arr[k++] = b[j++];
        }
    }
    while (i < n1) {
        arr[k++] = a[i++];
    }
    while (j < n2) {
        arr[k++] = b[j++];
    }
    free(a);
    free(b);
}

void mergeSort(int * arr, int start, int end){
    // break condition for only one element 
    if (start < end){
        int mid = (start + end)/2;

        mergeSort(arr, start, mid);
        mergeSort(arr, mid + 1, end);

        mergeArray(arr, start, mid, end);
    }
}

int isNumber(char c){
    return((c >= '0')&& (c<= '9'));
}

int readIntFromBuffer(char * buffer, int * pos){
    int a = 0;
    int index = *pos;
    while (!isNumber(buffer[index])){
        (index)++;
    }
    while (isNumber(buffer[index])){
        a = 10 * a + (buffer[index] - '0');
        (index)++;
    }
    *pos = index;
    return(a);
}

char * loadFileToBuffer(const char * filePath, FILE * f){
    fopen_s(&f, filePath, "rb");
    if (!f){
        printf("No file");
        return(0);
    }
    fseek(f, 0, SEEK_END);
    long long int size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char * buffer = (char *)malloc((size + 1) * sizeof(char));

    fread(buffer, 1, size, f);
    buffer[size] = 0;

    fclose(f);
    return(buffer);
}


void main(){
    FILE * f = NULL;
    char * buffer = loadFileToBuffer("mergesort_input.csv", f);
    Array a;
    createArray(&a, 30);
    int i = 0;
    fopen_s(&f, "mergesort_output.csv", "w");
    while (buffer[i]){
        if (isNumber(buffer[i])){
            int num = readIntFromBuffer(buffer, &i);
            addtoArray(&a, num);
        }
        if (buffer[i]=='\n' || buffer[i] == 0){
            mergeSort(a.arr, 0, a.size-1);
            
            for (int i=0; i<a.size-1; i++){
                fprintf(f, "%d,",a.arr[i]);
            }
            fprintf(f, "%d\n", a.arr[a.size-1]);
            a.size = 0;
        }
        i++;
        
    }
    fclose(f);
    deleteArray(&a);
    free(buffer);

}
