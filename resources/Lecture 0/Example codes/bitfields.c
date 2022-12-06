#include <stdio.h>
#include <stdbool.h>

typedef enum {
    NONE=0,
    LOG_INFO =1,
    LOG_WARN =1<<2,
    LOG_ERROR= 1<<3,
    LOG_ERROR_AND_WARN=LOG_ERROR|LOG_WARN,
    LOG_ALL= LOG_INFO | LOG_ERROR | LOG_WARN,
} LOGGER;

enum {
    PP,
    CC,
    DD
} testenum;

int main(){
    LOGGER log = LOG_ALL;
    printf("%d \n",sizeof(log));
    printf("%d \n",log);
    printf("%d \n",LOG_WARN);
    printf("%d \n",sizeof(testenum));
}