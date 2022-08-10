/**
 * @author Alex S
 * @date 11-06-2022
 * last updated 04-08-2022
 *This file will allow us to test things as we set up the Gene, Genome, and Species classes
 * */



#include <iostream>
#include <random>

//for writing to/reading from files
#include <fstream>
#include <string>
#include <sstream>

//for calling another executable
#include <stdlib.h>

//for timing
#include <chrono>

//my headers
#include "gene.h"
#include "Genome.h"
#include "Species.h"
#include "NOV.h"
#include "crossover.h"


//REMOVE LATER
#include <typeinfo>

//for python
//#include "fitness.h"


//put this function in crossover.h later
void crossover(Genome g1, Genome g2, Genome * child);
void matchMaker(Species s, int* parents, double avg_fitness);
void reproduce(std::vector <Species> *pop_ptr, const int MAX_POP, NOV *nov);



int Gene::innov_count{ 0 };

using namespace std;
int main(){
		

	/*==========  Version 1.1 of Main Loop  =========*/
	//created 25-07-2022
	//updated 04-08-2022
		

	/*SIMULATION HYPERPARAMETERS*/
	const int POP_SIZE  = 18; //number of genomes in the whole population
	const int NUM_MUT = 1; //number of mutation
	const int NUM_GEN = 5; //number of generations	
	int gen_count = 0;
	NOV nov(4,1); //create Nodal Order Vector object
	
	std::vector <Species> pop; //population, a vector of species
	Species species_init = Species(0); //the initial species
	for(int i = 0; i < POP_SIZE; i++){
		Genome newGenome; //create new default genome
		species_init.genome_vec.push_back(newGenome); //add genome to the initial spec.
	}
	pop.push_back(species_init); //add species to population

	
	ofstream timefp;
	timefp.open("eval_timings.csv"); //open file for timings

	ofstream neat_time_fp;
        neat_time_fp.open("neat_timings.csv");

	ofstream fit_fp;
	fit_fp.open("pop_avg_fitness.csv");
	//Mutate all the genomes in the species & calc fitness
	for(int g = 0; g < NUM_GEN; g++){ //each gen
		auto start_gen_a = std::chrono::high_resolution_clock::now();
		for(long unsigned int h = 0; h < pop.size(); h++){ //each species
			for(unsigned long int i = 0; i < pop[h].genome_vec.size(); i++){ //each genome
				for(int j = 0; j < NUM_MUT; j++){ //each mutation
					pop[h].genome_vec[i].mutate(&nov);
					//std::cout << "sleeping after mutate in main()\n";
					//sleep(1);
				}
				//get fitness
				//pop[h].genome_vec[i].dummyFitness();
				//pop[h].genome_vec[i].calcAdjusted(pop[h].genome_vec.size()); //?
			}
		}
		//std::cout <<"[MainLoop] called mutate() on all genomes in pop\n";
		/*Write all Genomes to a File*/
		char filename[] = "genome_data.csv";
		ofstream fp;
		fp.open(filename);
		int genome_count = 0;
		//for each species, and each genome within, calcNodeInfo, then writeToFile
		for(int i = 0; i < int(pop.size()); i++){//for each species 
			for(int j = 0; j < pop[i].size(); j++){//for each genome
				pop[i].genome_vec[j].calcNodeInfo(&nov);
				pop[i].genome_vec[j].writeToFile(&fp, genome_count);
				genome_count += 1;
			}

		}
		fp.close();
			
		/*Call python script to evaluated all genome's fitnesses*/
		auto stop_gen_a = std::chrono::high_resolution_clock::now(); //stop timing before eval
		auto start = std::chrono::high_resolution_clock::now();
		system("./calls_evaluate");
		//std::cout <<"[MainLoop] called evaluate.py\n";
		auto stop = std::chrono::high_resolution_clock::now();
		auto start_gen_b = std::chrono::high_resolution_clock::now(); //start timing after eval
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		timefp << "GEN " << g << ": " << double(duration.count())/1000000 << "\n";
		//std::cout << "GEN " << g << ": " << duration.count()/1000000 << ",\n";	

		/*Read the fitnesses from the file and assign to genomes' members*/
		fstream fpin; //create file pointer
		fpin.open("fitness.csv", ios::in); //open the file
		string line, score;
		getline(fpin, line);//reads all values and stores in "line"
		stringstream s(line);
		for(int h = 0; h < int(pop.size()); h++){//for each species
			for(int i = 0; i < int(pop[h].genome_vec.size()); i++){ //each genome
				//read fitness from the file
				getline(s,score,',');
				//do genome.setFitness(fitness)
				//cout << "read in fitness score " << score << endl;
				pop[h].genome_vec[i].setFitness(stoi(score));
				//calcAdjustedFitness	
				pop[h].genome_vec[i].calcAdjusted(pop[h].genome_vec.size()); 
			}	

		}
		fpin.close();

		/*calc species' and global average fitness scores*/
		for(int h = 0; h < int(pop.size()); h++){
			pop[h].calcFitness();
		}			
		//calcAverageFitness(); //called within reproduce
			
		cout << "====== Generation " << gen_count << "/" << NUM_GEN <<" ======\n";
		cout << "Number of Species:  " << pop.size() << "\n";
        	cout << "Number of Genomes: " << calcTotalPop(&pop) << std::endl;
		double pop_avg_fitness = calcAverageFitness(&pop);
		cout << "Population Avg. Fitness (pre-cull): " << pop_avg_fitness << endl;	
		fit_fp << pop_avg_fitness << endl;	
		cout << "Species Stats:\n";
		for(int j = 0; j < (int)pop.size(); j++){
        	        cout << "       Species " << j << ": size: " << pop[j].size() << ", avg Fit: "\
				<< pop[j].getFitness() << "\n";
	        }
 


		/*reproduce (create new population from fittest of old pop)*/
		reproduce(&pop, POP_SIZE, &nov);
		//std::cout <<"[MainLoop] called reproduce\n";
		gen_count++;
		auto stop_gen_b = std::chrono::high_resolution_clock::now(); //stop timing before eval
		auto neat_duration_a = std::chrono::duration_cast<std::chrono::microseconds>(stop_gen_a - start_gen_a); 
		auto neat_duration_b = std::chrono::duration_cast<std::chrono::microseconds>(stop_gen_b - start_gen_b);
                neat_time_fp << "GEN " << g << ": " << double((neat_duration_a.count() + neat_duration_b.count())) / 1000000 << "\n";
		//std::cout << "nt GEN " << g << ": " << double(neat_duration_b.count()) / 1000 << "\n";
	
	}
	neat_time_fp.close(); //close fp for writing time of NEAT execution/Generation
	timefp.close(); //close file pointer for recording evaluation times/generation
	fit_fp.close();
	/*END OF MAIN LOOP: printing results to confirm code working*/
	
	/*printing results...*/
	cout << "\n\n\n======After Main Loop our population looks like this after "<<gen_count\
		<<" generations =======:\n";
	cout << "Number of Species:  " << pop.size() << "\n";
	cout << "Number of Genomes: " << calcTotalPop(&pop) << std::endl;
	cout << "Genomes per Species:\n";
	for(int j = 0; j < (int)pop.size(); j++){
		cout << "	Species " << j << " has " << pop[j].size() << " genomes\n";
	}
	 //print the last two species's genomes
	/*
	cout << "Last species consists of these Genomes:\n";
	for(int i = 0; i < pop[pop.size()-1].size(); i++){ //for each genome in last species in pop
		pop[pop.size()-1].genome_vec[i].summary(); //print summary of genome
	}
				
	cout << "Second-to-Last species consists of these Genomes:\n";
	for(int i = 0; i < pop[pop.size()-2].size(); i++){ //for each genome in last species in pop
		pop[pop.size()-2].genome_vec[i].summary(); //print summary of genome
	}
	*/
	//nov.summary();
	
	/*TESTING WRITE NIV from GENOME.H*/
	//cout << "\n--writing NIV for first genome of first species--\n";
	//pop[0].genome_vec[0].calcNodeInfo(&nov);
	
	/*TESTING WRITING GENOME TO FILE*/
	/*
	char filename[] = "genome_data.csv";
	ofstream fp;
	fp.open(filename);
	for(int i = 0; i < 3; i++){
		pop[0].genome_vec[0].writeToFile(&fp, 0);
	}
	fp.close();	
	*/


	return 0;
		
}
/**
 * @brief produces offspring genome between 2 adults in a species
 * @param g1 the first parent gene
 * @param g2 the second parent gene
 * */
