#include <stdio.h>
#include <stdlib.h>

int main(void){
    int *p1 = malloc(4*sizeof(int)); //allocation enough for an array of 4 ints
    int *p2 = malloc(4*sizeof*p2); //we can do this instead

    if(p1){
        for (int n=0;n<4;++n){//fill the arrays
        p1[n]=n*n;
        }
        for (int n=0;n<4;++n){
            printf("p1[%d]==%d\n",n,p1[n]);

        }
        p2 = realloc(p2,10*sizeof(*p2)); //incase initially allocated memory wasn't enough
        if(p2){
            for (int n=0;n<10;++n)
                p1[n]=n*n;
            for (int n=0;n<10;++n)
                printf("p2[%d]==%d\n",n,p1[n]);
                if(p1) free(p1);
                if(p2) free(p2);
        }
    }
    return 0;
}