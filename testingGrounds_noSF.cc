/**
 * @author Alex S
 * @date 11-06-2022
 *This file will allow us to test things as we set up the Gene, Genome, and Species classes
 * */



#include <iostream>
#include <random>


#include "gene.h"
#include "Genome.h"
#include "Species.h"


//REMOVE LATER
#include "crossover.h"
#include <typeinfo>

//put this function in crossover.h later
void crossover(Genome g1, Genome g2, Genome * child);




int Gene::innov_count{ 0 };

using namespace std;
int main(){

		/*21-07-2022*/
		Genome g1; //constructor
		//g1.~Genome(); //destructor 
		

		//test copy constructor 
		Genome g2 = Genome(g1); //copy constructor 
		g1.mutate();
		g1.mutate();
		g1.mutate();
		g1.mutate();
		g1.mutate();
		g1.mutate();
		cout << "g1:\n";
		g1.summary();
		cout << "g2\n";
		g2.summary();

		//test copy assmt operator 
		Genome g3; 
		g3 = g1; //copy assignment operator
		cout << "g1:\n";
		g1.summary();
		cout << "g3\n";
		g3.summary();

		//test move constructor 

		//test move assmt op

		/*Creating whole new population of 2 species to find errors in Cull()
 		* Specifically, a segfault error with std::vector.erase()*/
		/*creating an initial population of Genomes in Species*/
		

		
 		int NB1 = 4; //species NB1 size;
		int NB2 = 4; //species NB2 size
		std::vector <Species> popB; //second population, "B"
		Species speciesB1;
		Species speciesB2;
		for(int i = 0; i < NB1; i++){
			Genome newGenome; 
			speciesB1.genome_vec.push_back(newGenome);
		}
		popB.push_back(speciesB1); //add species to population
		
		for(int i = 0; i < NB2; i++){
			Genome newGenome; 
			speciesB2.genome_vec.push_back(newGenome);
		}
		popB.push_back(speciesB2); //add species to population
		
		//Mutate all the genomes in the species & calc fitness
		int num_mut = 3;
		for(unsigned long int i = 0; i < popB[0].genome_vec.size(); i++){
			for(int j = 0; j < num_mut; j++){
				popB[0].genome_vec[i].mutate();
			}
			//get fitness
			popB[0].genome_vec[i].dummyFitness();
			popB[0].genome_vec[i].calcAdjusted(NB1);
		}

		for(unsigned long int i = 0; i < popB[1].genome_vec.size(); i++){
			for(int j = 0; j < num_mut; j++){
				popB[1].genome_vec[i].mutate();
			}
			//get fitness
			popB[1].genome_vec[i].dummyFitness();
			popB[1].genome_vec[i].calcAdjusted(NB2);
		}

		//===call cull() on both species in popB===
		cout << "calling cull() popB[0]...\n";
		popB[0].cull();
		cout << "calling cull() popB[0]...\n";
		popB[1].cull();
		
	return 0;
		
}
/**
 * @brief produces offspring genome between 2 adults in a species
 * @param g1 the first parent gene
 * @param g2 the second parent gene
 * */
void crossover(Genome g1, Genome g2, Genome *child){

	std::cout << "inside crossover()...\n";
	
	//create an empty Genome called child to store the inherited genes
	//Genome child(true);
	 
	auto urd = std::uniform_real_distribution<>(0,1); //random probability
	//auto uid = std::uniform_int_distribution<>(0,gene_vec.size() - 1); //random gene

	bool g1_done = false;
	bool g2_done = false;
	int i = 0; //index for g1's gene_vec
	int j = 0; //index for g2's gene_vec

	
	while(g1_done == false || g2_done == false){
		//check for match
		if(g1_done == false && g2_done == false){
			if(g1.gene_vec[i].getInnov() == g2.gene_vec[j].getInnov()){ //matching genes
				//pick parent to inherit the gene from
				if(urd(gen) < 0.5){
					Gene gene(g1.gene_vec[i]); //cpy construct gene
					child->gene_vec.push_back(gene); //add gene to child
				}	
				else{
					Gene gene(g2.gene_vec[j]); //cpy construct gene
                                        child->gene_vec.push_back(gene); //add gene to child
				}
			
				//disabled check			
				if(g1.gene_vec[i].getEnabled() == false || g2.gene_vec[j].getEnabled() == false ){
					
					if(urd(gen) < 0.75){
						//set gene to disabled in child
						child->gene_vec.back().setEnabled(true);
						//*last->setEnabled(false);
					}
					else{
						//set gene to enabled in child
						child->gene_vec.back().setEnabled(true);
                                                //last.setEnabled(true);
					}

					
					
				}
				i++;
				j++;
			}	
			//handle disjoints
			else{ //the innov numbers do not match
				if(g1.gene_vec[i].getInnov() > g2.gene_vec[j].getInnov()){ //if D in g2
					//D++; //we found a disjoint.
					//determine which parent is fitter
					if( g1.getAdjusted() <= g2.getAdjusted() ){
						//inherit the Disjoint Gene
						Gene gene ( g2.gene_vec[j] );
						child->gene_vec.push_back(gene);
					}
					
					j++;	
					std::cout << "disjoint gene from g2 added to child" << std::endl;
				}
				else{ //if D in g1
					
					if( g1.getAdjusted() >= g2.getAdjusted() ){
						//inherit the Disjoint Gene
						Gene gene ( g1.gene_vec[i] );
						child->gene_vec.push_back(gene);
					}
					i++;
					std::cout << "disjoint gene from g1 added to child" << std::endl;
				}
			}
		//Check for end of either Genome reached
		if(g1.size() <= i){
			g1_done = true;
			std::cout << "g1 hit end" << std::endl;
		}
		if(g2.size() <= j){
			g2_done = true;
			std::cout << "g2 hit end" << std::endl;
		}
						
		}
		//If one of the 2 genome's ends have been reached
		else{ //check for excess genes
			//std::cout << "searching for excess genes" << std::endl;
			//sleep(5);
			
			if(g1_done == true){ //g2 must have the E gene
				std::cout << "adding excess gene from g2 to child...\n";
				std::cout << "g2 has size: " << g2.size() << "and j = " << j << endl;
				if( g1.getAdjusted() <= g2.getAdjusted() ){	
					Gene temp(g2.gene_vec[j]);
					
					child->gene_vec.push_back(temp);
				}
				j++;
			}		
			else{ //g1 has the E gene
				std::cout << "adding excess gene from g1 to child...\n";
				if( g1.getAdjusted() >= g2.getAdjusted() ){
                                        Gene temp(g1.gene_vec[i]);
                                        child->gene_vec.push_back(temp);
                                }
				i++;
			}

			if(g1.size() <= i){
				g1_done = true;
				
			}
			if(g2.size() <= j){
				std::cout << "g2.size() <= j" << endl;
				g2_done = true;
			}
	
		}
	}

	//return &child;
}

		


/**
 * @brief selects 2 parent genes within a species
 * returns the INDICES of the two parents in the species' genome_vec member
 * */
//int * matchMaker(Species s){
	//std::vector <int> parents = {0,0};
	
	

//}

/**
 * @brief calls all the functions related to reproduction for a given population 
 *
 * */
//void reproduce(Species * pop){
	//get size of population
	//for each Species
		//cull()
		//calcAdjusted() on survivors
		//determine # of offspring per species
		//for each offspring to make...
			//matchMaker()
			//crossOver
			//place child in species based on compat()

//}


