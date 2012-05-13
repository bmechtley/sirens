# Copyright 2009 Arizona State University
# 
# This file is part of Sirens.
# 
# Sirens is free software: you can redistribute it and/or modify it under the terms 
# of the GNU Lesser General Public License as  published by the Free Software 
# Foundation, either version 3 of the License, or (at your option) any later version.
# 
# Sirens is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
# PURPOSE.  See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License along
# with Sirens. If not, see <http://www.gnu.org/licenses/>.


import os, sys, SCons.Util

source_prefix = 'source/'

# Append the source prefix to header files.
def append_source_prefix(header_list):
	return map(lambda x: os.path.join(source_prefix, x), header_list)

# Options.
AddOption('--prefix', default = '/usr/local', dest = 'prefix', type = 'string', nargs = 1, action = 'store', metavar = 'DIR', help = 'installation prefix')
AddOption('--shared', action = 'store_true', dest = 'shared', help = 'use to build a shared library')
AddOption('--debug-symbols', action = 'store_true', dest = 'debug_symbols', help = 'build in debug symbols')

# Headers.
install_segmentation_headers = append_source_prefix([
	'segmentation/SegmentationParameters.h',
	'segmentation/Segmenter.h'
])

install_retrieval_headers = append_source_prefix([
	'retrieval/FeatureComparator.h',
	'retrieval/SoundComparator.h',
	'retrieval/SimpleFeatureComparator.h',
	'retrieval/SimpleSoundComparator.h'
])

install_feature_headers = append_source_prefix([
	'features/SpectralCentroid.h', 
	'features/SpectralSparsity.h',
	'features/TemporalSparsity.h',
	'features/TransientIndex.h',
	'features/Harmonicity.h', 
	'features/Loudness.h',
])

install_headers = append_source_prefix([
	'CircularArray.h', 
	'FeatureSet.h',
	'Feature.h',
	'Sound.h',
	'Thread.h',
	'Sirens.h',
	'Exceptions.h'
])

# Source.
compile_source = Glob(os.path.join(source_prefix, '*.cpp'))
compile_source.extend(Glob(os.path.join(source_prefix, 'support/*.cpp')))
compile_source.extend(Glob(os.path.join(source_prefix, 'features/*.cpp')))
compile_source.extend(Glob(os.path.join(source_prefix, 'retrieval/*.cpp')))
compile_source.extend(Glob(os.path.join(source_prefix, 'segmentation/*.cpp')))

# Environment.
environment = Environment(CC = 'gcc')

if GetOption('debug_symbols'):
	environment['CCFLAGS'] += SCons.Util.CLVar('-g')

if os.environ.has_key('CC'):
	environment['CC'] = os.environ['CC']
if os.environ.has_key('CFLAGS'):
	environment['CCFLAGS'] += SCons.Util.CLVar(os.environ['CFLAGS'])
if os.environ.has_key('CXX'):
	environment['CXX'] = os.environ['CXX']
if os.environ.has_key('CXXFLAGS'):
	environment['CXXFLAGS'] += SCons.Util.CLVar(os.environ['CXXFLAGS'])
if os.environ.has_key('LDFLAGS'):
	environment['LINKFLAGS'] += SCons.Util.CLVar(os.environ['LDFLAGS'])

environment.Append(PREFIX = GetOption('prefix'))

library = environment.Library('sirens', compile_source) 

# Examples.
segmentation_example = environment.Program('examples/segmentation', 'examples/segmentation.cpp', LIBS = ['sirens', 'fftw3', 'sndfile'], LIBPATH = '.')
retrieval_example = environment.Program('examples/retrieval', 'examples/retrieval.cpp', LIBS = ['sirens', 'fftw3', 'sndfile'], LIBPATH = '.')
simple_retrieval_example = environment.Program('examples/simple_retrieval', 'examples/simple_retrieval.cpp', LIBS = ['sirens', 'fftw3', 'sndfile'], LIBPATH = '.')
features_example = environment.Program('examples/features', 'examples/features.cpp', LIBS = ['sirens', 'fftw3', 'sndfile'], LIBPATH = '.')

# Install library.
environment.Install('$PREFIX/lib', library)
environment.Install('$PREFIX/include/sirens', install_headers)
environment.Install('$PREFIX/include/sirens/features', install_feature_headers)
environment.Install('$PREFIX/include/sirens/retrieval', install_retrieval_headers)
environment.Install('$PREFIX/include/sirens/segmentation', install_segmentation_headers)

environment.Alias('install', ['$PREFIX/lib', '$PREFIX/include/sirens', '$PREFIX/include/sirens/features', '$PREFIX/include/sirens/retrieval', '$PREFIX/include/sirens/segmentation'])
