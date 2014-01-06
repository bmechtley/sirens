## Sirens Vamp features
Sirens is a collection of tools for segmentation, indexing, and retrieval of environmental sounds. This folder contains code to create [Vamp](http://vamp-plugins.org) plugins for its base set of features (in case you want to use them apart from Sirens for your own purposes). You can do analyses with these features using audio packages such as [Audacity](http://audacity.sf.net), [Sonic Visualizer](http://www.sonicvisualiser.org/), [Sonic Annotator](http://omras2.org/SonicAnnotator), etc.

Eventually, I'd like to implement Sirens itself to use either Vamp features or CSV input rather than the default set of hard-coded features so that the segmentation/comparison routines can be run on any time-series data.

## Installation
### OSX
Copy the pre-compiled `sirens.dylib` in your Vamp plugins path (You may need to create the Vamp directory if it does not exist.) e.g.:

    cp sirens.dylib ~/Library/Audio/Plug-Ins/Vamp/

### Other platforms / from source
1. Download and install the [Vamp plugin SDK](http://vamp-plugins.org/develop.html) and install it (see the [Developer Documentation](http://vamp-plugins.org/develop.html) for your platform.)
1. Open up `Makefile` for editing.
2. Make sure VAMP_SDK_DIR is pointing to the path where you installed the SDK by changing this line:

    `VAMP_SDK_DIR := /usr/local/share/vamp-plugin-sdk`

3. Comment out the following bits for OSX if that's not your platform and uncomment the bits relevant to your platform:

    `CXX := g++
    CXXFLAGS := -mmacosx-version-min=10.5 -arch i386 -arch x86_64 -I$(VAMP_SDK_DIR) -Wall -fPIC
    PLUGIN_EXT := .dylib
    LDFLAGS := $(CXXFLAGS) -dynamiclib -install_name $(PLUGIN_LIBRARY_NAME)$(PLUGIN_EXT) $(VAMP_SDK_DIR)/libvamp-sdk.a -exported_symbols_list vamp-plugin.list`

## Acknowledgements
Sirens-Vamp is based off research within the [Arts, Media and Engineering](http://ame.asu.edu/) program at [Arizona State University](http://asu.edu/). For more information, the following paper may be of interest:

G. Wichern, J. Xue, H. Thornburg, B. Mechtley, and A. Spanias, Segmentation, indexing, and retrieval for environmental and natural sounds, in _IEEE Transactions on Audio, Speech, and Language Processing_, vol. 18, no. 3, pp. 688-707, 2010.

Additionally, work on Sirens is supported by the [National Science Foundation](http://www.nsf.gov/) under Grants NSF IGERT DGE-05-04647 and NSF CISE Research Infrastructure 04-03428.

# Copyright
Copyright 2009 Arizona State University, released under the MIT License.
