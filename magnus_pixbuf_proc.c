// Python C-API module to modify image buffer in-place,
//  applying GIMP-like "Color -> Curves" for Value.
// Goal is to enhance legibility of colors
//  in specific range which I have to deal with atm.
//
// Build with:
//  gcc -O2 -fpic --shared $(python3-config --includes) \
//    magnus_pixbuf_proc.c -o magnus_pixbuf_proc.so
// Usage: import magnus_pixbuf_proc as mp; mpp.apply_curves(w, h, buff)
//
// How to update: use Color Levels/Curves tools in GIMP
//  on a screenshot to pick right parameters, look at gimpoperation*.c
//  for reference on what GIMP does with channels, copy algo here.
//

#include <stdio.h>

#include <Python.h>


// Curve for each channel in RGB:
// --------
//       /
//      /
//     /
// ---o
// --------
double ox = 170, oy = 20;

#define chan_value_map(v) \
	v < ox ? oy * v / 255.0 : oy + (255.0 - oy) * (v - ox) / (255.0 - ox);

static PyObject *
mpp_curves(PyObject *self, PyObject *args) {
	unsigned short w, h; Py_buffer img;
	if (!PyArg_ParseTuple(args, "HHy*", &w, &h, &img)) return NULL;

	unsigned char *buff = img.buf;
	unsigned char *end = buff + img.len;

	while (buff < end) {
		buff[0] = chan_value_map((double) buff[0]);
		buff[1] = chan_value_map((double) buff[1]);
		buff[2] = chan_value_map((double) buff[2]);
		buff += 3;
	}

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
