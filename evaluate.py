###Alexander Sanfilippo
###14th August, 2022
###TODO: get this running on chuck with NEAT
###     
        

#brief: Building NN function draft

#standart imports
import tensorflow as tf
from tensorflow import keras
import numpy as np
from csv import reader #for reading csv
import gym
import time
import csv

import sys

#use this to turn off annoying warnings
import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

#set seed to get same results on same NNs and same inputs



rank = sys.argv[1]

NUM_INPUTS = 8
NUM_OUTPUTS = 4

####CONTROLS AND HYPERPARAMETERS
seeds = [13]
max_steps = 1000 #max steps before forcing end of evaluation
set_max_steps = False #set max steps before evaluation ends
multiple_seeds = False #average evaluate over several seeds
do_set_seed = True #will turn off/on env.seed(seed)
if multiple_seeds:
	seeds = [101,98222]

for dummy in range(1):

    #1 create input nodes (the same for all genomes)
    input1 = tf.keras.Input(shape=(1,), name = 'INPUT_1')
    input2 = tf.keras.Input(shape=(1,), name = 'INPUT_2')
    input3 = tf.keras.Input(shape=(1,), name = 'INPUT_3')
    input4 = tf.keras.Input(shape=(1,), name = 'INPUT_4')
    input5 = tf.keras.Input(shape=(1,), name = 'INPUT_5')
    input6 = tf.keras.Input(shape=(1,), name = 'INPUT_6')
    input7 = tf.keras.Input(shape=(1,), name = 'INPUT_7')
    input8 = tf.keras.Input(shape=(1,), name = 'INPUT_8')

    

    #NODE_DICT--->holds key:value of node_number : layer
    node_dict = {}
    #add inputs to dictionary
    node_dict[1] = input1
    node_dict[2] = input2
    node_dict[3] = input3
    node_dict[4] = input4
    node_dict[5] = input5
    node_dict[6] = input6
    node_dict[7] = input7
    node_dict[8] = input8

    #the weights of each node.   layer_number : weight_list[]
    weight_dict = {}

    #neccessary for setting weights in right order
    layer_node_dict = {1:1,2:2,3:3,4:4,5:5,6:6,7:7,8:8}

    #merge the inputs  UNUSED
    #inputs = tf.keras.layers.Concatenate()([input1, input2, input3, input4])


    #for checking if we are constructing the output node since the
    #output layer must be created with different activation fxn
    #for binary output to AI GYM cart&pole
    output_node_numbers = [9,10,11,12]
    last_node_number = 12

    #list of all genome's fitnesses
    fit = []


    #read in all the nodes into a python vector called "node_list"
    node_list = []

    layer_name_list = [] #stores list of names of all named layers-used for weight setting part




    #node_row = []
    from csv import reader
    
    filename = 'genome_data' + rank + '.csv'
    with open(filename,'r') as read_obj:
        csv_reader = reader(read_obj)

        hit_END = False #if we finished making the NN
        for row in csv_reader:
            #print(len(row))
            #node_list.append(row)
            node_row = []
            
            for i in row:
                #print(i)
                
                if (i != '' and i != 'END'):
                    node_row.append(float(i))
                    #print("read in ", i, " and node_row is ", node_row)
                    
                    hit_END = False
                elif (i == 'END'): #build the NN and evaluate it
                    hit_END = True
                    tf.keras.backend.clear_session() #try to clear out old model completely

                    #clear node_dict except for inputs
                    node_dict.clear()
                    node_dict[1] = input1
                    node_dict[2] = input2
                    node_dict[3] = input3
                    node_dict[4] = input4
                    node_dict[5] = input5
                    node_dict[6] = input6
                    node_dict[7] = input7
                    node_dict[8] = input8
                    #print("read in ", i, " ===CONSTRUCTING NN BEGIN====")
                    #while we have not constructed all the nodes
                    layer_count = 8 #8 inputs established before loop
                    fully_constructed = False
                    output_layer = [0,0,0,0]
                    outputs_made = []

                    while fully_constructed == False:
                        #2 for each node (past the inputs) (index = i = node_number)
                        #print(len(node_list))
                        #print(node_list)
                        for k in range(len(node_list)): #for each node
                            input_node_list = [] #stores the layers for concatenate()
                            input_weight_list = []
                            #check if any of the input nodes do not exist yet
                            all_inputs_exist = True
                            #if rank == "1":
                                #print("rank ",rank," detected")
                            for j in range(int(node_list[k][1])):#for each input-node
                                cur_input = int(node_list[k][2+j])
                                #print("cur_input = ", cur_input)
                                if cur_input not in node_dict.keys():
                                    #if rank == "1":
                                        #print("input node [", cur_input, "]not created yet!")
                                    all_inputs_exist = False
                                    break
                                    
                            if(all_inputs_exist): #if all of this node's input nodes exist
                                #print("yes on all_inputs_exist for node ", int(node_list[k][0]))
                                #print("all inputs exist for this layer at node_list[",i,"], which are...")
                                for j in range(int(node_list[k][1])):#for each input-node
                                    #create list of all the inputs nodes's layers
                                    input_node_list.append(node_dict[int(node_list[k][2+j])])
                                    
                                    #create list of weights
                                    input_weight_list.append(node_list[k][2+int(node_list[k][1])+j])

                                    #print('node ', node_list[k][2+j],
                                    #      ', w=', node_list[k][2+int(node_list[k][1])+j])
                                
                                #add list of weights to weight dictionary for this node
                                weight_dict[int(node_list[k][0])] = input_weight_list
                                #create the new layer
                                if(int(node_list[k][0]) in output_node_numbers) and (int(node_list[k][0]) not in outputs_made ): #if at an output node
                                    int(node_list[k][0]) not in outputs_made #if we havnt already made this output
                                        
                                    #print("rank ", rank, ", creating output node # ", int(node_list[k][0]) )
