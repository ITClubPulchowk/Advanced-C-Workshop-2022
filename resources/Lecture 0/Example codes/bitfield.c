#include <stdio.h>
#include <stdbool.h>

typedef struct{ //without bitfields
bool warn;
bool error;
bool info;
} Logger;
int main(){
    Logger logger;
    logger.warn = true;
    logger.error = true;
    logger.info = true;
    if(logger.warn && logger.error && logger.info){

    }
    else if(logger.warn && logger.error){

    }else if (logger.error){

    }else if (logger.info){

    }else if(logger.warn){
        
    }
}