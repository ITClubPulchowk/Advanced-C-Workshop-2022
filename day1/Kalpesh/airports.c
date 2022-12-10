#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define AIRPORTS 15
#define slotsNo 15


typedef struct ValueNode{
    const char * value;
    struct ValueNode * next;
    uint32_t keyHash;
}ValueNode;

typedef struct{
    ValueNode * slots[slotsNo];
}HashTable;


uint32_t adler32(const char * key, int len){
    uint32_t a = 1, b = 0;
    for (int i = 0; i<len; i++){
        a = (a + key[i])%65521;
        b = (a + b)%65521;
    }
    return(a|(b<<16));
}

int len(const char * str){
    int len = 0;
    while (str[len]){
        len++;
    }
    return(len);
}



int isDelimiter(char c){
    return((c == 0) || (c == '\"') || (c == '\n') || (c == '\r'));
}

int isAlphabet(char c){
    return((c >= 'a' && c<= 'z')|| (c >= 'A' && c<= 'Z'));
}

const char * readStringFromBuffer(char * buffer, int * pos){
    while (!isAlphabet(buffer[*pos])){
        (*pos)++;
    }
    const char * a = buffer + *pos;
    while (!isDelimiter(buffer[*pos])){
        (*pos)++;
    }
    // changing delimiter to 0
    buffer[(*pos)++] = 0;
    
    return(a);
}

// sets all slots to point to NULL
void initHashTable(HashTable * hashTable){
    for (int i=0; i<slotsNo; i++){
        hashTable->slots[i] = NULL;
    }
}

void addToHashTable(HashTable *hashtable ,const char * key, const char * value){
    uint32_t keyHash = adler32(key, len(key));
    int index = keyHash % slotsNo;
    
    ValueNode * val = (ValueNode*)malloc(sizeof(ValueNode));
    val->value = value;
    val->keyHash = keyHash;
    val->next = NULL;

    if (!hashtable->slots[index]){
        hashtable->slots[index] = val;
    }
    else{
        printf("COLLISION: index:%d\n", index);
        ValueNode * a = hashtable->slots[index]; 
        while (a->next){
            a = a->next;
        }
        a->next = val;
    }

}

void emptyHashTable(HashTable * hashtable){
    for (int i=0; i< slotsNo; i++){
        if (hashtable->slots[i]){
            ValueNode *a, * b = hashtable->slots[i];
            while (b){
                a = b->next;
                free(b);
                b = a;
            }
            hashtable->slots[i] = NULL;
        }
    }
}


void hashTableDump(HashTable * hashtable){
    for (int i = 0; i<slotsNo; i++){
        printf("%d:",i);
        ValueNode * a = hashtable->slots[i];
        while (a){
            printf("%s\t", a->value);
            a = a->next;
        }
        printf("\n");
    }
}


ValueNode * getNodeFromHashtable(HashTable * hashtable, const char * key){
    uint32_t keyhash = adler32(key, len(key));
    int index = keyhash%slotsNo;

    ValueNode * val = hashtable->slots[index];
    while (val){
        if (val->keyHash == keyhash){
            return(val);
        } 
        val = val->next;
    }
    return(NULL);
}



char * loadFileToBuffer(const char * filePath, FILE * f){
    fopen_s(&f, filePath, "rb");
    if (!f){
        printf("No file");
        return(0);
    }
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char * buffer = (char *)malloc((size + 1) * sizeof(char));

    fread(buffer, 1, size, f);
    buffer[size] = 0;

    fclose(f);
    return(buffer);
}

void maxFlights(const char ** airports, HashTable * h){
    int max = 0;
    const char * winningAirport = "";
    for (int i=0; i< AIRPORTS;i++){
        int flights = 1;
        ValueNode * current = getNodeFromHashtable(h, airports[i]);
        if (current) {
            uint32_t hash = current->keyHash;
            while (current->next && (current->next->keyHash == hash)){
                flights++;
                current = current->next;
            }
            if (max <= flights){
                max = flights;
                winningAirport = airports[i];
            }
        }
    }

    FILE * f;
    fopen_s(&f, "airport_output.csv", "w");
    fprintf(f, "%d",max);
    ValueNode *winning = getNodeFromHashtable(h, winningAirport);
    for(int i=0; i< max; i++){
        fprintf(f,",\"%s\"",winning->value);
        winning = winning->next;
    }
    fclose(f);
}



void main(){
    HashTable h;
    initHashTable(&h);
    FILE * f = NULL;
    char *bufferA = loadFileToBuffer("airport_a.txt", f);
    const char * airports[AIRPORTS];
    int index =0;
    for (int i=0; i<15; i++){
        airports[i] = readStringFromBuffer(bufferA, &index);
    }

    
    index = 0;
    char * bufferB = loadFileToBuffer("airport_b.csv",f);
    const char * to;
    const char * from;
    while (bufferB[index]){
        from = readStringFromBuffer(bufferB, &index);
        to = readStringFromBuffer(bufferB, &index);
        addToHashTable(&h, to, from);
    }
    // hashTableDump(&h);

    maxFlights(airports, &h);
    
    emptyHashTable(&h);

    if (bufferA)
        free(bufferA);
    if (bufferB)
        free(bufferB);
}