CC = g++
CFLAGS = -g
LFLAGS = 
OBJPATH = obj
SRCPATH = src
EXECUTABLE0 = plotAtlasData
EXECUTABLE1 = plotDB
EXECUTABLE2 = plotDetArea
EXECUTABLE3 = plotProduction
EXECUTABLE4 = plotSourceScan
BOOST = 
BOOST_INCLUDES = 
INCLUDES =  `root-config --cflags`
LIBS =  `root-config --ldflags --glibs`
OBJECTS = $(OBJPATH)/AtlasStyle.o $(OBJPATH)/AtlasUtils.o $(OBJPATH)/drawfuns.o 
OBJMAIN0 = $(OBJPATH)/plotAtlasData.o 
OBJMAIN1 = $(OBJPATH)/plotDB.o 
OBJMAIN2 = $(OBJPATH)/plotDetArea.o 
OBJMAIN3 = $(OBJPATH)/plotProduction.o 
OBJMAIN4 = $(OBJPATH)/plotSourceScan.o 

all: exec

exec: $(OBJECTS) $(OBJMAIN0) $(OBJMAIN1) $(OBJMAIN2) $(OBJMAIN3) $(OBJMAIN4) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN0) -o $(EXECUTABLE0) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN1) -o $(EXECUTABLE1) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN2) -o $(EXECUTABLE2) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN3) -o $(EXECUTABLE3) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN4) -o $(EXECUTABLE4) 


$(OBJPATH)/AtlasStyle.o: $(SRCPATH)/AtlasStyle.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/AtlasStyle.cpp -o $(OBJPATH)/AtlasStyle.o

$(OBJPATH)/AtlasUtils.o: $(SRCPATH)/AtlasUtils.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/AtlasUtils.cpp -o $(OBJPATH)/AtlasUtils.o

$(OBJPATH)/drawfuns.o: $(SRCPATH)/drawfuns.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/drawfuns.cpp -o $(OBJPATH)/drawfuns.o

$(OBJPATH)/plotAtlasData.o: $(SRCPATH)/plotAtlasData.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotAtlasData.cxx -o $(OBJPATH)/plotAtlasData.o

$(OBJPATH)/plotDB.o: $(SRCPATH)/plotDB.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotDB.cxx -o $(OBJPATH)/plotDB.o

$(OBJPATH)/plotDetArea.o: $(SRCPATH)/plotDetArea.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotDetArea.cxx -o $(OBJPATH)/plotDetArea.o

$(OBJPATH)/plotProduction.o: $(SRCPATH)/plotProduction.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotProduction.cxx -o $(OBJPATH)/plotProduction.o

$(OBJPATH)/plotSourceScan.o: $(SRCPATH)/plotSourceScan.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotSourceScan.cxx -o $(OBJPATH)/plotSourceScan.o

clean:
	 rm $(OBJPATH)/*.o plotAtlasData plotDB plotDetArea plotProduction plotSourceScan 
