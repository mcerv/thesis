CC = g++
CFLAGS = -g
LFLAGS = 
OBJPATH = obj
SRCPATH = src
EXECUTABLE0 = plotDetArea
BOOST = 
BOOST_INCLUDES = 
INCLUDES =  `root-config --cflags`
LIBS =  `root-config --ldflags --glibs`
OBJECTS = $(OBJPATH)/drawfuns.o 
OBJMAIN0 = $(OBJPATH)/plotDetArea.o 

all: exec

exec: $(OBJECTS) $(OBJMAIN0) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN0) -o $(EXECUTABLE0) 


$(OBJPATH)/drawfuns.o: $(SRCPATH)/drawfuns.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/drawfuns.cpp -o $(OBJPATH)/drawfuns.o

$(OBJPATH)/plotDetArea.o: $(SRCPATH)/plotDetArea.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotDetArea.cxx -o $(OBJPATH)/plotDetArea.o

clean:
	 rm $(OBJPATH)/*.o plotDetArea 
