/* ---------------------------------------------------------------------------------------
--	SOURCE FILE: 	gps-utils.cpp 
--
--	FUNCTIONS:
--					void* gpsRead(void *ptr)
--					void cleanup(windowData* wWindowData, int message)
--			
--	DATE:			November, 8. 2015
--
--	DESIGNER:		Alvin Man, Oscar Kwan
--
--	PROGRAMMER:		Alvin Man, Oscar Kwan
--
--	NOTES:
--	This file consists of the main functionalities of the program. The core of the program
-- 	is to read information from the gps and print it out. This file consists of the reading
--	portion which is done in a thread.
----------------------------------------------------------------------------------------*/
#include "header.h"

/* ---------------------------------------------------------------------------------------
--
--	FUNCTION:		gpsRead
--
--	DATE:			November, 8. 2015
--
--	DESIGNER:		Alvin Man, Oscar Kwan
--
--	PROGRAMMER:		Alvin Man
--
--  INTERFACE:      void* gpsRead(void *ptr)
--
--	RETURNS:		Returns nothing
--
--	NOTES:			
--  This function is run inside its own thread, and handles the reading of GPS data coming
--  from the gpsd daemon socket.  
--  
--  If we cannot get GPS data from the stream, the timeout
--  is exceeded and this function calls cleanup.  If an error occurrs when reading GPS
--  data, we also issue a cleanup call.	
--
--  If we are successful at reading the GPS data, the data is sent to the printData
--  function to display it on the screen.
----------------------------------------------------------------------------------------*/
void* gpsRead(void *ptr) { 

	// gets structure
	windowData* wWindowData = (windowData*) ptr;

	while(1) {
		if(!gps_waiting(wWindowData->gps_data, 500000)) {

			cleanup(wWindowData, GPS_TIMEOUT);
			break;
		} else {
			if(gps_read(wWindowData->gps_data) == -1){

				cleanup(wWindowData, GPS_ERROR);
				break;
			} else {
				// print gps data
				printData(wWindowData);
			}
		}
	}

	return wWindowData;
}

/* ---------------------------------------------------------------------------------------
--
--	FUNCTION:		cleanp
--
--	DATE:			November, 8. 2015
--
--	DESIGNER:		Alvin Man, Oscar Kwan
--
--	PROGRAMMER:		Alvin Man, Oscar Kwan
--
--  INTERFACE:      void cleanup(windowData* wWindowData, int message)
--
--	RETURNS:		Returns nothing
--
--	NOTES:			
--	This function does the necessary steps to return the program and user to a good state
--  upon an error, timeout, or exit.  
--  The ncurses window is terminated so the user returns to the terminal screen.
--  The gpsd stream is disabled and closed, and the data structures are released from
--  memory.  If the cleanup was called by an error or timeout, an appropriate error msg
--  is displayed to the user.
----------------------------------------------------------------------------------------*/
void cleanup(windowData* wWindowData, int message) {

    if (!isendwin())
    {
	// kill the ncurses window so we get the terminal back
	(void)endwin();
    }

	gps_stream(wWindowData->gps_data, WATCH_DISABLE, NULL);
	gps_close(wWindowData->gps_data);
	free(wWindowData->gps_data);
	free(wWindowData);

	switch(message){
		case GPS_QUIT:
			break;
		case GPS_ERROR:
			(void)fprintf(stderr, "GPS error\n");
			break;
		case GPS_TIMEOUT:
			(void)fprintf(stderr, "GPS timed out\n");
			break;
		default:
			break;
	}
	exit(1);
}