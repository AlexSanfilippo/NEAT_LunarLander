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

	/*TESTING GENE*/
	Gene myGene(1,2,0.6, true);
	std::cout << "Gene created" << std::endl;	
	std::cout << "gene goes from "<< myGene.getIn() << " to "\
		 << myGene.getOut() << " with weight " << myGene.getWeight()\
		<< ", with innov. number " << myGene.getInnov() <<std::endl;
	Gene geneA(2,3,0.36, true);
	Gene geneB(1,5,0.45, true);
	std::cout << "geneB goes from "<< geneB.getIn() << " to "\
                 << geneB.getOut() << " with weight " << geneB.getWeight()\
                << ", with innov. number " << geneB.getInnov() <<std::endl;

	/*Vector Basics*/

	std::vector <int> vec; //create a vector to store ints
	std::vector <int> vec2 = {1,2,3,4,5}; //create and init vector
	vec = vec2; //copy one vec to another 
	auto vec3(vec2); //copy constructor usage

	cout << "front of vector is " << vec.front() <<"\nback is " << vec.back() <<\
		"\nelement at [1] is " << vec.at(1) << " or " << vec[1] << endl;	
	vec.push_back(6);
	cout << " after push_back(), back is " << vec.back() << endl;

	/*Vector of Genes: The Genome*/
	Gene A(1,2,0.456, true);
	Gene B(2,3, 0.899, true);

	vector <Gene> genome1;
	genome1.push_back(A);
	genome1.push_back(B);
	cout << "genome1 has length " << genome1.size() << endl; 
	A.summary();
	cout << endl;
	B.summary();
	cout << endl;

	/*Genome Class*/
	Genome g1;
	Genome g2a;
	Genome g2b;
	cout << "genome g1 has as its first gene: ";
	g1.get_gene_at(0);
	cout << endl;

		/*Mutations using the mutate() function*/
		g1.mutate(); 
		g1.summary();
		g1.mutate();
		g1.summary();

	/*CROSSOVER*/

		//int d_t = 5;  //compatibility threshold
		
		/*compat() function*/
		cout << "g2a and g2b summary() pre compat()" << endl;
		g2a.summary();
		g2b.summary();
		double d = compat(g2a, g2b);
		Genome g3, g4;
		
		double d2 = compat(g3, g4);
		g3.mutate();
		g3.mutate();
		g3.mutate();
                g3.mutate();
		cout << "g3 summary():\n";
		g3.summary();
		d2 = compat(g3, g4);
		cout << "Genome g1 and g2 have difference = " << d << std::endl;
		cout << "Genome g3 and g4 have difference = " << d2 << std::endl;

		/*Species class*/
		Species dog;
		Genome g_cat;
		Species cat(g_cat);
		Genome g_tabby;
		cat.addGenome(g_tabby);
		cat.addGenome(g3);	
		cout << "Species \"cat\" has g3:"  << endl;
		cat.genome_vec[2].summary();
		cat.genome_vec[0].mutate();
		cout << "Species \"cat\" has representative:"  << endl;
                cat.genome_vec[0].summary(); 

		/*creating an initial population of Genomes in Species*/
		int N = 12; //initial pop size;
		
		std::vector <Species> pop;
		Species init_species;
		for(int i = 0; i < N; i++){
			Genome newGenome; //construct new default genome
			//newGenome.calcAdjusted(N);
			init_species.genome_vec.push_back(newGenome);
		}
		pop.push_back(init_species); //add species to population
		
		/*testing crossover()*/
		for(int i = 0; i < 9; i++){
			pop[0].genome_vec[0].mutate();
		}
		cout << "PARENT ONE:\n";
		pop[0].genome_vec[0].summary();
		pop[0].genome_vec[0].dummyFitness(); // set fitness
		//cout << "after dummyFitness() fitness is" << pop[0].genome_vec[0].getFitness() << endl;
		pop[0].genome_vec[0].calcAdjusted(N); //set adj fitness

		cout << "adjusted: "<< pop[0].genome_vec[0].getAdjusted() << endl;
		for(int i = 0; i < 3; i ++){
			pop[0].genome_vec[1].mutate();
		}
		cout << "PARENT TWO\n";
		pop[0].genome_vec[1].summary();
		pop[0].genome_vec[1].dummyFitness();
		//cout << "after dummyFitness() fitness is" << pop[0].genome_vec[1].getFitness() << endl;
		pop[0].genome_vec[1].calcAdjusted(N);
		cout << "adjusted: " << pop[0].genome_vec[1].getAdjusted() << endl;
		Genome child(true);
		crossover(pop[0].genome_vec[0], pop[0].genome_vec[1], &child);
		pop[0].genome_vec.push_back(child);
		N++;
		cout << "CHILD:\n";
		child.summary();

		/*Culling and Sorting*/
		//Mutate all the genomes in the species
		int num_mut = 10;
		for(unsigned long int i = 0; i < pop[0].genome_vec.size(); i++){
			for(int j = 0; j < num_mut; j++){
				pop[0].genome_vec[i].mutate();
			}
			//get fitness
			pop[0].genome_vec[i].dummyFitness();
			pop[0].genome_vec[i].calcAdjusted(N);
		}

		/*TESTING calcOffspring()*/
		
		//1) add another species to the pop
		Species sec_species;
		int N2 = 12; //size of second species
		cout << "creating new species...\n";
		for(int i = 0; i < N2; i++){ //fix seg with N2+1. why?
			Genome newGenome; //construct new default genome
			sec_species.genome_vec.push_back(newGenome);
		}
		cout << "done populating new species...\n";
		//2) calc fitness and adjusted fitness for second species
		pop.push_back(sec_species); //add species to population
		cout << "added 2nd species to pop...\n";
		for(int i = 0; i < pop[1].size(); i++){ //mutate species [1]
			for(int j = 0; j < N2; j++){ //add mutations
				pop[1].genome_vec[j].mutate();
			}
			pop[1].genome_vec[i].dummyFitness();
			pop[1].genome_vec[i].calcAdjusted(N2);	
		}
		cout << "calculated adj fitness & fitness on 2nd species...\n";
		

		//3) CULL
		//Maybe pop[1] is wrong type somehow?
		if ( typeid(pop[0]) == typeid(pop[1]) ){
			cout << "both pop elms are same type\n";
		}

		cout << "\n---Species [0] pre-cull()---\n";
		for(int i = 0; i < pop[0].size(); i++){
		        cout << "spec 0 genome " << i << endl;
                        pop[0].genome_vec[i].summary();
                }

		cout << "\n\n===============\nCull Species [0]\n";
		pop[0].cull(); //working, no issues
		cout << "\n---Species [0] post-cull()---\n";
                for(int i = 0; i < pop[0].size(); i++){ //view spec 0 genomes post-cull()
                        cout << "spec 0 genome " << i << endl;
                        pop[0].genome_vec[i].summary();
                }
		
		//try viewing species 2 to observe errors
		cout << "\n---Species [1]---\n";
		for(int i = 0; i < pop[1].size(); i++){
			cout << "spec 1 genome " << i << endl;
			pop[1].genome_vec[i].summary();
		}
		cout << "\n\n===============\nCull Species [1]\n";
		pop[1].cull();
		cout << "completed cull() on 2nd species\n";
		for(int i = 0; i < pop[1].size(); i++){
                        cout << "spec 1 genome " << i << endl;
                        pop[1].genome_vec[i].summary();
                }
		cout << "after cull, spec 0 has size " << pop[0].size() << " and spec 1 has size " <<\
			pop[1].size() << endl;
		//for(int i = 0; i < pop.size(); i++){
		//	pop[i].cull(); //CULL
		//}
		cout << "culled both species...\n";
		//4)CALC OFFSPRING PER SPECIES
		calcOffspring(&pop, 12); //CALC OFFSPRING COUNT for all SPECIES
		//5)COMPLETE REPRODUCTION CYCLE
		
		cout << "after cull, before step 5...\n";
		//for(long unsigned int i = 0; i < pop.size(); i++){ //for each species to repop
		//	for(int j = 0; j < pop[i].getOffspring(); j++){ //for each offspring to make per species
			//draw two random distinct indices of genomes
			//create empty child Genome
			//call crossover to write child
			//add child to species
		//	}
		//} 


		/*TESTING CULL FUNCTION*/
		/*
		cout << "pre-cull genomes\n";	
		for(unsigned long int i = 0; i < pop[0].size(); i++){ //print genomes in species 0
                        cout << "genome #" << i+1 << ": f=" << pop[0].genome_vec[i].fitness << \
                        ", adj=" << pop[0].genome_vec[i].adjusted << endl;
                }
		pop[0].cull();	
		cout << "after cull(), pop size is " << pop[0].size();
		cout << "after cull(), pop consists of: \n";
		
		for(unsigned long int i = 0; i < pop[0].size(); i++){ //print genomes in species 0
                        cout << "genome #" << i+1 << ": f=" << pop[0].genome_vec[i].fitness << \
                        ", adj=" << pop[0].genome_vec[i].adjusted << endl;
                }
		*/
		/* //TESTING SORT FUNCTION
		cout << "species 0 before sort:\n";
		for(unsigned long int i = 0; i < pop[0].genome_vec.size(); i++){ //print genomes in fitness order
                        cout << "genome #" << i+1 << ": f=" << pop[0].genome_vec[i].fitness << \
                        ", adj=" << pop[0].genome_vec[i].adjusted << endl;
                }
		
		cout << "species 0 after sort:\n";
		pop[0].sort(pop[0].genome_vec, 0, N-1);
		for(unsigned long int i = 0; i < pop[0].genome_vec.size(); i++){ //print genomes in fitness order
			cout << "genome #" << i+1 << ": f=" << pop[0].genome_vec[i].fitness << \
			", adj=" << pop[0].genome_vec[i].adjusted << endl; 
		}
		*/
		
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


