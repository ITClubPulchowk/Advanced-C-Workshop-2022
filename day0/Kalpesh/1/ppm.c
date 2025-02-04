#include <stdio.h>
#include <stdlib.h>

#define P6

typedef struct{
    unsigned char r,g,b;
}Color;

typedef struct{
    int x,y;
    int r;
}Circle;

typedef struct{
    int x,y;
    int w,h;
}Rect;

int readIntFromBuffer(char * buffer, int * pos){
    int a = 0;
    int index = *pos;
    while (buffer[index] == '\n' || buffer[index] == ' ' || buffer[index] == '\r'){
        (index)++;
    }
    while (buffer[index] != '\n' && buffer[index] != ' ' && buffer[index] != 0 && buffer[index] != '\r'){
        if (buffer[index] >= '0' && buffer[index] <= '9'){
            a = 10 * a + (buffer[index] - '0');
        }
        
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
    FILE* f;
    
    char * buffer = loadFileToBuffer("render.txt", f);

    int i = 0;
    int columns = readIntFromBuffer(buffer,&i);
    int rows = readIntFromBuffer(buffer,&i);


    Color * pixelData = (Color *) malloc(rows * columns * sizeof(Color));
    Color current;
    while (*(buffer+i) != 0){
        if (*(buffer+i) == 'r'){
            Rect r;
            i++;
            r.x = readIntFromBuffer(buffer,&i);
            r.y = readIntFromBuffer(buffer,&i);
            r.w = readIntFromBuffer(buffer,&i);
            r.h = readIntFromBuffer(buffer,&i);
            current.r = readIntFromBuffer(buffer,&i);
            current.g = readIntFromBuffer(buffer,&i);
            current.b = readIntFromBuffer(buffer,&i);
            for (int i=r.x - 0.5 * r.w; i<= r.x + 0.5 * r.w; i++){
                for (int j = r.y - 0.5 * r.h; j<= r.y + 0.5 * r.h; j++){
                    if (i>=0 && j>=0 && i<columns && j<rows)
                        pixelData[i + j * columns] = current;
                }
            }
        }
        else if (*(buffer+i) == 'c'){
            Circle c;
            i++;
            c.x = readIntFromBuffer(buffer,&i);
            c.y = readIntFromBuffer(buffer,&i);
            c.r = readIntFromBuffer(buffer,&i);
            current.r = readIntFromBuffer(buffer,&i);
            current.g = readIntFromBuffer(buffer,&i);
            current.b = readIntFromBuffer(buffer,&i);
            for (int i=c.x - c.r; i<= c.x + c.r; i++){
                for (int j = c.y - c.r; j<= c.y + c.r; j++){
                    int rel_x = i - c.x, rel_y = j-c.y;
                    if ((rel_x* rel_x +rel_y*rel_y -c.r*c.r) <= 0 && i>=0 && j>=0 && i<columns && j<rows){
                        pixelData[i + j * columns] = current;
                    }
                }
            }
        }
        else{ 
        }
        i++;
    }

    fopen_s(&f, "out.ppm", "wb");

#ifdef P3
    fprintf(f, "P3 %d %d 255\n",columns, rows);
    for (int i=0; i<rows*columns; i++){
        fprintf(f, "%d %d %d\n", pixelData[i].r, pixelData[i].g, pixelData[i].b);
    }
#endif

#ifdef P6
    fprintf(f, "P6 %d %d 255\n",columns, rows);
    for (int i=0; i<rows*columns; i++){
        fwrite(&pixelData[i].r, sizeof(unsigned char), 1,f);
        fwrite(&pixelData[i].g, sizeof(unsigned char), 1,f);
        fwrite(&pixelData[i].b, sizeof(unsigned char), 1,f);
    }
#endif
    fclose(f);
    
    

}