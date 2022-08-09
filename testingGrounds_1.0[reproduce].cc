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
void matchMaker(Species s, int* parents);
void reproduce(std::vector <Species> *pop_ptr);



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
		

		
 		int NB1 = 20; //species NB1 size;
		int NB2 = 20; //species NB2 size
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
	
	/*TEST MATCHMAKER FXN*/
	
	int parents[2]  = {-1,-1};
	matchMaker(popB[0], parents);
	cout << "species 0 has size " << popB[0].size() << "and parents are at indices" <<\
		parents[0] << " and " << parents[1] << endl;

	/*TEST REPRODUCTION FXN*/
	reproduce(&popB);

	cout << "\n\n\n======After reproduce our population looks like this=======:\n";
	cout << "Number of Species:  " << popB.size() << "\n";
	cout << "Genomes per Species:\n";
	for(int j = 0; j < (int)popB.size(); j++){
		cout << "	Species " << j << " has " << popB[j].size() << " genomes\n";
	}
	cout << "Last species consists of these Genomes:\n";
	for(int i = 0; i < popB[popB.size()-1].size(); i++){ //for each genome in last species in pop
		popB[popB.size()-1].genome_vec[i].summary(); //print summary of genome
	}
				
	cout << "Second-to-Last species consists of these Genomes:\n";
	for(int i = 0; i < popB[popB.size()-2].size(); i++){ //for each genome in last species in pop
		popB[popB.size()-2].genome_vec[i].summary(); //print summary of genome
	}			
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
void matchMaker(Species s, int* parents){
	
	if(s.genome_vec.size() == 2){ //avoid long while loops if only 2 genomes
		parents[0] = 0;
		parents[1] = 1;
	}
	else if(s.genome_vec.size() > 2){ //make sure species has at least 2 member to breed
		//call rng
		auto uid = std::uniform_int_distribution<>(0,s.genome_vec.size() - 1); //-1 or no?
		parents[0] = uid(gen);	
		parents[1] = uid(gen);
		while(parents[0] == parents[1]){
			parents[1] = uid(gen);
		}
	}
	else{
		std::cout << " n < 2 genomes in this species" << std::endl;
		parents[0] = -1;
                parents[1] = -1;
	}

}

/**
 * @brief calls all the functions related to reproduction for a given population 
 *
 * */
void reproduce(std::vector <Species> *pop_ptr){
	
	/*TODO:  Need to make a new pop of just the species and their reps (but empty genome_vec's)
 	* Then we can run through the OLD pop and find the # children, make the chrildren, and then
 	* compare children to representatives in the new pop and put the children in the new pop's species.
 	* If a new species is created, it is added to the new pop.  Finally, the point to the old pop is 
 	* set to point to the new pop (thus replaced it) and the old pop is deleted from memory.*/

	std::vector<Species> &pop = *pop_ptr; //makes handling pop ptr easier

	/*HYPER PARAMETERS*/ //add these to a struct in a world_setting.h file later on
	int dist_threshold = 9; //not sure what to set this to
	int max_pop = 50;


	//get size of population
	int pop_size = pop.size();
	calcOffspring(&pop, max_pop); //works for all species already, no loop req.
	//for each Species-choose a representative genome
	for(int i = 0; i < pop_size; i++){
		pop[i].cull(); //ensures choose a fit rep
		pop[i].chooseRep();
	}
	
	//NEW POPULATION-for new generation	
	std::vector <Species> new_pop;
	new_pop = pop; //will this deep copy???

	//now make children for each species and speciate the children
	for(int i = 0; i < pop_size; i++){
		//for each offspring to make...
		for(int j = 0; j < pop[i].getOffspring(); j++){
			//matchMaker() to get two parents indices
			int parents[2] = {-1,-1};
			matchMaker(pop[i], parents);
			//crossOver
			Genome child = Genome(true); //create empty child
			//give child its parents' genetics
                	crossover(pop[i].genome_vec[parents[0]], pop[i].genome_vec[parents[1]], &child); 
                	int pop_size_dyn = new_pop.size(); //updates as new species are created;
			bool child_placed = false;
			
                	//loop through species again and loop at repr. genome to determine child's species
                	for(int k = 0; k < pop_size_dyn; k++){ 
				
				//TP-finding why infinite search for Excess Genes in Compat()
				std::cout << "just before compat is called in reproduce()"<< std::endl;
				std::cout << "CHILD SUMMARY:\n";
				child.summary();
				std::cout << "REP SUMMARY [k = "<< k <<", and pop_size (old) = "\
					 << pop_size << "]:\n";
                                new_pop[k].rep.summary();
				//sleep(3);
				double dist = compat(child/*cur child genome*/, new_pop[k].rep/*representative*/);
				if(dist <= dist_threshold){
					new_pop[k].genome_vec.push_back(child);
					child_placed = true;
				}
			}
			if(child_placed == false){ //if no compatible species found for this child
				//then make a new species with this child as its rep
				Species new_spec = Species(child); //create new species with child genome as first genome
				//above constructor handles setting the representative.
				new_pop.push_back(new_spec);
				std::cout << "new species created for child\n";
			}
		}
	}
	//pop_ptr = &new_pop; //return the new population --FAILS
	pop = new_pop; //TRY THIS 
	//pop.clear();//delete the old pop;
	//pop.shrink_to_fit();
	//pop = &new_popl
	std::cout << "new species count = " << new_pop.size() << std::endl;
	std::cout << "reached end of reproduce()\n";
}


