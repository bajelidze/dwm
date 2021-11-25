static const Block blocks[] = {
	/*Icon*/	/*Command*/		/*Update Interval*/	/*Update Signal*/
    { "", "bash weather | sed 's/\\..*°/°/'", 300, 0 },
    { "TEMP: ", "sensors | awk '/Core 0:/ {print $3}' | sed 's/+//;s/\\.0//'", 3, 0 },
    { "", "battery", 5, 0 },
    { " ", "bc <<< \"$(xrandr --prop --verbose | sed -n '/Brightness:/s/.*:\\ //p')*100\" | sed 's/\\..*//'", 0, 12 },
    { "VOL: ", "volumestatus", 0, 10 },
    { "", "klayout", 0, 11 },
	{ "", "date '+%b %d (%a) %H:%M:%S'", 1, 0 },
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim[] = " | ";
static unsigned int delimLen = 5;
