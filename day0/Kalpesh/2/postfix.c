#include <stdio.h>
#include <stdlib.h>

typedef struct{
    double * array;
    int size;
    int top;
}Stack;

void createStack(Stack * stack, int size){
    stack->size = size;
    stack->array = (double *)malloc(size*sizeof(double));
    stack->top = -1;
}


void StackDump(Stack * stack){
    for (int i=0; i<=stack->top; i++){
        printf("%.2f\t", stack->array[i]);
    }
    printf("\n");
}

void push(Stack * stack, double data){
    stack->top++;
    if (stack->top == stack->size){

        int newSize = stack->size * 1.7 + 1;
        // printf("[Realloc increase] Old size: %d New size: %d\n", stack->size, newSize);
        double *newArray = (double *)malloc(newSize * sizeof(double));
        for (int i=0; i< stack->top; i++){
            newArray[i] = stack->array[i];
        }
        free(stack->array);
        stack->size = newSize;
        stack->array = newArray;
    }
    stack->array[stack->top] = data;
}

double pop(Stack * stack){
    if (stack->top == -1){
        return(0.0f);
    }
    double top = stack->array[stack->top];
    stack->top--;
    if ((stack->top) < (stack->size)*0.6){
        int newSize = stack->size * 0.8 + 1;
        // printf("[Realloc decrease] Old size: %d New size: %d\n", stack->size, newSize);
        double *newArray = (double *)malloc(newSize * sizeof(double));
        for (int i=0; i<= stack->top; i++){
            newArray[i] = stack->array[i];
        }
        free(stack->array);
        stack->size = newSize;
        stack->array = newArray;
    }
    return(top);

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

int isNumber(char c){
    return((c >= '0')&& (c<= '9'));
}

int isOperator(char c){
    return((c == '+')||(c == '-')||(c == '*')||(c == '/'));
}

int readIntFromBuffer(char * buffer, int * pos){
    int a = 0;
    int index = *pos;
    while (buffer[index] == '\n' || buffer[index] == ' ' || buffer[index] == '\r'){
        (index)++;
    }
    while (buffer[index] != '\n' && buffer[index] != ' ' && buffer[index] != 0 && buffer[index] != '\r' && !isOperator(buffer[index])){
        if (buffer[index] >= '0' && buffer[index] <= '9'){
            a = 10 * a + (buffer[index] - '0');
        }
        
        (index)++;
    }
    *pos = index;
    return(a);
}


int main (){
    Stack stack;
    createStack(&stack, 8);
    
    FILE * f = NULL;
    char * buffer = loadFileToBuffer("expressions.txt", f);

    int i = 0;
    fopen_s(&f, "results.txt", "w");
    while (buffer[i]){
        if (isNumber(buffer[i])){
            push(&stack, (double)readIntFromBuffer(buffer, &i));
        }
        if (isOperator(buffer[i])){
            if (stack.top < 1){
                printf("Invalid expression\n");
                return(-1);
            }
            double a = pop(&stack);
            double b = pop(&stack);
            // printf("[EVALUATE]: ");
            
            switch (buffer[i]){
            case '+':
                push(&stack, (double)b+a);
                break;
            case '-':
                push(&stack, (double)b-a);
                break;
            case '*':
                push(&stack, (double)b*a);
                break;
            case '/':
                push(&stack, (double)b/a);
                break;
            default:
                break;
            }
            i++;
        }
        if (buffer[i] == '\n' || buffer[i] == 0){
            double result = pop(&stack);
            if (stack.top == -1){
                fprintf(f, "%0.6f\n", result);
            }
        }
        i++;
    }
    fclose(f);
    free(buffer);

    return(0);
}