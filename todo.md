# Recommended Sirens Tasks
There are a number of tasks that can help improve Sirens and make it more useful to a broader audience. This document lists a few of these tasks.

## Modularity
Features need to be hard-coded within Sirens right now. A more appropriate model would be to use a common plugin format such as [LADSPA](http://www.ladspa.org/) or [Vamp](http://www.vamp-plugins.org/) and allow users of Sirens to select their own features. Currently, this can be done with examples/segmentcsv.cpp using arbitrary CSV input.

## Exceptions
None of this code throws exceptions. More error-checking needs to be done.

## Tests
There are currently no tests. 

## Feature extraction
1. **Algorithm documentation** - More complex features, such as Spectral Centroid, Transient Index, and Harmonicity need better code documentation. 

## Segmentation
1. **Algorithm documentation** - The algorithms used for segmentation could be more appropriately documented.
2. **Parameter documentation** - The parameters are not sufficiently documented as of yet.
3. **Viterbi beam search** - Currently, the most optimal mode sequence for segmentation is found by applying the full Viterbi algorithm to the network, which has complexity O(3<sup>F + 2</sup>N), where F is the number of featues and N is the length of the sound in frames. The beam search approximation is much more efficient (O(F<sup>2</sup>N)) and probably sufficient for segmentation.
4. **Parameter learning** - Techniques for parameter learning can help reduce the complexity of tuning Sirens segmentation. Currently, each feature has 12 parameters that need to be set. New environments or recording devices require retuning these parameters.

## Retrieval
1. **Algorithm documentation** - The algorithms used for retrieval could be more appropriately documented.
2. **Matrix optimization** - Currently, the retrieval code is the only part of Sirens that resorts to using a matrix library (uBLAS). Many matrices are passed by value, and some of the computations could be made more efficient either by abandoning uBLAS or by using more advanced features in uBLAS such as ranges and slices and different types of matrices. 
