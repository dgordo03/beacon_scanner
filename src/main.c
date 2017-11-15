#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
typedef struct {
    int rssi;
    int major;
    int minor;
    char uuid[32];
    int power;
    int n;
} Beacon;

typedef struct {
    int row;
    int column;
    double distance;
    int isParked;
} ParkingSpot;

typedef struct {
    int row;
    int column;
    int count;
    double distance;
} ConsistentSpot;

int beaconCount2;
int rssiTotal2;
int beaconCount1;
int rssiTotal1;
ParkingSpot parkingSpot;
ConsistentSpot newSpot;


void spotJSON(ParkingSpot spot, char *json)
{
    if (spot.isParked == 1)
    {
        sprintf(json, "{\"parked\":\"true\",\"row\":\"%d\",\"column\":\"%d\"}", spot.row, spot.column);
    }
    else
    {
        sprintf(json, "{\"parked\":\"false\"}");
    }
}

/*The distance from the BLE is calculated using the RSSI value
*Input: Beacon structure
*Output: The distance as a float
*/
double distance(Beacon beacon)
{
    //N=2, P=12
    //d= 10 ^ ((Rssi-P)/(10*N))
    double dist;
    double power;
    power = ((double)beacon.rssi - (double)beacon.power) / (10 * (double)beacon.n);
    dist = pow(10.00, power);
    return dist;
}

int newLocation(Beacon beacon)
{
    // 6.7 x 2.6 m
    // make it so that the results have to be 5 consecutive results that all agree
    double dist;
    int updateLocation;
    dist = distance(beacon);
   // printf("distance : %f\t", dist);
   // printf("beacon : %d\n", beacon.minor);
    updateLocation = 0;
    if (dist <= 6.7)
    {
        if (beacon.minor == newSpot.row)
	{
            if (newSpot.distance > dist)
            {
	        newSpot.distance = dist;
	        newSpot.row = beacon.minor;
	        newSpot.column = beacon.major;
	    }
            newSpot.count++;
	}
	else if (newSpot.row == -1)
	{
	    newSpot.distance = dist;
	    newSpot.row = beacon.minor;
	    newSpot.column = beacon.major;
	    newSpot.count = 1;
	}
	else
	{
            if (dist < newSpot.distance)
	    {
	        newSpot.count = 1;
		newSpot.column = beacon.major;
		newSpot.row = beacon.minor;
		newSpot.distance = dist;
	    }
	    else
	    {
	        newSpot.count++;
	    }
	}
    }
    else
    {
	if (newSpot.row == -1)
	{
	    newSpot.count++;
	}
	else if (beacon.minor != newSpot.row)
	{
	    newSpot.count++;
	}
	else
	{
	    newSpot.count = 0;
	    newSpot.column = -1;
	    newSpot.row = -1;
	    newSpot.distance = 0.0;
	}
    }
    if (newSpot.count >= 100)
    {
	if (parkingSpot.row != newSpot.row)
	{
	    updateLocation = 1;
	}
	if (newSpot.distance <= 6.7 && newSpot.distance > 0.0)
	{
	    parkingSpot.row = newSpot.row;
	    parkingSpot.column = newSpot.column;
	    parkingSpot.isParked = 1;
	    parkingSpot.distance = newSpot.distance;
	}
	else
	{
	    parkingSpot.row = -1;
	    parkingSpot.column = -1;
	    parkingSpot.isParked = 0;
	    parkingSpot.distance = 0.0;
	}
    }
    return updateLocation;
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
    iBeacon.minor = -1;
    iBeacon.rssi = -1;
    iBeacon.major = -1;
    iBeacon.power = -1;
    iBeacon.n = -1;
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
	int updateSpot;
        for (i = 0; i < 32; i++)
	{
	    iBeacon.uuid[i] = uuid[i];
	}
	iBeacon.major = hexToDec(major, 4);
	iBeacon.minor = hexToDec(minor, 4);
	twosComplementHex(rssi, 2);
	iBeacon.rssi = hexToDec(rssi, 2);
	iBeacon.power = 62;
	iBeacon.n = 2;
    }
    return iBeacon;
}

int main()
{
	beaconCount1=0;
	beaconCount2=0;
	rssiTotal1=0;
	rssiTotal2=0;
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
    parkingSpot.isParked = 0;
    parkingSpot.row = -1;
    parkingSpot.column = -1;
    parkingSpot.distance = 0.0;
    newSpot.row = -1;
    newSpot.column = -1;
    newSpot.count = 0;
    newSpot.distance = 0.0;

    //while (1)
    //{
        FILE *fp;
        fp = fopen(fileLoc, "r");
        int asciiChar;
	int iBeaconBool = 0;
        char curChar;
	int newParkLoc;
        char curBeacon[500] = {'\0'};
        do
	{
	    asciiChar = fgetc(fp);
	    curChar = asciiChar;
	    if (asciiChar == '>')
	    {
                if (iBeaconBool == 1)
		{
	            beacon = parseiBeacon(curBeacon);
		    if (beacon.minor > -1)
		    {
		       newParkLoc = newLocation(beacon);
		       if (newParkLoc == 1)
		       {
			   char data[100];
			   spotJSON(parkingSpot, data);
			   char cwd[1024];
			   spotJSON(parkingSpot, data);
			   getcwd(cwd, 1024);
			   strcat(cwd, "/files/beacon.txt");
			   writeToFile(cwd, data);
		       }
		    }
		}
		iBeaconBool = 1;
		index = 0;
                memset(curBeacon, '\0', 500);
	    }
	    else if (asciiChar == '<')
	    {
                if (iBeaconBool == 1)
		{
	            beacon = parseiBeacon(curBeacon);
		    if (beacon.minor > -1)
		    {
		       newParkLoc = newLocation(beacon);
		       if (newParkLoc == 1)
		       {
			   //update the parking location data
			   char data[100];
			   char cwd[1024];
			   spotJSON(parkingSpot, data);
			   getcwd(cwd, 1024);
			   strcat(cwd, "/files/beacon.txt");
			   writeToFile(cwd, data);
		       }
		    }
		}
	        iBeaconBool = 0;
	    }
	    if (iBeaconBool == 1)
	    {
                curBeacon[index] = curChar;
	        index++;
	    }
	    sleep(0.1);
        } while (asciiChar != EOF);
	fclose(fp);
    //}
    printf("rssi 1 avg : %f\n", (double)rssiTotal1/(double)beaconCount1);
    printf("rssi 2 avg : %f\n", (double)rssiTotal2/(double)beaconCount2);
    return 0;
}

/* This is how to print to the correct file
char cwd[1024];
getcwd(cwd, 1024);
strcat(cwd, "/files/fileName");
writeToFile(cwd, "string");
*/
