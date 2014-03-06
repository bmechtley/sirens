# XCode project for OSX installation.

This project is set up to build sirens assuming that FFTW is installed in /usr/local/include. To use a different location, replace the reference in the project's file list with the one you want to use. I recommend installing FFTW with [homebrew](http://brew.sh/).

It's easiest to install this using `sh install.sh`. This will install the library to /usr/local/lib/sirens.dylib and headers to /usr/local/include/sirens.
