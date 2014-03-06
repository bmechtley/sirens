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
    '': [os.path.join('source/', s) for s in [
        'CircularArray.h',
        'FeatureSet.h',
        'Feature.h',
        'Sound.h',
        'Thread.h',
        'Sirens.h',
        'FFT.h',
        'matrix_support.h',
        'string_support.h',
        'math_support.h',
        'Stk.h',
        'FileRead.h',
        'SpectralCentroid.h',
        'SpectralSparsity.h',
        'TemporalSparsity.h',
        'TransientIndex.h',
        'Harmonicity.h',
        'Loudness.h',
        'FeatureComparator.h',
        'SoundComparator.h',
        'SimpleFeatureComparator.h',
        'SimpleSoundComparator.h',
        'SegmentationParameters.h',
        'Segmenter.h'
    ]]
}

# Source.
compile_source = Glob(os.path.join(source_prefix, '*.cpp'))

# Environment.
environment = Environment(CC='gcc')
environment.Append(PREFIX=GetOption('prefix'))

# STK defines.
environment.Append(CCFLAGS="-D__MACOSX_CORE__")

if sys.byteorder == 'little':
    environment.Append(CCFLAGS="-D__LITTLE_ENDIAN__")

# Compile with debug symbols.
if GetOption('debug_symbols'):
    environment.Append(CCFLAGS='-g')

library = environment.Library('sirens', compile_source)

# Examples.
for example in [
    'segment',
    'segment_csv',
    'similarity',
    'similarity_simple',
    'similarity_first_csv',
    'features'
]:
    environment.Program(
        'examples/' + example + '.cpp',
        LIBS=['sirens', 'fftw3', 'pthread'],
        LIBPATH='.'
    )

# Install library.
environment.Install('$PREFIX/lib', library)
for header_path in install_headers:
    environment.Install(
        '$PREFIX/include/sirens' + header_path,
        install_headers[header_path]
    )

environment.Alias('install', [
    '$PREFIX/lib',
    '$PREFIX/include/sirens'
])

