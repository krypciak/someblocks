# someblocks
Modular status bar for [somebar](https://git.sr.ht/~raphi/somebar) written in c.

This is a fork of [dwmblocks](https://github.com/torrinfail/dwmblocks), modified
to connect to somebar instead of dwm.

The mailing list for this project is
[~raphi/public-inbox@lists.sr.ht](mailto:~raphi/public-inbox@lists.sr.ht).
# usage
To use someblocks first run 'make' and then install it with 'sudo make install'.
After that you can put someblocks in your startup script to have it start with dwl/somebar.
# modifying blocks
The statusbar is made from text output from commandline programs.
Blocks are added and removed by editing the blocks.h header file.
By default the blocks.h header file is created the first time you run make which copies the default config from blocks.def.h.
This is so you can edit your status bar commands and they will not get overwritten in a future update.
# patches
Here are some patches to someblocks that add features that I either don't want to merge in, or that require a dwl/somebar patch to work.
I do not maintain these but I will take pull requests to update them.
