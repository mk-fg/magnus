Magnus tool with color tweaks
-----------------------------

Stripped-down fork of `stuartlangridge/magnus`_ tool, tweaked to not only zoom
on part of the screen under pointer, but also remap output colors to make them
more distinctive within specific small input range (think GIMP Color Levels/Curves).

.. _stuartlangridge/magnus: https://github.com/stuartlangridge/magnus/

Use-case is visual aid for editing small `GTK+ Theme`_ elements,
where one can't tell colors of most pixels apart, looking through
colorblind eyes at old crappy cheap display, and them being
already very close there to begin with.

.. _GTK+ Theme: https://github.com/mk-fg/clearlooks-phenix-humanity

Consists of simplified magnus gtk app (with bunch of stuff removed)
and tiny C module to process image pixels quickly.

How to prepare/build and use::

  % gcc -O2 -fpic --shared $(python3-config --includes) \
      magnus_pixbuf_proc.c -o magnus_pixbuf_proc.so

  % ./magnus --help
  ...

  % ./magnus

Dependencies (Arch):

- python
- python-gobject
- (optional) libkeybinder3 - for Win-Alt-<plus/minus> zoom keys and such
- (optional) python-setproctitle - "magnus" in ps output instead of "python ..." line

Global key bindings (only available with optional libkeybinder3):

- Win-Alt-<plus/equal>: increase zoom level +1
- Win-Alt-<minus>: decrease zoom level -1
- Win-Alt-c: cycle color-tweak curves/modes
