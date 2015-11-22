CC = g++
CFLAGS = -g
LFLAGS = 
OBJPATH = obj
SRCPATH = src
EXECUTABLE0 = Efwhm
BOOST = 
BOOST_INCLUDES = 
INCLUDES =  `root-config --cflags`
LIBS =  `root-config --ldflags --glibs`
OBJECTS = $(OBJPATH)/drawfuns.o 
OBJMAIN0 = $(OBJPATH)/Efwhm.o 

all: exec

exec: $(OBJECTS) $(OBJMAIN0) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN0) -o $(EXECUTABLE0) 


$(OBJPATH)/drawfuns.o: $(SRCPATH)/drawfuns.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/drawfuns.cpp -o $(OBJPATH)/drawfuns.o

$(OBJPATH)/Efwhm.o: $(SRCPATH)/Efwhm.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/Efwhm.cxx -o $(OBJPATH)/Efwhm.o

clean:
	 rm $(OBJPATH)/*.o Efwhm 
