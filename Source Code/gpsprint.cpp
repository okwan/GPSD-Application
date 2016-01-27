/* ---------------------------------------------------------------------------------------
--	SOURCE FILE: 	gpsprint.cpp 
--
--	FUNCTIONS:
--					void printData(windowData* wWindowData)
--
--	DATE:			November, 8. 2015
--
--	DESIGNER:		Alvin Man, Oscar Kwan
--
--	PROGRAMMER:		Oscar Kwan
--
--	NOTES:
--	This file conducts the printing operation that is done in the read thread. It uses the
-- 	gps_data structure to point to different elements provided by the gps api.
----------------------------------------------------------------------------------------*/
#include "header.h"

/* ---------------------------------------------------------------------------------------
--
--	FUNCTION:		printData
--
--	DATE:			November, 8. 2015
--
--	DESIGNER:		Alvin Man, Oscar Kwan
--
--	PROGRAMMER:		Oscar Kwan
--
-- INTERFACE:       void printData(windowData* wWindowData)
--
--	RETURNS:		Returns nothing
--
--	NOTES:
--	This function is used by passing in the wWindowData structure that consists of the two
--	windows we are using to display the data and the gps_data to get the info from the gps.
--	
--	Satelitte information
--		- PRN, Eleation, Azimuth, SRN, Used flag
--	GPS information
--		- Timestamp, latitude, longitude, speed, fix
----------------------------------------------------------------------------------------*/
void printData(windowData* wWindowData) {

    time_t now;
    time(&now);
    char buf[200] = {0};
    strftime(buf, sizeof buf, "%b %d %Y. %X", gmtime(&now));

	int n = 0;
	// printing out data in right window
	for (int i = 0; i < MAX_POSSIBLE_SATS; i++) {
		if (n < MAX_SATS_WINDOW) {
			if ((i < wWindowData->gps_data->satellites_visible) && (wWindowData->gps_data->satellites_visible > 0)) {
				mvwprintw(wWindowData->satelliteWin, i+2, 2, "%-3d    %-2d           %-3d       %-2d     %c", 
				wWindowData->gps_data->PRN[i], 
				wWindowData->gps_data->elevation[i],
				wWindowData->gps_data->azimuth[i], 
				(int)wWindowData->gps_data->ss[i],
				wWindowData->gps_data->used[i] ? 'Y' : 'N');
			} else {
				mvwprintw(wWindowData->satelliteWin, i+2, 2, "                                      "); 
			}
		}
		n++;
	}

	// printing out data in left window
	// print time
	if (isnan(wWindowData->gps_data->fix.time) == 0) 
		mvwprintw(wWindowData->win, 3, 15, "%s UTC", buf);

	// print latitude
	if (wWindowData->gps_data->fix.mode >= MODE_2D && isnan(wWindowData->gps_data->fix.latitude) == 0)
		 mvwprintw(wWindowData->win, 5, 15, "%.2f° %c", 
		 	wWindowData->gps_data->fix.latitude,
		 	wWindowData->gps_data->fix.latitude < 0 ? 'S' : 'N');

	// print longitude
	if (wWindowData->gps_data->fix.mode >= MODE_2D && isnan(wWindowData->gps_data->fix.longitude) == 0)
		 mvwprintw(wWindowData->win, 7, 15, "%.2f° %c", 
		 	wWindowData->gps_data->fix.longitude,
		 	wWindowData->gps_data->fix.longitude < 0 ? 'W' : 'E');

    // print speed
    if(!isnan(wWindowData->gps_data->fix.speed))
        mvwprintw(wWindowData->win, 9, 15, "%.2f", wWindowData->gps_data->fix.speed);
    else
        mvwprintw(wWindowData->win, 9, 15, "N/A");

    // print status NO FIX, 2D, OR 3D
    switch(wWindowData->gps_data->fix.mode) {
    	case MODE_2D: 
    		mvwprintw(wWindowData->win, 11, 15, "2D FIX");
    		break;
    	case MODE_3D: 
			mvwprintw(wWindowData->win, 11, 15, "3D FIX");
    		break;
    	default:
    		mvwprintw(wWindowData->win, 11, 15, "NO FIX");
    		break;
    }

	wrefresh(wWindowData->win);
	wrefresh(wWindowData->satelliteWin);
	sleep(1);
}