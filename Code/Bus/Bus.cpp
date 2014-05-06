// Bus.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <shlwapi.h>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


#pragma comment(lib, "Shlwapi.lib")


using namespace std;


typedef vector<string> BusLines;
typedef map<string, BusLines> BusHoursMap;

struct BusMap
{
	BusLines busLines;
	BusHoursMap workDays;
	BusHoursMap saturDays;
	BusHoursMap holyDays;
};


void ImportBusHours(const char* hours, BusHoursMap& hoursMap, const char* busLine)
{
	string hoursStr(hours);
	istringstream hs(hoursStr);
	string hour;

	while( hs >> hour )
	{
		const char* currHour = hour.c_str();
		int hourTime = 0, minTime = 0;

		if( sscanf(currHour, "%d:%d", &hourTime, &minTime)== 2 )
		{
			hoursMap[currHour].push_back(busLine);
		}
	}
}


void ImportBusHours(FILE* busLog, BusMap& busMap, const char* busLine)
{
	char currLine[1000];
	BusHoursMap* busHours = &busMap.workDays;

	while( fgets(currLine, 1000, busLog) )
	{
		if( ! isprint(currLine[0]) )
			continue;
		else if( strstr(currLine, "Sábados") == currLine )
			busHours = &busMap.saturDays;
		else if( strstr(currLine, "Domingos e Feriados") == currLine )
			busHours = &busMap.holyDays;
		else if( atoi(currLine) )
			ImportBusHours(currLine, *busHours, busLine);
		else
			break;
	}
}


const char* FindDirection(const char* dirStr)
{
	if( strstr(dirStr, "SAO PAULO") == dirStr )
		return "SP";
	if( strstr(dirStr, "SAO BERNARDO") == dirStr )
		return "SBC";
	if( strstr(dirStr, "SANTO ANDRE") == dirStr )
		return "SA";
	if( strstr(dirStr, "OSASCO") == dirStr )
		return "OS";
	if( strstr(dirStr, "DIADEMA") == dirStr )
		return "DI";
	return "UNK";
}


void CorrectDirection(char* dirStr)
{
	char* spc = dirStr;
	while( isspace(*spc) )
		spc++;
	if( spc != dirStr )
		memmove(dirStr, spc, strlen(spc) + 1);

	spc = &dirStr[strlen(dirStr) - 1];
	while( isspace(*spc) )
		spc--;
	if( spc != dirStr )
		*(spc + 1) = 0;

	if( const char* dir = FindDirection(dirStr) )
		strcpy(dirStr, dir);
}


void ImportBus(FILE* busLog, BusMap& busMap)
{
	char currLine[1000];
	char currBusLine[500];
	char currDirection[500];
	char currBusStart[500];
	char currBusEnd[500];

	while( fgets(currLine, 1000, busLog) )
	{
		if( strstr(currLine, "Linha:") == currLine )
		{
			const char* busLine = currLine + sizeof("Linha:");
			sscanf(busLine, "%s", currBusLine);
			continue;
		}

		if( strstr(currLine, "Terminal Inicial:") == currLine )
		{
			const char* busLine = currLine + sizeof("Terminal Inicial:");
			strcpy(currBusStart, busLine);
			continue;
		}

		if( strstr(currLine, "Terminal Final:") == currLine )
		{
			const char* busLine = currLine + sizeof("Terminal Final:");
			strcpy(currBusEnd, busLine);

			char currBusData[1500];
			sprintf(currBusData, "Cod: %s\r\nStart:%sEnd:%s\r\n", 
				currBusLine, currBusStart, currBusEnd);
			busMap.busLines.push_back(currBusData);

			continue;
		}

		if( strstr(currLine, "Itinerário Sentido") == currLine )
		{
			strcpy(currDirection, currLine + sizeof("Itinerário Sentido"));
			CorrectDirection(currDirection);
			continue;
		}

		if( strstr(currLine, "Dias Úteis") == currLine )
		{
			char currBusLineAndDir[1000];
			sprintf(currBusLineAndDir, "%s-%s", currBusLine, currDirection);
			ImportBusHours(busLog, busMap, currBusLineAndDir);
		}
	}
}


void ExportBusHours(const BusHoursMap& hoursMap, FILE* busTxt)
{
	for( BusHoursMap::const_iterator curr = hoursMap.begin(), end = hoursMap.end();
		curr != end; ++curr )
	{
		fprintf(busTxt, "%s: ", curr->first.c_str());
		const BusLines& busLines = curr->second;
		
		for( size_t i= 0; i < busLines.size(); ++i )
		{
			fprintf(busTxt, "%s ", busLines[i].c_str());
		}
		fputs("\r\n", busTxt);
	}
}


void ExportBus(const BusMap& busMap, FILE* busTxt)
{
	fputs("Linhas:\r\n", busTxt);
	for( size_t i = 0; i < busMap.busLines.size(); ++i )
		fprintf(busTxt, "%s", busMap.busLines[i].c_str());

	fputs("\r\n\r\nDias Úteis\r\n", busTxt);
	ExportBusHours(busMap.workDays, busTxt);

	fputs("\r\n\r\nSábados\r\n", busTxt);
	ExportBusHours(busMap.workDays, busTxt);

	fputs("\r\n\r\nDomingos e Feriados\r\n", busTxt);
	ExportBusHours(busMap.workDays, busTxt);
}


int _tmain(int argc, _TCHAR* argv[])
{
	if( argc == 3 )
	{
		FILE* busLog = fopen(argv[1], "r");
		FILE* busTxt = fopen(argv[2], "w");

		if( busLog && busTxt )
		{
			BusMap busMap;

			ImportBus(busLog, busMap);
			ExportBus(busMap, busTxt);
		}
	}
}

