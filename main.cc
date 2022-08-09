/**
 * @brief runs the NEAT algorithm on each processor
 * @author Alexander Sanfilippo
 * @date 2022-07-03
 * @TODO need crossover, and speciation functions
 *  Here is the main NEAT CPP code, which handles all
 *  parts of the program except the construction and running
 *  of the neural networks which will be done in python.  This
 *  main function will run on each processor.  
 * */




#include <iostream>
#include <random>
#include <cstdlib>

#include "gene.h"
#include "Genome.h"
//int Gene::innov_count{ 0 };

//using namespace std;
//int main(){

	/*Create the initial population*/

	/*MAIN LOOP*/

		/*Mutate all*/

		/*Evaluate on cart & pole*/

		/*cross-over*/




//	return 0;
//}
/**
 * @breif determines compatibility of 2 genomes
 * see eqauation 1 pg 13
 * */
double compat(Genome A, Genome B){
	//find larger size of the 2 Genomes
	double D = 0; //number of disjoint genes
	double E = 0; //number of excess genes
	double N; 
					/*Find N*/
	if(A.size() >= B.size()){ N = A.size(); } //size of the larger of the 2 genomes
	else{ N = B.size();}

					/*Find D and E genes, and W*/	
	bool a_done = false;
	bool b_done = false;
	int a_innov = 1;
	int b_innov = 1;
	int ia = 0;
	int ib = 0;
	w_diff_sum = 0; //use to get W
	comp_genes = 0; //how many compatible genes there were
	while(a_done == false || b_done == false){
		//check for match
		if(a_done == false && b_done == false){
			if(A.gene_vec[ia].getInnov() == A.gene_vec[ib].getInnov()){ //matching genes, just move on
				ia++;
				ib++;
				w_diff_sum += abs(A.gene_vec[ia] - B.gene_vec[ib]);
				comp_genes += 1;	
			}	
			//handle disjoints
			else{ //the innov numbers do not match
				if(A.gene_vec[ia] > B.gene_vec[ib]){
					D++; //we found a disjoint.
					ib++;	
				}
				else{
					D++;
					ia++;
				}
			}
			if(A.size() == ia){
				a_done = true;
			}
			else if(B.size() == ib){
				b_done = true;
			}				
		}
		else{ //check for excess genes
			if(a_done == true){
				E++;
				bi++;
			}		
			else{
				E++;
				ai++;
			}

			if(A.size() == ia){
				a_done = true;
			}
			else if(B.size() == ib){
				b_done = true;
			}
		
		}

	}
	
	double W = w_diff_sum / comp_genes;	
	double delta = 1*E/N + 1*D/N + 1*W;
	return delta
};

/**
 * @brief combines genes of 2 parents 
 *
 * */

//void crossover(Genome *pop /*pointer to vector of Genomes*/){
	

//};


