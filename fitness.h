/**
 * @brief trying to call a python script from a c++ program using this function.
 * @author Alexander Sanfilippo
 * @date 2022-07-29
 * @TODO write a function that calls the fitness function in python
 *     -- Update teh indef thing to have a new name and hexadecimal num
 *     */
	

#ifndef FITNESS_H_6540EED2
#define FITNESS_H_6540EED2

//new includes
#include <stdio.h>
//#include <conio.h> //?what is this?
#include <Python.h>
#include <iostream>

//old includes (not sure what ill need just yet
/*#include <random>
#include <cstdlib>
#include "gene.h"
#include "Genome.h"
#include "Species.h"
#include <unistd.h>
*/




/**
 * @brief calls a python script
 * */
void fitness(char * filename/*string of filename*/){
	
	char filename[] = "evaluate.py";
	FILE* fp;

	Py_Initialize();

	//fp = _Py_fopen(filename, "r"); //"can't find fxn" error
	fp = fopen(filename, "r"); //try this
	PyRun_SimpleFile(fp, filename);

	Py_Finalize();
	/*char filename[] = filename;
	FILE* fp; //file pointer

	Py_Initialize();
	
	fp = _Py_fopen(filename, "r"); //set fp to our file in read mode
	PyRun_SimpleFile(fp, filename); //run the file

	Py_Finalize(); //close python
	*/
	return 0;
};


#endif

