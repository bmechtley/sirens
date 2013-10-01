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
AddOption(
    '--prefix', 
    default='/usr/local', 
    dest='prefix', 
    type='string', 
    nargs=1, 
    action='store', 
    metavar='DIR', 
    help='installation prefix'
)

AddOption(
    '--shared', 
    action='store_true', 
    dest='shared', 
    help='use to build a shared library'
)

AddOption(
    '--debug-symbols', 
    action='store_true', 
    dest='debug_symbols', 
    help='build in debug symbols'
)

# Headers.
install_headers = {
    '/segmentation': [os.path.join('source/segmentation/', s) for s in [
        'SegmentationParameters.h', 
        'Segmenter.h'
    ]],
    '/retrieval': [os.path.join('source/retrieval/', s) for s in [
        'FeatureComparator.h', 
        'SoundComparator.h', 
        'SimpleFeatureComparator.h', 
        'SimpleSoundComparator.h'
    ]],
    '/features': [os.path.join('source/features/', s) for s in [
        'SpectralCentroid.h', 
        'SpectralSparsity.h', 
        'TemporalSparsity.h', 
        'TransientIndex.h', 
        'Harmonicity.h', 
        'Loudness.h'
    ]],
    '/stk': [os.path.join('source/stk/',  s) for s in [
        'Stk.h', 
        'FileRead.h'
    ]],
    '': [os.path.join('source/', s) for s in [
        'CircularArray.h', 
        'FeatureSet.h', 
        'Feature.h', 
        'Sound.h', 
        'Thread.h', 
        'Sirens.h'
    ]
}

# Source.
compile_source = Glob(os.path.join(source_prefix, '*.cpp'))
compile_source.extend(Glob(os.path.join(source_prefix, 'support/*.cpp')))
compile_source.extend(Glob(os.path.join(source_prefix, 'features/*.cpp')))
compile_source.extend(Glob(os.path.join(source_prefix, 'retrieval/*.cpp')))
compile_source.extend(Glob(os.path.join(source_prefix, 'segmentation/*.cpp')))
compile_source.extend(Glob(os.path.join(source_prefix, 'stk/*.cpp')))

# Environment.
environment = Environment(CC='gcc', CCFLAGS='-D__MACOSX_CORE__')
environment.Append(PREFIX = GetOption('prefix'))

if GetOption('debug_symbols'):
    environment.Append(CCFLAGS='-g')

library = environment.Library('sirens', compile_source)

# Examples.
for example in ['segmentcsv', 'segmentation', 'retrieval', 'simple_retrieval', 'features']:
    environment.Program('examples/' + example + '.cpp', LIBS=['sirens', 'fftw3', 'pthread'], LIBPATH='.')

# Install library.
environment.Install('$PREFIX/lib', library)
for header_path in install_headers:
    environment.Install('$PREFIX/include/sirens' + header_path, install_headers[header_path])

environment.Alias('install', ['$PREFIX/lib', '$PREFIX/include/sirens', '$PREFIX/include/sirens/features', '$PREFIX/include/sirens/retrieval', '$PREFIX/include/sirens/segmentation', '$PREFIX/include/sirens/stk'])
