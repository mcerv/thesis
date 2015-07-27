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
OBJECTS = $(OBJPATH)/drawfuns.o $(OBJPATH)/Efwhm.o 

all: plotHistos

plotHistos: $(OBJECTS)
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) -o $(EXECUTABLE)

$(OBJPATH)/drawfuns.o: $(SRCPATH)/drawfuns.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/drawfuns.cpp -o $(OBJPATH)/drawfuns.o

$(OBJPATH)/Efwhm.o: $(SRCPATH)/Efwhm.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/Efwhm.cpp -o $(OBJPATH)/Efwhm.o

clean:
	 rm $(OBJPATH)/*.o plotHistos