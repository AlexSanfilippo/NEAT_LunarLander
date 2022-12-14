/** @author Alexander Sanfilippo
 * @date 11-06-2022
 * @TODO clean up commented code and add comments*/

#ifndef GENE_H_4D4421E5
#define GENE_H_4D4421E5




class Gene {

	public:
		
		/*constructors */
		//Gene(); 
		Gene(unsigned int input, unsigned int output, double w, bool on){
			in = input;
			out = output;
			weight = w;
			enabled = on;
			innov_count += 1;
			innov = innov_count;
			//innovCountUp();
			//innov = innov_count;
		}

		/**
 		* @brief copy constructor for crossover: makes gene without increasing global innov_count;
 		* */
		/*??WHY?? was this ever commented out?  problems?
		Gene(Gene &g){
			in = g.in;
			out = g.out;
			weight = g.weight;
			enabled = g.enabled;
			innov = g.innov;	
			
		}
		*/
		/*  Antiquated, above constructor works fine
		Gene(unsigned int input, unsigned int output, double w, bool on, bool initial){
                        in = input;
                        out = output;
                        weight = w;
                        enabled = on;
			//static int innov_count;
			if(initial){
				innov_count = 1;
			}
                        //innovCountUp();
                        //innov = innov_count;
                }
		*/
		/*member functions-getters and setters*/
		/**
 		* @brief changes input 
 		* @param node the innov num. of the new input node
 		* */
		void setIn(unsigned int node){
			in = node; 
		}
		void setOut(unsigned int node){
			out = node;
		}
		/**
 		* @param weight the new weight value
 		* */
		void setWeight(double w){
			weight = w;	
		}
		void setEnabled(bool status){
			enabled = status;
		} 
		void setInnov(unsigned int inum){
			innov = inum;
		}
		void resetInnovCount(){
			innov_count = 0;
		}
		void setInnovCount(int n){
			innov_count = n;
		}
		//void setInnovCount(unsigned int n){innov_count = n;}
		int getIn(){return in;};
		int getOut(){return out;};
		double getWeight(){return weight;};
		bool getEnabled(){return enabled;};
		int getInnov(){return innov;}; 
		int getInnovCount(){ return innov_count; };
		/**
 		* @brief gives quick data on the gene
 		*
 		* */
		void summary(){ 
			std::cout << "#" << innov << ", "<< in << ", " << \
			out << ", "  << weight << ", " << enabled << "; ";
		}
		//void innovCountUp(){ innov_count += 1;};
		//int getInnovCount(){ return innov_count;};

	unsigned int in;  //what node it goes from 
	unsigned int out;  //what node it goes to
	double weight;  //weight of connection
	bool enabled;  //whether this connection is on or off
	unsigned int innov; //innovation number 
	static int innov_count; //Global count of all used innov numbers
};

#endif
