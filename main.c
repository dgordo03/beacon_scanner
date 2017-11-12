#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    char *fileLoc;
    char *fileName;
    int index;
    fileLoc = getenv("HOME");
    fileName = "/dump.txt";
    strcat(fileLoc, fileName);
    index = 0;
    //while (1)
    //{
        FILE *fp;
        fp = fopen(fileLoc, "r");
        char curChar;
        char curLine[100] = {'\0'};
        do
        {
            curChar = fgetc(fp);
            if (curChar != '\n')
            {
               curLine[index] = curChar;
               index++;
            }
            else if (curChar == '\n')
            {
                printf("%s\n", curLine);
                index = 0;
                memset(curLine, '\0', 100);
            }
        } while (curChar != EOF);
        printf("%s\n", curLine);
        fclose(fp);
    //}
    return 0;
}
