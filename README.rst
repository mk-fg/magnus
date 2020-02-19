Magnus tool with color tweaks
=============================

.. contents::
  :backlinks: none


Description
-----------

Stripped-down fork of `stuartlangridge/magnus`_ tool, tweaked to not only zoom
on part of the screen under pointer, but also remap output colors to make them
more distinctive within specific small input range (think GIMP Color Levels/Curves or
"color zoom"), and with the ability to freeze-split image for side-by-side comparison.

.. _stuartlangridge/magnus: https://github.com/stuartlangridge/magnus/

Use-case is visual aid for editing small `GTK+ Theme`_ elements,
where one can't tell colors of most pixels apart, looking through
colorblind eyes at old crappy cheap display, and them being
already very close there to begin with.

.. _GTK+ Theme: https://github.com/mk-fg/clearlooks-phenix-humanity

Consists of simplified magnus gtk app (with bunch of stuff removed)
and tiny C module to process image pixels quickly.


Build / Usage
-------------

It's just a python3 script with C-API .so module in the same dir.

Latter has to be compiled (might need python3-dev pkg or such)::

  % gcc -O2 -fpic --shared $(python3-config --includes) \
      magnus_pixbuf_proc.c -o magnus_pixbuf_proc.so

Run::

  % ./magnus --help
  ...

  % ./magnus


Dependencies
------------

On Arch:

- python
- python-gobject (aka gobject-introspection bindings for python)
- (optional) libkeybinder3 - for Win-Alt-<plus/minus> zoom keys and such
- (optional) python-setproctitle - "magnus" in ps output instead of "python ..." line

Lookup same stuff on other distros, maybe also check original
`stuartlangridge/magnus`_ for how they're called on Debian/Ubuntu.


Key Bindings
------------

These are global key bindings, and are only available if optional libkeybinder3
is installed (along with its gobject-introspection/gi bindings).

Functionality for all these keys is also available via dropdowns/buttons in window header bar.

- Pixel-size zoom (-z/--initial-zoom option):

  - Win-Alt-<plus/equal>: increase zoom level +1
  - Win-Alt-<minus>: decrease zoom level -1

- Color "zoom"/tweaks:

  - Win-Alt-c: cycle color-tweak curves/modes (-c/--color-mode option)

  For color curve parameters see magnus_pixbuf_proc.c file.

- Freeze-split mode (to compare zoomed images side-by-side):

  - Win-Alt-f: freeze-toggle what's currently under cursor in half of the window
  - Win-Alt-Ctrl-f: same as freeze-toggle above, but with 2s delay (-d/--freeze-key-delay option)

  Delay can be useful to freeze snapshot when keyboard will be grabbed by something else.
