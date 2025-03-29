//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Icon*/	/*Command*/		/*Update Interval*/	/*Update Signal*/
	{"󰍛", "$HOME/.config/scripts/bar/mem.sh",	2,		0},

	{"", "$HOME/.config/scripts/bar/swap.sh",	2,		0},

    {"  ", "$HOME/.config/scripts/bar/cpu.sh",  2, 0},

    {"", "$HOME/.config/scripts/bar/cputemp.sh",    2,      0},

    {"", "$HOME/.config/scripts/bar/screentemp-gammastep.sh",  15,     0},

    {"", "$HOME/.config/scripts/bar/network-traffic.sh",  5,      0},

    {"", "$HOME/.config/scripts/bar/updates.sh", 60, 0},

    {" ", "$HOME/.config/scripts/bar/uptime.sh", 60, 0},

	{"", "$HOME/.config/scripts/bar/date.sh",		10,		0},
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim[] = "  ";
static unsigned int delimLen = 5;
