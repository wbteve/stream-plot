Examples
========

Example usage of streamplot.

Example 0
---------

Run `stdbuf -oL python ex0.py | ../bin/streamplot`. You should see a linear ramp on the plot window.

The command `stdbuf -oL` forces the output of `python ex0.py` to be line buffered. If `stdbuf -oL` is not used (i.e. if the command `python ex0.py | ../bin/streamplot` is entered), because of buffering, no changes are displayed on the plot and after a while, a large number of points are plotted at once. 
