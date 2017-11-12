#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef struct {
    int rssi;
    int beacon;
    int row;
    int column;
} Beacon;

/*The distance from the BLE is calculated using the RSSI value
*Input: Beacon structure
*Output: The distance as a float
*/
float distance(Beacon beacon)
{

}

/*Data is written to a file at a specified location
*Input: the location of the file, the information to be written to that file
*Output: null
*/
void writeToFile(char *fileLoc, char *info)
{
    FILE *fp;
    fp = fopen(fileLoc, "w");
    fprintf(fp, "%s", info);
    fclose(fp);
}

void pushToGit()
{
    char cwd[1024];
    char *command;
    getcwd(cwd, 1024);
    strcat(cwd, "/src/git_push.sh");
    strcpy(command, "bash ");
    strcat(command, cwd);
    system(command);
}

int main()
{
    char *fileLoc;
    char *fileName;
    int index;
    int beacon_config;
    Beacon beacon;
    fileLoc = getenv("HOME");
    fileName = "/dump.txt";
    strcat(fileLoc, fileName);
    index = 0;
    beacon_config = 0;
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
                char *a;
                a = strstr(curLine, "ADV_NONCONN_IND");
                if (a != NULL)
                {
                    beacon_config = 1;
                }
                if (beacon_config == 1)
                {
                    a = strstr(curLine, "RSSI");
                    if (a != NULL)
                    {
                        char *token;
                        token = strtok(curLine, ":");
                        token = strtok(NULL, ":");
                        beacon.rssi = atoi(token);
                        beacon_config = 0;
                    }
                }
                index = 0;
                memset(curLine, '\0', 100);
            }
        } while (curChar != EOF);
        fclose(fp);
    //}
    pushToGit();
    return 0;
}

/* This is how to print to the correct file
char cwd[1024];
getcwd(cwd, 1024);
strcat(cwd, "/files/beacon.txt");
writeToFile(cwd, token);
*/
