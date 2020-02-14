Magnus tool with image tweaks
-----------------------------

Fork of `stuartlangridge/magnus`_, but designed to not only zoom on parts of
screen, but also map color ranges to make them more distinctive within specific
small input range.

Use-case is editing a GTK+ Theme, where I can't tell colors of most elements
apart, looking through my colorblind eyes onto my old crappy el-cheapo display.

Consists of slightly modified original magnus script and tiny C module to
process image pixels quickly.

How to prepare/build and use::

  % gcc -O2 -fpic --shared \
    $(python3-config --includes) \
    magnus_pixbuf_proc.c -o magnus_pixbuf_proc.so
  % ./magnus

Requirements (Arch): python python-gobject

.. _stuartlangridge/magnus: https://github.com/stuartlangridge/magnus/
