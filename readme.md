
## Quick note
This is mostly here for archival purposes / reproducibility. Much better bulk feature extraction tools exist now, such as [Sonic Annotator](http://www.vamp-plugins.org/sonic-annotator/) and [ESSENTIA](http://essentia.upf.edu). The one ever-so-slight benefit this implementation may have had over using Sonic Annotator was multithreaded feature extraction (sharing spectra etc.), but this is supported by ESSENTIA. I may, in the future, extract the segmentation bits to work on arbitrary CSV / annotation data from one of the above. - Brandon

## Sirenns
Sirens is a library for segmentation, indexing, and retrieval of environmental and natural sounds. In other words, it allows you to break long field recordings into individual sonic "events" and then compare those events in terms of content-based similarity.

In the `vamp/` folder, you can also find implementations of the individual temporal/spectral features used in Sirens as [Vamp](http://vamp-plugins.org) plugins.

## OSX Installation
To install Sirens on OSX, run `install.sh` in the `xcode` folder. There's a readme in there that should explain more.

## Generic Installation
Sirens relies on [FFTW](http://www.fftw.org) (for spectral analysis) and [SCons](http://www.scons.org) (for installation). To install, perform the following command in the project's directory:

	sudo scons install

To build the applications in the examples/ directory, do

	scons

### Parameters
If you do not have root access, you may have troubles installing Sirens. In this case, you can put the library in a custom directory. For example, to put the library in ~/lib and the includes in ~/include/sirens:

	scons install --prefix=$HOME

Additionally, if you need to point the Sirens install to a specific directory to find FFTW headers, you can specify any additional parameters to the compiler using `$CFLAGS` and `$LDFLAGS`. For example, to use `~/include/fftw3.h`, `~/lib/fftw3.a`, and so on:

	CFLAGS=-I$HOME/include LDFLAGS=-L$HOME/lib scons install --prefix=$HOME

## Using Sirens
Any application using Sirens also needs to link against [FFTW](http://www.fftw.org) and pthread:

	g++ myproject.cpp -lsirens -lfftw3 -lpthread

A tutorial will come eventually. See the `examples/` directory for some ideas.

## Upcoming changes
There are a number of tasks planned for improving Sirens. See `todo.md`.

## Copyright / Acknowledgements
Copyright 2009-2012 Arizona State University. Sirens is licensed under the [GNU LGPL](http://creativecommons.org/licenses/LGPL/2.1/). See COPYING.LESSER for details. If you would like to use Sirens for a commercial application or have other difficulties with its licensing restrictions, please contact [Brandon Mechtley on Github](http://www.github.com/plant/).

Sirens is based off research within the [Arts, Media and Engineering](http://ame.asu.edu/) program at [Arizona State University](http://asu.edu/). Additionally, work on Sirens is supported by the [National Science Foundation](http://www.nsf.gov/) under Grants NSF IGERT DGE-05-04647 and NSF CISE Research Infrastructure 04-03428. For more information, the following paper may be of interest:

G. Wichern, J. Xue, H. Thornburg, B. Mechtley, and A. Spanias, Segmentation, indexing, and retrieval for environmental and natural sounds, _IEEE Transactions on Audio, Speech, and Language Processing_, vol. 18, no. 3, pp. 688-707, 2010.

