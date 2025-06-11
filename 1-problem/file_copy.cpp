#include <iostream>
#include <stdio.h>

#define BUF_SIZE 100

int main(){
    FILE* fp;
    FILE* fp2;
    char buf[BUF_SIZE];

    char fileName[] = "data.txt";
    char fileNameCopy[] = "data_copy.txt";

    fp = fopen(fileName, "r");
    fp2 = fopen(fileNameCopy, "w");

    char c;
    while((c = fgetc(fp)) != EOF){
        std::cout << c << "\n";
        fputc(c, fp2);
    }

    fclose(fp);
    fclose(fp2);

}