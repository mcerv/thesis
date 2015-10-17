CC = g++
CFLAGS = -g
LFLAGS = 
OBJPATH = obj
SRCPATH = src
EXECUTABLE = plotHistos
BOOST = 
BOOST_INCLUDES = 
INCLUDES =  `root-config --cflags`
LIBS =  `root-config --ldflags --glibs`
OBJECTS = $(OBJPATH)/drawfuns.o $(OBJPATH)/histana.o $(OBJPATH)/main.o $(OBJPATH)/plots.o $(OBJPATH)/plotter.o $(OBJPATH)/progbar.o $(OBJPATH)/pulsefilereader.o 

all: plotHistos

plotHistos: $(OBJECTS)
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) -o $(EXECUTABLE)

$(OBJPATH)/drawfuns.o: $(SRCPATH)/drawfuns.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/drawfuns.cpp -o $(OBJPATH)/drawfuns.o

$(OBJPATH)/histana.o: $(SRCPATH)/histana.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/histana.cpp -o $(OBJPATH)/histana.o

$(OBJPATH)/main.o: $(SRCPATH)/main.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/main.cpp -o $(OBJPATH)/main.o

$(OBJPATH)/plots.o: $(SRCPATH)/plots.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plots.cpp -o $(OBJPATH)/plots.o

$(OBJPATH)/plotter.o: $(SRCPATH)/plotter.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotter.cpp -o $(OBJPATH)/plotter.o

$(OBJPATH)/progbar.o: $(SRCPATH)/progbar.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/progbar.cpp -o $(OBJPATH)/progbar.o

$(OBJPATH)/pulsefilereader.o: $(SRCPATH)/pulsefilereader.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/pulsefilereader.cpp -o $(OBJPATH)/pulsefilereader.o

clean:
	 rm $(OBJPATH)/*.o plotHistos