void crossover(Genome g1, Genome g2, Genome *child){

	//std::cout << "inside crossover()...\n";
	
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
		//std::cout << "b4 first if\n";
		if(g1_done == false && g2_done == false){
			//std::cout << "after first if\n";
			//std::cout << "b4 2nd if\n";
			//std:: cout << "g1.gene_vec[i].getInnov()   : "<< g1.gene_vec[i].getInnov() << std::endl;
			//std:: cout << "g2.gene_vec[j].getInnov()   : "<< g2.gene_vec[j].getInnov() << std::endl;
			if(g1.gene_vec[i].getInnov() == g2.gene_vec[j].getInnov()){ //matching genes
				//std::cout << "after 2nd if\n";
				//pick parent to inherit the gene from
				if(urd(gen) < 0.5){
					Gene gene(g1.gene_vec[i]); //cpy construct gene
					child->gene_vec.push_back(gene); //add gene to child
					
					if(child->gene_vec.back().getEnabled() == false){
						if(g1.getAdjusted() > g2.getAdjusted()){//if disabled gene has higher fitness
							if(urd(gen) > 0.75){ //25% odds to switch to enabled
								child->gene_vec.back().setEnabled(true);
							}
						}
						else{
							child->gene_vec.back().setEnabled(true);
						}
					}
				}	
				else{
					Gene gene(g2.gene_vec[j]); //cpy construct gene
                                        child->gene_vec.push_back(gene); //add gene to child
					
					if(child->gene_vec.back().getEnabled() == false){
						if(g2.getAdjusted() > g1.getAdjusted()){//if disabled gene has higher fitness
							if(urd(gen) > 0.75){ //25% odds to switch to enabled
								child->gene_vec.back().setEnabled(true);
							}
						}
						else{
							child->gene_vec.back().setEnabled(true);
						}
					}
					
				}
			
				//disabled check--OLD
				/*
				//std::cout << "b4 3rd if\n";			
				if(g1.gene_vec[i].getEnabled() == false || g2.gene_vec[j].getEnabled() == false ){
					//std::cout << "after 34d if\n";
					if(urd(gen) < 0.75){
						//set gene to disabled in child
						child->gene_vec.back().setEnabled(true);
						// *last->setEnabled(false);
					}
					else{
						//set gene to enabled in child
						child->gene_vec.back().setEnabled(true);
                                                //last.setEnabled(true);
					}
				}
				*/
				i++;
				j++;
			}	
			//handle disjoints
			else{ //the innov numbers do not match
				if(g1.gene_vec[i].getInnov() > g2.gene_vec[j].getInnov()){ //if D in g2
					//determine which parent is fitter
					if( g1.getAdjusted() <= g2.getAdjusted() ){
						//inherit the Disjoint Gene
						Gene gene ( g2.gene_vec[j] );
						child->gene_vec.push_back(gene);
					}
					
					j++;	
					//std::cout << "disjoint gene from g2 added to child" << std::endl;
				}
				else{ //if D in g1
					
					if( g1.getAdjusted() >= g2.getAdjusted() ){
						//inherit the Disjoint Gene
						Gene gene ( g1.gene_vec[i] );
						child->gene_vec.push_back(gene);
					}
					i++;
					//std::cout << "disjoint gene from g1 added to child" << std::endl;
				}
			}
			//Check for end of either Genome reached
			if(g1.size() <= i){
				g1_done = true;
				//std::cout << "g1 hit end" << std::endl;
			}
			if(g2.size() <= j){
				g2_done = true;
				//std::cout << "g2 hit end" << std::endl;
			}
						
		}
		//If one of the 2 genome's ends have been reached
		else{ //check for excess genes
			//std::cout << "searching for excess genes" << std::endl;
			//sleep(5);
			
			if(g1_done == true){ //g2 must have the E gene
				//std::cout << "adding excess gene from g2 to child...\n";
				//std::cout << "g2 has size: " << g2.size() << "and j = " << j << endl;
				if( g1.getAdjusted() <= g2.getAdjusted() ){	
					Gene temp(g2.gene_vec[j]);
					
					child->gene_vec.push_back(temp);
				}
				j++;
			}		
			else{ //g2 == done, so g1 has the E gene
				//std::cout << "adding excess gene from g1 to child...\n";
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
				//std::cout << "g2.size() <= j" << endl;
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
void matchMaker(Species s, int* parents, double avg_fitness){
	//int s_size = s.genome_vec.size();
	//std::cout << "[matchMaker()] with species of size " << s_size << std::endl;
	if(s.genome_vec.size() == 2){ //avoid long while loops if only 2 genomes
		//std::cout << " exactly 2 genomes in this species\n";
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
	else if(s.genome_vec.size() < 1){
		//std::cout << " n < 2 genomes in this species" << std::endl;
		parents[0] = -1;
                parents[1] = -1;
	}
	//SEXUAL REPRODUCTION
	else if(s.genome_vec.size() == 1 && s.getFitness() > avg_fitness){
		//std::cout << "performing self-fertilization reproduction";
		parents[0] = 0;
		parents[1] = 0;
	}
	//kill unfit species with 1 genome
	else if(s.genome_vec.size() == 1 && s.getFitness() < avg_fitness){
		parents[0] = -1;
                parents[1] = -1;

	}	


}

/**
 * @brief calls all the functions related to reproduction for a given population 
 *
 * */
void reproduce(std::vector <Species> *pop_ptr, const int MAX_POP, NOV *nov){
	
	//std::cout << "in reproduce()...\n";	

	Gene ic_gene2 = Gene(0,0,0,true);
	int old_innov_count = ic_gene2.getInnovCount();
	
	//std::cout << "in reproduce() old_innov_count is " << old_innov_count << std::endl;

	std::vector<Species> &pop = *pop_ptr; //makes handling pop ptr easier

	//calc population average fitness
	double pop_avg_fitness = calcAverageFitness(pop_ptr); //crossover.h


	/*HYPER PARAMETERS*/ //add these to a struct in a world_setting.h file later on
	int dist_threshold = 25; //not sure what to set this to
	


	//get size of population
	int pop_size = pop.size();
	calcOffspring(&pop, MAX_POP); //works for all species already, no loop req.
	//for each Species-choose a representative genome
	for(int i = 0; i < pop_size; i++){
		if(pop[i].size() > 2){ //why was this ever 4? changed to 2	
			pop[i].cull(); //ensures choose a fit rep
			pop[i].chooseRep(); //choose a random new repr genome
		}	
	}
	
	
	//NEW POPULATION-for new generation	
	std::vector <Species> new_pop;
	Species temp_spec = Species(old_innov_count);
	int sec_index = 0;
	for(long unsigned int i = 0; i < pop.size(); i++){ //for each species in old pop
		if(!(pop[i].genome_vec.empty())){ //if this old species had genomes
			Species empty_spec = Species(old_innov_count);
			new_pop.push_back(empty_spec); //add empty species to new pop
			new_pop[sec_index].rep = pop[i].rep; //deep copy?
			sec_index++;
		}
		else{
			//std::cout << "an empty species (" <<i<< ") was skipped\n";
			//sleep(2);
		}
	}
	//std::cout << "finished adding old species to new_pop, skipping empty species\n";

	int child_count = 0;

	//now make children for each species and speciate the children
	for(int i = 0; i < pop_size; i++){
		//for each offspring to make...
		for(int j = 0; j < pop[i].getOffspring(); j++){
			//matchMaker() to get two parents indices
			int parents[2] = {0,0};
			//std::cout << "calling matchMAker..."<<std::endl;
			matchMaker(pop[i], parents, pop_avg_fitness);
			//std::cout << "called matchMaker." << std::endl;
			//crossOver
			Genome child = Genome(true); //create empty child
			child_count++;
			//give child its parents' genetics
			/*std::cout << "calling crossover on species " << i << "/" << pop_size << \
				" and parents at indices " << parents[0] << ", " << parents[1]\
				<< std::endl;
			*/
			

			if(!(parents[0] == -1 || parents[1] == -1)){	
                		//std::cout << "calling crossever..."<<std::endl;
				if(parents[0] != parents[1]){
					//Genome child = Genome(true); //create empty child
					crossover(pop[i].genome_vec[parents[0]], pop[i].genome_vec[parents[1]], &child); 
				}
				//asexual reproduction v2				
				else if(parents[0] == parents[1]){ 
					/*std::cout << "attempting asexual reproduction in species " << i << "parents[0] = "<<\
						parents[0] << std::endl; */
					//just add the same identical gene to the species
					child = pop[i].genome_vec[parents[0]]; //child is CLONE of parent
					//std::cout << "cloned the child\n";
					//new_pop[i].genome_vec.push_back(child_ace); //place child in parent species
					//std::cout <<"\ntriggered asexual reproduction in species " << i << "\n";
				}

				//std::cout << "[reproduce()] after crossover()\n";
				int pop_size_dyn = new_pop.size(); //updates as new species are created;
				bool child_placed = false;
				//SPECIATE THE CHILD
				//loop through species again and loop at repr. genome to determine child's species
				for(int k = 0; k < pop_size_dyn; k++){ 
					
					double dist = compat(child/*cur child genome*/, new_pop[k].rep/*representative*/);
					if(dist <= dist_threshold){
						new_pop[k].genome_vec.push_back(child);
						child_placed = true;
						//std::cout << "Placed child " << child_count << " in species " << k << std::endl;
						k = pop_size_dyn; //stop adding the child to species
					}
				}
				
				if(child_placed == false){ //if no compatible species found for this child
					//std::cout << "adding new species..."<<std::endl;
					//then make a new species with this child as its rep
					Species new_spec = Species(child, old_innov_count); //create new species with child genome as first genome
					//above constructor handles setting the representative.
					new_pop.push_back(new_spec);
					//std::cout << "added new species."<<std::endl;
					//std::cout << "new species created for child\n";
					//std::cout << "Placed child " << child_count << " in NEW species " << std::endl;
				}
			}
			//this else block is TP only
			else {
				std::cout << "caught -1 parents at "<< i << "/" << pop_size << std::endl;
			}
		}
	}
	pop = new_pop; //TRY THIS 
	//std::cout << "reached end of reproduce()\n";
}


