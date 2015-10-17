#!/usr/bin/env python

"""
Python script parses the source files and automatically generates a Makefile
appropriate for compiling on Linux.
"""

compiler = 'g++'
cflags = '-g'
lflags = ''
objpath = 'obj'
srcpath = 'src'
executable = 'plotHistos'

#Picoscope-specific includes
boost = ''
boost_includes = ''
#includes = '-I/Applications/PicoScope6.app/Contents/Resources/include/'
incROOT = " `root-config --cflags`"
includes = ''#'-I/opt/picoscope/include -I/opt/picoscope/include/libps6000-1.4 -I/opt/local/include/ -I/opt/local/include/gsl/'
includes += incROOT
#libs = '-L/Applications/PicoScope6.app/Contents/Resources/lib/libps6000-1.4 -L/Applications/PicoScope6.app/Contents/Resources/lib -lpicoipp -liomp5 -lps6000'
libROOT = " `root-config --ldflags --glibs`"
libs = ''#'-L/opt/picoscope/lib/ -L/Applications/PicoScope6.app/Contents/Resources/lib/ -L/opt/local/lib -lgsl -lpicoipp -liomp5 -lps6000'
libs += libROOT

ext_source = '.cpp'
ext_header = '.h'
sources = []
objects = []

makefile = None

import os

def find_sources():
    global sources
    global objects

    path = '.'
    if len(srcpath) > 0:
        path = srcpath + '/'

    for root, dir, files in os.walk(path):
        for file in files:
            if len(file) < len(ext_source):
                continue
            if file[-len(ext_source):] != ext_source:
                continue

            source = os.path.join(root[len(path):], file)
            object = file[:-len(ext_source)] + '.o'

            objects.append(object)
            sources.append(source)


def write_preamble():
    makefile.write('CC = ' + compiler + '\n')
    makefile.write('CFLAGS = ' + cflags + '\n')
    makefile.write('LFLAGS = ' + lflags + '\n')
    makefile.write('OBJPATH = ' + objpath + '\n')
    makefile.write('SRCPATH = ' + srcpath + '\n')
    makefile.write('EXECUTABLE = ' + executable + '\n')
    #Picoscope-specific
    makefile.write('BOOST = ' + boost + '\n')
    makefile.write('BOOST_INCLUDES = ' + boost_includes + '\n')
    makefile.write('INCLUDES = ' + includes + '\n')
    makefile.write('LIBS = ' + libs + '\n')



    makefile.write('OBJECTS = ')
    for object in objects:
        makefile.write('$(OBJPATH)/' + object + ' ')
    makefile.write('\n\n')


def write_targets():
    makefile.write('all: ' + executable + '\n\n')
    makefile.write(
            executable + ': $(OBJECTS)\n'
            '\t$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) -o $(EXECUTABLE)\n\n')

    for (object, source) in zip(objects, sources):
        makefile.write(
                '$(OBJPATH)/' + object + ': $(SRCPATH)/' + source + '\n'
                '\t$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/' + source +
                ' -o $(OBJPATH)/' + object + '\n\n')

    makefile.write('clean:\n\t rm $(OBJPATH)/*.o '+executable)


if __name__ == "__main__":
    makefile = open('Makefile', 'w')

    find_sources()
    write_preamble()
    write_targets()

    makefile.close()
