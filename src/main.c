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
    long counter;
} ParkingSpot;

typedef struct {
    int row;
    int column;
    int count;
    double distance;
} ConsistentSpot;

int beaconCount2;
int rssiTotal2;
int beaconCount;
int rssiTotal;
long counter;
float maxDistance = 7.0;
int noSpot = -1;
ParkingSpot parkingSpot;
ParkingSpot oldParkingSpot;
ConsistentSpot newSpot;

void formatString(ParkingSpot spot, char *json)
{
    if (spot.isParked == 1)
    {
        sprintf(json, "parked=true,row=%d,column=%d", spot.row, spot.column);
    }
    else
    {
        sprintf(json, "parked=false");
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
    double dist;
    int updateLocation;
    dist = distance(beacon);
    updateLocation = 0;
    if (counter < parkingSpot.counter)
    {
	return 0;
    }
    if (dist <= maxDistance)
    {
	if (newSpot.row < 0)
	{
            newSpot.distance = dist;
	    newSpot.row = beacon.minor;
	    newSpot.column = beacon.major;
	    newSpot.count = 1;
	}
	else if (beacon.minor == newSpot.row)
	{
	    if (dist < newSpot.distance)
	    {
                newSpot.distance = dist;
	    }
	    newSpot.count++;
	}
	else
	{
            if (dist < newSpot.distance)
	    {
	        newSpot.distance = dist;
		newSpot.row = beacon.minor;
		newSpot.column = beacon.major;
		newSpot.count++;
	    }
	    else
	    {
		newSpot.count++;
	    }
	}
    }
    else
    {
	if (newSpot.row == beacon.minor)
	{
	    newSpot.count = 1;
	    newSpot.column = noSpot;
	    newSpot.row = noSpot;
	    newSpot.distance = dist;
	}
	else
	{
            newSpot.count++;
	}
    }
    printf("count: %d\n",newSpot.count);
    printf("row: %d\n",newSpot.row);
    if (newSpot.count > 20)
    {
        if (newSpot.row != parkingSpot.row)
	{
	    updateLocation = 1;
	}
        if (newSpot.row == noSpot)
	{
            parkingSpot.row = noSpot;
	    parkingSpot.column = noSpot;
	    parkingSpot.isParked = 0;
	    parkingSpot.distance = 0.0;
	    parkingSpot.counter = counter;
	}
	else
	{
	    parkingSpot.row = newSpot.row;
	    parkingSpot.column = newSpot.column;
	    parkingSpot.distance = newSpot.distance;
	    parkingSpot.isParked = 1;
	    parkingSpot.counter = counter;
	}
    }
	/*if (beacon.minor == 1)
	{
		rssiTotal+=beacon.rssi;
		beaconCount++;
	}
	else if (beacon.minor ==2)
	{
		rssiTotal2 += beacon.rssi;
		beaconCount2++;
	}*/
    return updateLocation;
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
        for (i = 0; i < 32; i++)
	{
	    iBeacon.uuid[i] = uuid[i];
	}
	iBeacon.major = hexToDec(major, 4);
	iBeacon.minor = hexToDec(minor, 4);
	twosComplementHex(rssi, 2);
	iBeacon.rssi = hexToDec(rssi, 2);
	iBeacon.power = 40;
	iBeacon.n = 4;
    }
    return iBeacon;
}

int main()
{
    beaconCount=0;
    beaconCount2=0;
    rssiTotal=0;
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
    parkingSpot.row = -10;
    parkingSpot.column = -10;
    parkingSpot.distance = 0.0;
    parkingSpot.counter = 0;
    newSpot.row = -10;
    newSpot.column = -10;
    newSpot.count = 0;
    newSpot.distance = 0.0;
    oldParkingSpot.row = -10;
    oldParkingSpot.column = -10;
    counter = 0;
    int asciiChar;
    int iBeaconBool = 0;
    char curChar;
    int newParkLoc;

    while (1)
    {
        FILE *fp;
        fp = fopen(fileLoc, "r");
        char curBeacon[500] = {'\0'};
	counter = 0;
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
		       counter++;
		       newParkLoc = newLocation(beacon);
		       if (newParkLoc == 1)
		       {
			   //if (oldParkingSpot.row != parkingSpot.row)
			   //{
                               char data[100];
                               formatString(parkingSpot, data);
                               char cwd[1024];
			       char cmd[1024];
                               getcwd(cwd, 1024);
                               strcat(cwd, "/files/beacon.txt");
			       sprintf(cmd, "python src/updateFirebase.py %s %s", "car1", data);
                               system(cmd);
			       oldParkingSpot.row = parkingSpot.row;
			       oldParkingSpot.column = parkingSpot.column;
			   //}
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
		    counter++;
	            beacon = parseiBeacon(curBeacon);
		    if (beacon.minor > -1)
		    {
		       newParkLoc = newLocation(beacon);
		       if (newParkLoc == 1)
		       {
			   //update the parking location data
			   char data[100];
			   char cwd[1024];
			   char cmd[1024];
			   formatString(parkingSpot, data);
			   getcwd(cwd, 1024);
			   strcat(cwd, "/files/beacon.txt");
			   sprintf(cmd, "python src/updateFirebase.py %s %s", "car1", data);
                           system(cmd);
			   oldParkingSpot.row = parkingSpot.row;
			   oldParkingSpot.column = parkingSpot.column;
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
        } while (asciiChar != EOF);
	fclose(fp);
	counter = 0;
    }
    return 0;
}
