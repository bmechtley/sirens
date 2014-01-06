## Sirens
Sirens is a library for segmentation, indexing, and retrieval of environmental and natural sounds. In other words, it allows you to break long field recordings into individual sonic "events" and then compare those events in terms of content-based similarity.

In the `vamp/` folder, you can also find implementations of the individual temporal/spectral features used in Sirens as [Vamp](http://vamp-plugins.org) plugins. 

## Installation
Sirens relies on [FFTW](http://www.fftw.org) (for spectral analysis) and [SCons](http://www.scons.org) (for installation). To install, perform the following command in the project's directory:

	sudo scons install

To build the applications in the examples/ directory, do

	scons

### Parameters
If you do not have root access, you may have troubles installing Sirens. In this case, you can put the library in a custom directory. For example, to put the library in ~/lib and the includes in ~/include/sirens:

	scons install --prefix=$HOME

Additionally, if you need to point the Sirens install to a specific directory to find FFTW headers, you can specify any additional parameters to the compiler using $CFLAGS and $LDFLAGS. For example, to use ~/include/fftw3.h, ~/lib/fftw3.a, and so on:

	export CFLAGS=-I$HOME/include
	export LDFLAGS=-L$HOME/lib
	scons install --prefix=$HOME

## Using Sirens
Any application using Sirens also needs to link against [FFTW](http://www.fftw.org) and pthread:

	g++ myproject.cpp -lsirens -lfftw3 -lpthread

A tutorial will come eventually. See the examples/ directory for some ideas.

## Upcoming changes
There are a number of tasks planned for improving Sirens. See `todo.md`.
 
## Copyright / Acknowledgements
Copyright 2009-2012 Arizona State University. Sirens is licensed under the [GNU LGPL](http://creativecommons.org/licenses/LGPL/2.1/). See COPYING.LESSER for details. If you would like to use Sirens for a commercial application or have other difficulties with its licensing restrictions, please contact [Brandon Hawley on Github](http://www.github.com/plant/).

Sirens is based off research within the [Arts, Media and Engineering](http://ame.asu.edu/) program at [Arizona State University](http://asu.edu/). Additionally, work on Sirens is supported by the [National Science Foundation](http://www.nsf.gov/) under Grants NSF IGERT DGE-05-04647 and NSF CISE Research Infrastructure 04-03428. For more information, the following paper may be of interest: 

G. Wichern, J. Xue, H. Thornburg, B. Mechtley, and A. Spanias, Segmentation, indexing, and retrieval for environmental and natural sounds, IEEE Transactions on Audio, Speech, and Language Processing, vol. 18, no. 3, pp. 688-707, 2010.

