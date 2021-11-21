//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Icon*/	/*Command*/		/*Update Interval*/	/*Update Signal*/
	// {"Mem:", "free -h | awk '/^Mem/ { print $3\"/\"$2 }' | sed s/i//g",	30,		0},

    { "", "bash weather | sed 's/\\..*°/°/'", 300, 0 },
    { "BAT: ", "battery", 5, 0 },
    { "BRIT: ", "bc <<< \"$(xrandr --prop --verbose | sed -n '/Brightness:/s/.*:\\ //p')*100\" | sed 's/\\..*//'", 0, 12 },
    { "VOL: ", "volumestatus", 0, 10 },
    { "TEMP: ", "sensors | awk '/Core 0:/ {print $3}' | sed 's/+//;s/\\.0//'", 3, 0 },
    { "", "klayout", 0, 11 },
	{ "", "date '+%b %d (%a) %H:%M:%S'", 1, 0 }
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim[] = " | ";
static unsigned int delimLen = 5;
