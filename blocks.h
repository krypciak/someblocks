//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Icon*/	/*Command*/		/*Update Interval*/	/*Update Signal*/
	{"", "free -h | awk '/^Mem/ { print $3\"/\"$2 }' | sed s/i//g",	30,		0},
    
    {"", "printf \"%.0f%%\" \"$(awk '{u=$2+$4; t=$2+$4+$5; if (NR==1){u1=u; t1=t;} else print ($2+$4-u1) * 100 / (t-t1); }' <(grep 'cpu ' /proc/stat) <(sleep 1;grep 'cpu ' /proc/stat))\"",   5, 0},

    {" ", "echo \"scale = 1; $(cat /sys/class/hwmon/hwmon0/temp1_input) / 1000\" | bc | tr -d '\n'; printf '°C'",    5,      0},

    {" ", "gammastep -p 2> >(grep 'temperature') | awk '{printf $4}'",  15,     0},

    {"", "$HOME/.config/dotfiles/scripts/network-traffic.sh download",  1,      0},
    {"", "$HOME/.config/dotfiles/scripts/network-traffic.sh upload",    1,      0},

    {"﬌", "cat /tmp/updates", 1200, 0},

    {"", "uptime --pretty | sed -E -e \'s/ (minutes|minute)/m/g\' -e \'s/ (hours|hour)/h/g\' -e \'s/ (day|days)/d/g\'", 60, 0},

	{"", "date +'%m %b  %d %a  %R:%S '",		1,		0},
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim[] = "  ";
static unsigned int delimLen = 5;
