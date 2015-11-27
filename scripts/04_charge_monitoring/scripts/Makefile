CC = g++
CFLAGS = -g
LFLAGS = 
OBJPATH = obj
SRCPATH = src
EXECUTABLE0 = plotDB
EXECUTABLE1 = plotDetArea
EXECUTABLE2 = plotProduction
EXECUTABLE3 = plotSourceScan
BOOST = 
BOOST_INCLUDES = 
INCLUDES =  `root-config --cflags`
LIBS =  `root-config --ldflags --glibs`
OBJECTS = $(OBJPATH)/drawfuns.o 
OBJMAIN0 = $(OBJPATH)/plotDB.o 
OBJMAIN1 = $(OBJPATH)/plotDetArea.o 
OBJMAIN2 = $(OBJPATH)/plotProduction.o 
OBJMAIN3 = $(OBJPATH)/plotSourceScan.o 

all: exec

exec: $(OBJECTS) $(OBJMAIN0) $(OBJMAIN1) $(OBJMAIN2) $(OBJMAIN3) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN0) -o $(EXECUTABLE0) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN1) -o $(EXECUTABLE1) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN2) -o $(EXECUTABLE2) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN3) -o $(EXECUTABLE3) 


$(OBJPATH)/drawfuns.o: $(SRCPATH)/drawfuns.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/drawfuns.cpp -o $(OBJPATH)/drawfuns.o

$(OBJPATH)/plotDB.o: $(SRCPATH)/plotDB.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotDB.cxx -o $(OBJPATH)/plotDB.o

$(OBJPATH)/plotDetArea.o: $(SRCPATH)/plotDetArea.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotDetArea.cxx -o $(OBJPATH)/plotDetArea.o

$(OBJPATH)/plotProduction.o: $(SRCPATH)/plotProduction.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotProduction.cxx -o $(OBJPATH)/plotProduction.o

$(OBJPATH)/plotSourceScan.o: $(SRCPATH)/plotSourceScan.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotSourceScan.cxx -o $(OBJPATH)/plotSourceScan.o

clean:
	 rm $(OBJPATH)/*.o plotDB plotDetArea plotProduction plotSourceScan 