##                                    #create 4 concatenation layers
##                                    for z in range(len(output_node_numbers)):
                                    layer_inputs = tf.keras.layers.Concatenate()(input_node_list) #not needed
                                    index_output = int(node_list[k][0]) - NUM_INPUTS-1 #tween 0 and 3 for output index
                                    #create output node for classification
                                    output_layer[index_output] = tf.keras.layers.Dense(1,
                                            name = str(int(node_list[k][0])), activation='sigmoid')(layer_inputs)
                                    layer_name_list.append(str(int(node_list[k][0])))
                                    #if rank == "1":
                                    #    print("created output layer with name", str(int(node_list[k][0])))
                                    node_dict[int(node_list[k][0])] = output_layer[index_output]
                                    #add output layer to the layer:node-number dictionary
                                    layer_node_dict[layer_count+1] = int(node_list[k][0])
                                    #NEW NEW Stop condition for noutputs > 1
                                    #stops if all output node numbers(ints) in a list outputs_made
                                    outputs_made.append(int(node_list[k][0]))
                                    
                                    if len(outputs_made) == 4:  #4 outputs
                                        fully_constructed = True

                                    #remove this node from the node_list
                                    #node_list.remove(node_list[k]) #s
                                 
                                    #NEW stop condition: stop if node list is empty
                                    #if not node_list:
                                     #   print("node list empty, stopping NN construction...")
                                      #  fully_constructed = True

                                    #only 1 output node, so we can end loop here
                                    #if(node_list[k][0] == last_node_number):
                                    #outputs_made = outputs_made + 1
                                    #if(outputs_made == 4):
                                     #   print("build ", outputs_made, " outputs, now fully constructed...")
                                      #  fully_constructed = True
                                elif(int(node_list[k][1]) == 1): #if only 1 input connection
                                    #print(len(input_node_list),"=len(input_node_list) before name")
                                    new_layer = tf.keras.layers.Dense(1, name = str(int(node_list[k][0])) )(input_node_list[0])
                                    layer_name_list.append(str(int(node_list[k][0])))
                                    #print("created layer with name", str(int(node_list[k][0])))
                                    #add new layer to the node dictionary
                                    node_dict[node_list[k][0]] = new_layer
                                    #add node # to layer-node dictionary
                                    layer_node_dict[layer_count+1] = int(node_list[k][0])
                                    #remove this node from the node_list
                                    node_list.remove(node_list[k]) #s
                                    #count this layer
                                    layer_count += 1
                                    break
                                else: #not output and >1 input connection
                                    #print(len(input_node_list),"=len(input_node_list)")
                                    layer_inputs = tf.keras.layers.Concatenate()(input_node_list)
                                    new_layer = tf.keras.layers.Dense(1, name = str(int(node_list[k][0])))(layer_inputs)
                                    layer_name_list.append(str(int(node_list[k][0])))
                                    #print("created layer with name", str(int(node_list[k][0])))
                                    #add new layer to the node dictionary
                                    node_dict[node_list[k][0]] = new_layer
                                    #add node # to layer-node dictionary
                                    layer_node_dict[layer_count+1] = int(node_list[k][0])
                                    #remove this node from the node_list
                                    node_list.remove(node_list[k]) #
                                    #count this layer
                                    layer_count += 1
                                    break


                    
                    #(step N)build the model
                    #print("\n\n ++++OUTPUT LAYER LIST MEMBERS rank", rank)
                    #print("1: ",type(output_layer), ",  ", type(output_layer[0]))
                    #print("2: ", type(output_layer[1]))
                    #print("3: ", type(output_layer[2]))
                    #print("4: ", type(output_layer[3]))
                    model = tf.keras.Model(inputs=[input1, input2, input3, input4,input5, input6, input7, input8],outputs=[output_layer[0],output_layer[1],output_layer[2],output_layer[3]])



    ##                ###graph the model
    ##                img_file = 'BIG' + str(len(fit)) + '.png'
    ##                tf.keras.utils.plot_model(model, to_file=img_file, show_shapes=True)

                    ##graph the model with pydot
                    #dot_model = tf.keras.utils.model_to_dot(
                    #    model,rankdir="LR", dpi=200) #convert to pydot
                    ##dot_model.set_bgcolor('lightyellow')
                    #dot_img_file = 'DOTGRAPH' + str(len(fit)) + '.png'
                    #dot_model.write_png(dot_img_file)
                    


    ##                ####check we did this correctly-print the weights of each layer
    ##                for cur in model.layers:
    ##                    print(cur.get_weights())
                    


                            
                    #Set the Weights--Using Layer name
                    for name in layer_name_list:
                        cur_layer = model.get_layer(name)
                        weights = cur_layer.get_weights()
                        if(len(weights) != 0):
                            new_weights = weight_dict[int(name)]

                            
                            for i in range(len(weights[0])): 
                                weights[0][i] = new_weights[i]
                            cur_layer.set_weights(weights)
                    

                            
    ##                ####check we did this correctly-print the weights of each layer
    ##                for cur in model.layers:
    ##                    print(cur.get_weights())     


    ###### AI GYM ######
                    fitness_sum = 0
                    for seed in seeds:
                        #fitness of this genome
                        fitness = 0

			    #create environment for cart and pole
	##                    env = gym.make('BipedalWalker-v3', render_mode='human',
	##                                   new_step_api=True)
			    #env = gym.make('BipedalWalker-v3', new_step_api=True)
			    
			    #env = gym.make('BipedalWalker-v3') #run without rendering

                        env = gym.make('LunarLander-v2');
                        #redefine observation if do_set_seed:
                        if do_set_seed:
                            env.seed(seed=seed)

			    #change max episode length
                        if set_max_steps:
                            env._max_episode_steps = max_steps


			    #get initial state
                        state = env.reset()

                        terminated = False
                        while(not terminated):
                            #convert state to proper format for NN
                            state_list = [0,0,0,0,0,0,0,0]
                            for i in range(NUM_INPUTS):
                                elm = np.array([state[i]]);
                                state_list[i] = elm
				
                            #get action from our NN
                            action = model.predict(state_list)
                            #do data type converstions from keras model to gym
                            action = list(action)
                            true_action = action.index(max(action))
				
				#env.render() #neccesary in this version
				#take action in the env, get reward and new state
                            state, reward, terminated, info = env.step(true_action)
                            fitness += reward

				

				#if we want to slow down the visuals
				#time.sleep(0.001)
                        env.close()
			
			#scale fitness between 0 and about 270
                        #if fitness < -200:
                        #    fitness = 0
                        #else:
                        #    fitness = fitness + 200
                        fitness = fitness + 1000
                        #print("fitness for seed ", seed, " is ", fitness)
                        fitness_sum = fitness_sum + fitness
                    fitness = fitness_sum/len(seeds) #get averate fitness of mulitple trials
                    fit.append(fitness) #add min possible fitness to offset negatives

            
                    node_list.clear();
                    node_row.clear();
                #keras.backend.clear_session() #try to clear out old model completely   
            #print(node_row)
            layer_name_list.clear()
            
            if(hit_END == False):
                node_list.append(node_row)
        
    
#write fitnesses to proc specific file
print("fit in rank ",rank ," = ", fit)
#write fit to file
fitfile = "fitness" + rank + ".csv";
#file = open('fitness.csv', 'w')
file = open(fitfile, 'w')
writer = csv.writer(file)
writer.writerow(fit)
file.close()
        


