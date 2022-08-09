/**
 * @author Alexander Sanfilippo
 * @brief call a python script from a CPP function/script
 * @date 04-08-2022
 * */

#define PY_SSIZE_T_CLEAN
//from example
#include <stdio.h>

//#include <conio.h>
#include <Python.h>
//#include <python3.9/Python.h>
//#include <../miniconda3/envs/summer_project/include/python3.9/Python.h>
int main()
{
	//PyObject* pInt;

	Py_Initialize();

	PyRun_SimpleString("print('Hello World from Embedded Python!!!')");
	
	Py_Finalize();

	return 0;
}





