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
//  mpp.apply_curves(w, h, buff)
//
// How to update: use Color Levels/Curves tools in GIMP
//  on a screenshot to pick right parameters, look at gimpoperation*.c
//  for reference on what GIMP does with channels, copy algo here.
//

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
double curve1_chan_value_map(double x) {
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
double curve2_chan_value_map(double x) {
	double ax = 120, ay = 20;
	double bx = 170, by = 220;
	if (x < ax) return 0 + (x / ax) * ay;
	if (x < bx) return ay + ((x - ax) / (bx - ax)) * (by - ay);
	return by + ((x - bx) / (255 - bx)) * (255.0 - by);
}


static PyObject *
mpp_curves(PyObject *self, PyObject *args) {
	unsigned int curve; unsigned short w, h; Py_buffer img;
	if (!PyArg_ParseTuple(args, "IHHy*", &curve, &w, &h, &img)) return NULL;

	double (*curve_func)(double x);
	switch (curve % 3) {
		default: case 0: goto done;
		case 1: curve_func = &curve1_chan_value_map; break;
		case 2: curve_func = &curve2_chan_value_map; break;
	}

	unsigned char *buff = img.buf;
	unsigned char *end = buff + img.len;
	while (buff < end) {
		buff[0] = curve_func(buff[0]);
		buff[1] = curve_func(buff[1]);
		buff[2] = curve_func(buff[2]);
		buff += 3;
	}

	done:
	PyBuffer_Release(&img);
	return Py_None;
}


// Python C-API boilerplate

static PyObject *mpp_error;

static PyMethodDef mpp_methods[] = {
	{"apply_curves", mpp_curves, METH_VARARGS,
		"(w,h,buffer) Loop over specified image-pixel-buffer,"
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
	mpp_error = PyErr_NewException("magnus_pixbuf_proc.error", NULL, NULL);
	Py_INCREF(mpp_error);
	PyModule_AddObject(m, "error", mpp_error);
	return m;
}
