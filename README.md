stream-plot
===========

A data visualization tool for streaming data.

Features
---------

- Auto-focus. As streaming data gets displayed, the plot zooms appropriately and fits the data to the screen.
- Key press triggers the program to save the last n samples acquired

Dependencies
-----------

- Python 2.7
- NumPy - http://www.numpy.org/
- Galry (tested with 0.1.0.rc1) - http://rossant.github.io/galry/

How to use
----------

- Install all dependencies (Note that Galry has a few more dependencies).
- See src/streamplot_test.py for an example.

Shortcuts
---------

When the plot figure is in focus, the following shortcuts may be used.
The entire list of available shortcuts may be seen by pressing 'h' when the window is in focus.

- 'a' - Toggle auto-focus
- 'd' - Save the last n samples into a csv file
- 'f' - Save all buffered samples
- 'm' - Change marker style ( yet to be implemented )
