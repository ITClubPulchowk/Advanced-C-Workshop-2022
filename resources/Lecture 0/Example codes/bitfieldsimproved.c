#include <stdio.h>
#include <stdbool.h>

typedef enum { //with bitfields
NONE=0; //0b0000
LOG_INFO=1; //0b0001
LOG_WARN = 1 << 1; //0b0010
LOG_ERROR = 1 << 2; //0b0100
LOG_ERROR_AND_WARN = LOG_ERROR|LOG_WARN, //0b0110
LOG_ALL = LOG_INFO|LOG_ERROR|LOG_WARN; //0b0111
} LOGGER;

int main(){
    LOGGER log = LOG_ALL;
    switch(log){
        case LOG_ALL: break;
        case LOG_INFO: break;
        case LOG_ERROR_AND_WARN: break;
        case NONE: break;
    }
}