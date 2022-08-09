/**
 * @author Alexander Sanfilippo
 * @brief call a python script from a CPP function/script
 * @date 04-08-2022
 * */


//from example

#include <stdio.h>
//#include <conio.h> //does not exist and/or work on linux
//#include <curses> //suggested replacedmnt for conio.h
#include <Python.h>
int main()
{

	//fails with _Py_open not existing
	char filename[] = "evaluate.py";
	FILE* fp;

	Py_Initialize();

	//fp = _Py_fopen(filename, "r"); //"can't find fxn" error
	fp = fopen(filename, "r"); //try this
	PyRun_SimpleFile(fp, filename);

	Py_Finalize();
	

	//new example --also fails with _Py_fopen_obj not found
	/* 
	PyObject *obj = Py_BuildValue("s", "basic_python.py");
	FILE *file = _Py_fopen_obj(obj, "r+");
	if(file != NULL) {
		PyRun_SimpleFile(file, "test.py");
	}
	Py_Finalize();
	*/

	return 0;
}





