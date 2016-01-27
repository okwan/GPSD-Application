/* ---------------------------------------------------------------------------------------
--	SOURCE FILE: 	dcgps.cpp 
--
--	FUNCTIONS:
--					int main()					
--					WINDOW *createNewWindow(int height, int width, int starty, int startx)
--					void setUpLeftWindow(WINDOW *win) 
--					void setUpRightWindow(WINDOW* satelliteWin)
--
--	DATE:			November, 8. 2015
--
--	DESIGNER:		Alvin Man, Oscar Kwan
--
--	PROGRAMMER:		Alvin Man, Oscar Kwan
--
--	NOTES:
--	The program is used to get gps data such as the latitude and longitude. The design is
--	uses a thread to capture the gps read operation. THe ncurses library is used to as a
-- 	mechanism to display the data captured by the gps read thread through a clean user
-- 	interface.
-- 
--	The program initializes a connection by opening the gpsd stream and once a connection
-- 	is found it starts to read data from the gps dongle. The data is then displayed on the
--	ncurses window.
----------------------------------------------------------------------------------------*/
#include "header.h"

/* ---------------------------------------------------------------------------------------
--
--	FUNCTION:		main
--
--	DATE:			November, 8. 2015
--
--	DESIGNER:		Alvin Man, Oscar Kwan
--
--	PROGRAMMER:		Alvin Man
--
--  INTERFACE:      int main()
--
--	NOTES:
--	The main function of the program initiates the gps stream and opens it. It also calls
--	the user interfacte window functions and creates the read thread.
--
--	At the end of the program, the cleanup method is called to close the gps stream and windows.
----------------------------------------------------------------------------------------*/
int main() {

	unsigned int flags = WATCH_ENABLE;
	static struct fixsource_t source;

	WINDOW *win, *satelliteWin;

	pthread_t thread;

	//malloc gps data structure
	gps_data_t *gps_data_p = (gps_data_t*) malloc(sizeof(struct gps_data_t));

	//set source information
	source.server = LOCAL_HOST;
	source.port = DEFAULT_GPSD_PORT;
	source.device = NULL;

	//open the gpsd stream
	if(gps_open(source.server, source.port, gps_data_p) == -1){
		fprintf(stderr, "Error: Cannot open gpsd stream.\n");
		exit(0);
	}

	if(source.device != NULL){
		flags |= WATCH_DEVICE;
	}

	gps_stream(gps_data_p, flags, source.device);
	
	if ( (win = initscr()) == NULL) {
	 	cerr << "Could not initialize screen" << endl;
	}

	// window settings
    initscr();
    cbreak();
    keypad(stdscr, true);
    curs_set(0);
    noecho();

    start_color();
	refresh();

	// initialize both windows
	win = createNewWindow(13,45,0,0);
	satelliteWin = createNewWindow(13,45,0,45);

	// setup window labels
	setUpLeftWindow(win);
	setUpRightWindow(satelliteWin);

	// initializes wWindowData structure to pass into readThread
	windowData* thData = (windowData*)malloc(sizeof(struct windowData));
	thData->gps_data = gps_data_p;
	thData->satelliteWin = satelliteWin;
	thData->win = win;

	// create thread & pass structure into it
	pthread_create(&thread, NULL, gpsRead, (void*) thData);
	getch();
	// clean up procedure for windows
	endwin();
	cleanup(thData, GPS_QUIT);
}

/* ---------------------------------------------------------------------------------------
--
--	FUNCTION:		createNewWindow
--
--	DATE:			November, 8. 2015
--
--	DESIGNER:		Oscar Kwan
--
--	PROGRAMMER:		Oscar Kwan
--
--  INTERFACE:      WINDOW *createNewWindow(int height, int width, int starty, int startx)
--
--	RETURNS:		Returns the window (local_win)
--
--	NOTES:
--	This function is used to initialize the ncurses window. You can pass in the size and
--	the coordinates where the window should start at.
----------------------------------------------------------------------------------------*/

WINDOW *createNewWindow(int height, int width, int starty, int startx) {
	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0, 0);
	wrefresh(local_win);  //shows box

	return local_win;
}

/* ---------------------------------------------------------------------------------------
--
--	FUNCTION:		setUpLeftWindow
--
--	DATE:			November, 8. 2015
--
--	DESIGNER:		Oscar Kwan
--
--	PROGRAMMER:		Oscar Kwan
--
--  INTERFACE:      void setUpLeftWindow(WINDOW *win)
--
--	RETURNS:		Returns nothing
--
--	NOTES:
--	Intializes the the left side of the window. This window displays general information
-- 	about the gps such as the latitude and longitude.
----------------------------------------------------------------------------------------*/
void setUpLeftWindow(WINDOW *win) {
	wattrset(win, A_BOLD);
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	mvwprintw(win, 1, 2, "DCGPS");
	mvwprintw(win, 3, 2, "Time:");
	mvwprintw(win, 5, 2, "Latitude:");
	mvwprintw(win, 7, 2, "Longitude:");
	mvwprintw(win, 9, 2, "Speed:");
	mvwprintw(win, 11, 2, "Status:");
	wattroff(win, COLOR_PAIR(1));
	wrefresh(win);
}

/* ---------------------------------------------------------------------------------------
--
--	FUNCTION:		setUpRightWindow
--
--	DATE:			November, 8. 2015
--
--	DESIGNER:		Oscar Kwan
--
--	PROGRAMMER:		Oscar Kwan
--
--  INTERFACE:      void setUpRightWindow(WINDOW* satelliteWin)
--
--	RETURNS:		Returns nothing
--
--	NOTES:
--	Intializes the the right side of the window. This window displays specific information
--	regarding each satellite that is being received by the gps dongle.
----------------------------------------------------------------------------------------*/
void setUpRightWindow(WINDOW* satelliteWin) {
	wattrset(satelliteWin, A_BOLD);
	wattron(satelliteWin, COLOR_PAIR(1));
	mvwprintw(satelliteWin, 1, 2, "PRN    Elevation    Azimuth   SNR  Used");
	wattroff(satelliteWin, COLOR_PAIR(1));
	wrefresh(satelliteWin);
}