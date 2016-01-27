/* ---------------------------------------------------------------------------------------
--	SOURCE FILE: 	header.h
--			
--	DATE:			November, 8. 2015
--
--	DESIGNER:		Alvin Man, Oscar Kwan
--
--	PROGRAMMER:		Alvin Man, Oscar Kwan
--
--	NOTES:
--	Header file for GPS application.  
--  Includes all necessary libraries.
--  Declares all constants.
--  Declares data structures.
--  Declares function prototypes.
----------------------------------------------------------------------------------------*/
#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <gps.h>
#include <ncurses.h>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <math.h>

#define MAX_SATS_WINDOW 10
#define MAX_POSSIBLE_SATS (MAXCHANNELS - 2)
#define MAIN_WIN_COLOR 1
#define LOCAL_HOST "localhost"
#define GPS_QUIT	0	// default quit
#define GPS_ERROR	-1	// Error with reading GPS data
#define GPS_TIMEOUT	-2	// GPS timeout after too long of waiting for single

using namespace std;

// structures
struct windowData {
	WINDOW *satelliteWin;
	WINDOW *win;
	gps_data_t *gps_data;
};

struct fixsource_t {
	char *spec;
	char const *server;
	char const *port;
	char * device;
};

// functions
void* gpsRead(void *ptr);
void cleanup(windowData* wWindowData, int message);
void printData(windowData*);
WINDOW *createNewWindow(int height, int width, int starty, int startx);
void setUpLeftWindow(WINDOW *win);
void setUpRightWindow(WINDOW *satelliteWin);

#endif // HEADER_H