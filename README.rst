Magnus tool with color tweaks
-----------------------------

Stripped-down fork of `stuartlangridge/magnus`_ tool, tweaked to not only zoom
on parts of the screen, but also remap output color ranges to make them more
distinctive from within specific small input range.

Use-case is visual aid for editing small GTK+ Theme elements, where I can't tell
colors of most pixels apart, looking through colorblind eyes at old crappy
el-cheapo display, and them being already very close to begin with.

Consists of slightly modified original magnus script and tiny C module to
process image pixels quickly.

How to prepare/build and use::

  % gcc -O2 -fpic --shared $(python3-config --includes) \
      magnus_pixbuf_proc.c -o magnus_pixbuf_proc.so

  % ./magnus --help
  ...

  % ./magnus

Dependencies (Arch):

- python
- python-gobject
- (optional) libkeybinder3 - for Win-Alt-<plus/minus> zoom keys
- (optional) python-setproctitle - "magnus" in ps output instead of "python ..." line

.. _stuartlangridge/magnus: https://github.com/stuartlangridge/magnus/
