CC = g++
CFLAGS = -g
LFLAGS = 
OBJPATH = obj
SRCPATH = src
EXECUTABLE0 = plotFreq
EXECUTABLE1 = plotPercent
BOOST = 
BOOST_INCLUDES = 
INCLUDES =  `root-config --cflags`
LIBS =  `root-config --ldflags --glibs`
OBJECTS = $(OBJPATH)/drawfuns.o $(OBJPATH)/histana.o $(OBJPATH)/measset.o $(OBJPATH)/plots.o $(OBJPATH)/plotter.o $(OBJPATH)/progbar.o $(OBJPATH)/pulsefilereader.o 
OBJMAIN0 = $(OBJPATH)/plotFreq.o 
OBJMAIN1 = $(OBJPATH)/plotPercent.o 

all: exec

exec: $(OBJECTS) $(OBJMAIN0) $(OBJMAIN1) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN0) -o $(EXECUTABLE0) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN1) -o $(EXECUTABLE1) 


$(OBJPATH)/drawfuns.o: $(SRCPATH)/drawfuns.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/drawfuns.cpp -o $(OBJPATH)/drawfuns.o

$(OBJPATH)/histana.o: $(SRCPATH)/histana.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/histana.cpp -o $(OBJPATH)/histana.o

$(OBJPATH)/measset.o: $(SRCPATH)/measset.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/measset.cpp -o $(OBJPATH)/measset.o

$(OBJPATH)/plots.o: $(SRCPATH)/plots.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plots.cpp -o $(OBJPATH)/plots.o

$(OBJPATH)/plotter.o: $(SRCPATH)/plotter.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotter.cpp -o $(OBJPATH)/plotter.o

$(OBJPATH)/progbar.o: $(SRCPATH)/progbar.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/progbar.cpp -o $(OBJPATH)/progbar.o

$(OBJPATH)/pulsefilereader.o: $(SRCPATH)/pulsefilereader.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/pulsefilereader.cpp -o $(OBJPATH)/pulsefilereader.o

$(OBJPATH)/plotFreq.o: $(SRCPATH)/plotFreq.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotFreq.cxx -o $(OBJPATH)/plotFreq.o

$(OBJPATH)/plotPercent.o: $(SRCPATH)/plotPercent.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotPercent.cxx -o $(OBJPATH)/plotPercent.o

clean:
	 rm $(OBJPATH)/*.o plotFreq plotPercent 
