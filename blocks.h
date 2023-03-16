//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Icon*/	/*Command*/		/*Update Interval*/	/*Update Signal*/
	{"", "$HOME/.config/dotfiles/scripts/bar/mem.sh",	2,		0},
    
    {"", "$HOME/.config/dotfiles/scripts/bar/cpu.sh",  2, 0},

    {" ", "$HOME/.config/dotfiles/scripts/bar/cputemp.sh",    1,      0},

    {" ", "$HOME/.config/dotfiles/scripts/bar/screentemp_gammastep.sh",  15,     0},

    {"", "$HOME/.config/dotfiles/scripts/bar/network-traffic.sh",  5,      0},

    {"﬌", "cat $HOME/.cache/update", 60, 0},

    {" ", "$HOME/.config/dotfiles/scripts/bar/uptime.sh", 60, 0},

    {" ", "cat /tmp/keyboard_layout",                                  1,      0},

	{"", "$HOME/.config/dotfiles/scripts/bar/date.sh",		1,		0},
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim[] = "  ";
static unsigned int delimLen = 5;
