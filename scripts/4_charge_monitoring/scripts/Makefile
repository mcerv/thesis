CC = g++
CFLAGS = -g
LFLAGS = 
OBJPATH = obj
SRCPATH = src
EXECUTABLE0 = plotDetArea
EXECUTABLE1 = plotProduction
BOOST = 
BOOST_INCLUDES = 
INCLUDES =  `root-config --cflags`
LIBS =  `root-config --ldflags --glibs`
OBJECTS = $(OBJPATH)/drawfuns.o 
OBJMAIN0 = $(OBJPATH)/plotDetArea.o 
OBJMAIN1 = $(OBJPATH)/plotProduction.o 

all: exec

exec: $(OBJECTS) $(OBJMAIN0) $(OBJMAIN1) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN0) -o $(EXECUTABLE0) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN1) -o $(EXECUTABLE1) 


$(OBJPATH)/drawfuns.o: $(SRCPATH)/drawfuns.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/drawfuns.cpp -o $(OBJPATH)/drawfuns.o

$(OBJPATH)/plotDetArea.o: $(SRCPATH)/plotDetArea.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotDetArea.cxx -o $(OBJPATH)/plotDetArea.o

$(OBJPATH)/plotProduction.o: $(SRCPATH)/plotProduction.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotProduction.cxx -o $(OBJPATH)/plotProduction.o

clean:
	 rm $(OBJPATH)/*.o plotDetArea plotProduction 
