static const Block blocks[] = {
	/*Icon*/	/*Command*/		/*Update Interval*/	/*Update Signal*/
	// { "", "play_widget", 60, 0 },
	{ "", "weather", 300, 0 },
    { "", "sensorstatus", 3, 0 },
    { "", "battery", 5, 0 },
    { "^c#FDD835^^d^", "bc <<< \"$(xrandr --prop --verbose | sed -n '/Brightness:/s/.*:\\ //p')*100\" | sed 's/\\..*//'", 0, 12 },
    { "", "volumestatus", 0, 10 },
    { "", "klayout.sh", 0, 11 },
	{ "", "date '+%b %d (%a) %H:%M:%S'", 1, 0 },
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim[] = " ";
static unsigned int delimLen = 5;
