'''
Data taken from KDD Cup 2001 
http://pages.cs.wisc.edu/~dpage/kddcup2001/
'''

import numpy
import random
import math

def load(normalize=False):
   trainFilename = 'thrombin.data'

   with open(trainFilename, 'r') as infile:
      dataAndLabels = [line.strip().split(',') for line in infile]
      labels = numpy.array([(1 if row[0] == 'A' else 0) for row in dataAndLabels])
      trainingData = numpy.array([[int(x) for x in row[1:]] for row in dataAndLabels])

   if normalize:
      return normalizeData(trainingData), labels

   return trainingData, labels

a, b = load()
print (a.shape)
print (b.shape)
'''
I need to print the transpose of a for it to work with my JL-code
'''
numpy.savetxt("thrombin_cleaned_1.data", a.T, delimiter=" ",fmt='%1.0f')


