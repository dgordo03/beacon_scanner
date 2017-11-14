#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef struct {
    int rssi;
    int major;
    int minor;
    char uuid[32];
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
    char command[1024];
    getcwd(cwd, 1024);
    strcat(cwd, "/src/git_push.sh");
    strcpy(command, "bash ");
    strcat(command, cwd);
    system(command);
}

char decToHexSingle(int dec)
{
    char hex;
    if (dec == 15)
    {
        hex = 'F';
    }
    else if (dec == 14)
    {
        hex = 'E';
    }
    else if (dec == 13)
    {
        hex = 'D';
    }
    else if (dec == 12)
    {
        hex = 'C';
    }
    else if (dec == 11)
    {
        hex = 'B';
    }
    else if (dec == 10)
    {
        hex = 'A';
    }
    else
    {
        hex = dec + '0';
    }
    return hex;
}

int hexToDec(char *hex, int length)
{
    int decimal;
    int i;
    decimal = 0;
    for (i = 0; i < length; i++)
    {
	int curHex;
        if (hex[i] == 'A')
	{
            curHex = 10;
	}
	else if (hex[i] == 'B')
	{
            curHex = 11;
	}
	else if (hex[i] == 'C')
	{
            curHex = 12;
	}
	else if (hex[i] == 'D')
	{
            curHex = 13;
	}
	else if (hex[i] == 'E')
	{
            curHex = 14;
	}
	else if (hex[i] == 'F')
	{
            curHex = 15;
	}
	else
	{
            curHex = hex[i] - '0';
	}
	int j;
	int power;
	power = 1;
	for (j = 1; j < (length-i); j++)
	{
            power *= 16;
	}
	decimal += power * curHex;
    }
    return decimal;
}

int hexToDecSingle(char hex)
{
    int dec;
    if (hex == 'A')
    {
        dec = 10;
    }
    else if (hex == 'B')
    {
        dec = 11;
    }
    else if (hex == 'C')
    {
        dec = 12;
    }
    else if (hex == 'D')
    {
        dec = 13;
    }
    else if (hex == 'E')
    {
        dec = 14;
    }
    else if (hex == 'F')
    {
        dec = 15;
    }
    else
    {
        dec = hex - '0';
    }
    return dec;
}

void twosComplementHex(char *hex, int length)
{
    char twosHex[100] = {'\0'};
    int i;
    for (i = 0; i < length; i++)
    {
	int temp;
        int dec;
	char newHex;
	temp = hexToDecSingle(hex[i]);
        dec = 15 - temp;
	if (i == (length-1))
	{
	    dec += 1;
	}
	hex[i] = decToHexSingle(dec);
    }
}

Beacon parseiBeacon(char *rawData)
{
    //look for 4C 00 02
    char *token;
    char uuid[32] = {'\0'};
    char major[4] = {'\0'};
    char minor[4] = {'\0'};
    char rssi[2] = {'\0'};
    int boolOne, boolTwo, boolThree, boolUUID, boolMajor, boolMinor, boolRSSI, garbage;
    Beacon iBeacon;
    boolOne = 0;
    boolTwo = 0;
    boolThree = 0;
    boolUUID = 0;
    boolMajor = 0;
    boolMinor = 0;
    boolRSSI = 0;
    garbage = 0;
    token = strtok(rawData, " \n");
    while (token != NULL)
    {
	if (boolOne == 0 && strcmp(token, "00") == 0)
	{
            boolOne = 1;
	}
	else if (boolOne == 1 && boolTwo == 0 && strcmp(token, "02") == 0)
	{
            boolTwo = 1;
	}
	else if (boolOne == 1 && boolTwo == 1 && boolThree == 0 && strcmp(token, "15") == 0)
	{
            boolThree = 1;
	}
	else if (boolOne == 1 && boolTwo == 1 && boolThree == 1)
	{
            if (boolUUID < 32)
	    {
                uuid[boolUUID] = token[0];
                uuid[boolUUID+1] = token[1];
                boolUUID += 2;
	    }
	    else if (boolMajor < 4)
	    {
                major[boolMajor] = token[0];
                major[boolMajor+1] = token[1];
                boolMajor += 2;
	    }
	    else if (boolMinor < 4)
	    {
                minor[boolMinor] = token[0];
                minor[boolMinor+1] = token[1];
                boolMinor += 2;
	    }
	    else if (garbage < 2)
	    {
                garbage += 2;
	    }
	    else if (boolRSSI < 2)
	    {
                rssi[boolRSSI] = token[0];
                rssi[boolRSSI+1] = token[1];
		boolRSSI += 2;
	    }
	}
	else
	{
            boolOne = 0;
	    boolTwo = 0;
	    boolThree = 0;
	}
	token = strtok(NULL, " \n");
    }
    if (boolRSSI == 2)
    {
	    int i;
	    iBeacon.major = hexToDec(major, 4);
	    iBeacon.minor = hexToDec(minor, 4);
	    printf("uuid : ");
	    for (i = 0; i < 32; i++)
	    {
		    iBeacon.uuid[i] = uuid[i];
		    printf("%c", iBeacon.uuid[i]);
	    }
	    printf("\n");
	    printf("Major : %d\n", iBeacon.major);
	    printf("Minor : %d\n", iBeacon.minor);
	    twosComplementHex(rssi, 2);
	    iBeacon.rssi = hexToDec(rssi, 2);
	    printf("rssi : %d\n", iBeacon.rssi);
	    printf("RSSI : ");
            for (i = 0; i < 2; i++)
	    {
		    printf("%c", rssi[i]);
	    }
	    printf("\n");
    }
    printf("\n");
}

int main()
{
    char *fileLoc;
    char *fileName;
    int index;
    int beacon_config;
    Beacon beacon;
    fileLoc = getenv("HOME");
    fileName = "/test.txt";
    strcat(fileLoc, fileName);
    index = 0;
    beacon_config = 0;
    //while (1)
    //{
        FILE *fp;
        fp = fopen(fileLoc, "r");
        int asciiChar;
	int iBeaconBool = 0;
        char curChar;
        char curBeacon[500] = {'\0'};
        do
	{
	    asciiChar = fgetc(fp);
	    curChar = asciiChar;
	    if (asciiChar == '>')
	    {
                if (iBeaconBool == 1)
		{
	            parseiBeacon(curBeacon);
		}
		iBeaconBool = 1;
		index = 0;
                memset(curBeacon, '\0', 500);
	    }
	    else if (asciiChar == '<')
	    {
	        if (iBeaconBool == 1)
		{
	            parseiBeacon(curBeacon);
		}
	        iBeaconBool = 0;
	    }
	    if (iBeaconBool == 1)
	    {
                curBeacon[index] = curChar;
	        index++;
	    }   
	    
        } while (asciiChar != EOF);
	fclose(fp);
    //}
    return 0;
}

/* This is how to print to the correct file
char cwd[1024];
getcwd(cwd, 1024);
strcat(cwd, "/files/beacon.txt");
writeToFile(cwd, token);
*/
