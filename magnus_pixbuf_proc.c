// Python C-API module to modify image buffer in-place,
//  applying GIMP-like "Color -> Curves" for Value.
// Goal is to enhance legibility of colors
//  in specific range which I have to deal with atm.
//
// Build with:
//  gcc -O2 -fpic --shared $(python3-config --includes) \
//    magnus_pixbuf_proc.c -o magnus_pixbuf_proc.so
// Usage:
//  import magnus_pixbuf_proc as mpp
//  print(mpp.color_modes)
//  mpp.apply_curves(mode_index, w, h, buff)
//
// How to update: use Color Levels/Curves tools in GIMP
//  on a screenshot to pick right parameters, look at gimpoperation*.c
//  for reference on what GIMP does with channels, copy algo here.
//

#define __STDC_WANT_LIB_EXT2__ 1
#include <stdio.h>

#include <Python.h>


// Value mapping: y(x) = y-min + x-fraction(0-1.0) * y-range

// Curve-1 for each of RGB - for light colors
// --------x
//        /
//       /
//      /
//   --o = (170,20)
//  /
// x--------
double mpp_light_chan_value_map(double x) {
	double ox = 170, oy = 20;
	if (x < ox) return 0 + (x / ox) * oy;
	return oy + ((x - ox) / (255.0 - ox)) * (255.0 - oy);
}

// Curve-2 for each of RGB - mid-range colors
// ---------x
//       b-/ = (170,220)
//      /
//     /
//  /-a = (120,20)
// x--------
double mpp_mid_chan_value_map(double x) {
	double ax = 120, ay = 20;
	double bx = 170, by = 220;
	if (x < ax) return 0 + (x / ax) * ay;
	if (x < bx) return ay + ((x - ax) / (bx - ax)) * (by - ay);
	return by + ((x - bx) / (255.0 - bx)) * (255.0 - by);
}

// Curve-3 for each of RGB - dark colors
// -------x
//       /
//    o-- = (120,220)
//   /
//  /
// x-------
double mpp_dark_chan_value_map(double x) {
	double ox = 120, oy = 220;
	if (x < ox) return 0 + (x / ox) * oy;
	return oy + ((x - ox) / (255.0 - ox)) * (255.0 - oy);
}

#define mpp_color_mode_max 3
#define mpp_color_mode_list \
	Py_BuildValue( "[s,s,s,s]", "none", \
		"light shades", "mid shades", "dark shades" )


static PyObject *mpp_error;

static PyObject *
mpp_curves(PyObject *self, PyObject *args) {
	unsigned int color_mode; unsigned short w, h; Py_buffer img;
	if (!PyArg_ParseTuple(args, "IHHy*", &color_mode, &w, &h, &img)) return NULL;
	PyObject *res = NULL;

	if (color_mode > mpp_color_mode_max) {
		char *err;
		int n = asprintf( &err,
			"Index out of range for mpp.color_modes list: %d", color_mode );
		PyErr_SetString(mpp_error, err);
		goto end;
	}

	double (*curve_func)(double x) = NULL;
	switch (color_mode) {
		default: case 0: break;
		case 1: curve_func = &mpp_light_chan_value_map; break;
		case 2: curve_func = &mpp_mid_chan_value_map; break;
		case 3: curve_func = &mpp_dark_chan_value_map; break;
	}

	if (curve_func) {
		unsigned char *buff = img.buf;
		unsigned char *end = buff + img.len;
		while (buff < end) {
			buff[0] = curve_func(buff[0]); // R
			buff[1] = curve_func(buff[1]); // G
			buff[2] = curve_func(buff[2]); // B
			buff += 3; } }
	res = Py_None;

	end:
	PyBuffer_Release(&img);
	return res;
}


// Python C-API boilerplate

static PyMethodDef mpp_methods[] = {
	{"apply_curves", mpp_curves, METH_VARARGS,
		"(mode_index,w,h,buffer) Loop over specified image-pixel-buffer,"
			" applying hardcoded curve adjustments to each pixel in there."},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef mpp_module = {
	PyModuleDef_HEAD_INIT,
	"magnus_pixbuf_proc",
	"Fast processing for Gdk.pixbuf.get_pixels() buffers.",
	-1,
	mpp_methods
};

PyMODINIT_FUNC PyInit_magnus_pixbuf_proc(void) {
	PyObject *m = PyModule_Create(&mpp_module);
	if (!m) return NULL;

	PyModule_AddObject(m, "color_modes", mpp_color_mode_list);

	mpp_error = PyErr_NewException("magnus_pixbuf_proc.error", NULL, NULL);
	Py_INCREF(mpp_error);
	PyModule_AddObject(m, "error", mpp_error);

	return m;
}
