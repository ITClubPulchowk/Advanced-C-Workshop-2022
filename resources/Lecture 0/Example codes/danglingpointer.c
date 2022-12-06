 #include <stdio.h>  
    int *fun(){  
    int y=10;  
    return &y;  
  }  
int main()  
{  
int *p=fun();  
printf("%d", *p);  
return 0;  
}  