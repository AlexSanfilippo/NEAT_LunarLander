



#Alexander Sanfilippo
#29-07-2022

#breif: This file is for testing running python on chuck with tensorflow
#and keras.  Once that is working, we can test calling and executing a python
#script from within a c++ function (to get a genome's fitness) 

#step 1:running python code on chuck - run with Python <script_name.py>
testVal = 1996
print("running python code.  testVal is", 1996, "\n")


#####What version of python is actually running?
import sys
print("running Python Version: ", sys.version)



#step 2: importing tensorflow and keras
#import tensorflow as tf
#from tensorflow import keras
import tensorflow as tf
print("TF Version: ", tf.__version__)
from tensorflow import keras
import numpy as np
from csv import reader #for reading csv
import gym
import time
import csv
import sys

print("imported all without error")
myarr = np.array([1,2,3,4,5])
print("created np array ", myarr)

sys.stdout.write('HELLO STDOUT!!!!! :)\n')
sys.stdout.flush()
sys.exit(0)

def main():
	mylist = [1,2,3,4,5,5,5,5]
	return mylist

#Step 3: call this python script from a c++ file
#step 4: try calling evaluate.py from testingGrounds.cc
	
#step 5: --DONE move on to writing NN building code
	#build NN with specific weights and nodal connections
		#may need to be a fully connected NN with some weights
			#set to "1", which is equivalent output-wise to
			#them not being there at all. 
#step 6: a complete sequential version of our program!



