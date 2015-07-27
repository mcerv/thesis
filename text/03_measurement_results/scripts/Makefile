CC = g++
CFLAGS = -g
LFLAGS = 
OBJPATH = obj
SRCPATH = src
EXECUTABLE0 = decay
EXECUTABLE1 = decaycompare
EXECUTABLE2 = decaysolver
EXECUTABLE3 = landaus
EXECUTABLE4 = plotgraphs
EXECUTABLE5 = templatedecaysolver
EXECUTABLE6 = tempVar
EXECUTABLE7 = wanaresultsincl
BOOST = 
BOOST_INCLUDES = 
INCLUDES =  `root-config --cflags`
LIBS =  `root-config --ldflags --glibs`
OBJECTS = $(OBJPATH)/drawfuns.o $(OBJPATH)/waveform.o $(OBJPATH)/waveformana.o 
OBJMAIN0 = $(OBJPATH)/decay.o 
OBJMAIN1 = $(OBJPATH)/decaycompare.o 
OBJMAIN2 = $(OBJPATH)/decaysolver.o 
OBJMAIN3 = $(OBJPATH)/landaus.o 
OBJMAIN4 = $(OBJPATH)/plotgraphs.o 
OBJMAIN5 = $(OBJPATH)/templatedecaysolver.o 
OBJMAIN6 = $(OBJPATH)/tempVar.o 
OBJMAIN7 = $(OBJPATH)/wanaresultsincl.o 

all: exec

exec: $(OBJECTS) $(OBJMAIN0) $(OBJMAIN1) $(OBJMAIN2) $(OBJMAIN3) $(OBJMAIN4) $(OBJMAIN5) $(OBJMAIN6) $(OBJMAIN7) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN0) -o $(EXECUTABLE0) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN1) -o $(EXECUTABLE1) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN2) -o $(EXECUTABLE2) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN3) -o $(EXECUTABLE3) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN4) -o $(EXECUTABLE4) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN5) -o $(EXECUTABLE5) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN6) -o $(EXECUTABLE6) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN7) -o $(EXECUTABLE7) 


$(OBJPATH)/drawfuns.o: $(SRCPATH)/drawfuns.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/drawfuns.cpp -o $(OBJPATH)/drawfuns.o

$(OBJPATH)/waveform.o: $(SRCPATH)/waveform.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/waveform.cpp -o $(OBJPATH)/waveform.o

$(OBJPATH)/waveformana.o: $(SRCPATH)/waveformana.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/waveformana.cpp -o $(OBJPATH)/waveformana.o

$(OBJPATH)/decay.o: $(SRCPATH)/decay.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/decay.cxx -o $(OBJPATH)/decay.o

$(OBJPATH)/decaycompare.o: $(SRCPATH)/decaycompare.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/decaycompare.cxx -o $(OBJPATH)/decaycompare.o

$(OBJPATH)/decaysolver.o: $(SRCPATH)/decaysolver.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/decaysolver.cxx -o $(OBJPATH)/decaysolver.o

$(OBJPATH)/landaus.o: $(SRCPATH)/landaus.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/landaus.cxx -o $(OBJPATH)/landaus.o

$(OBJPATH)/plotgraphs.o: $(SRCPATH)/plotgraphs.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotgraphs.cxx -o $(OBJPATH)/plotgraphs.o

$(OBJPATH)/templatedecaysolver.o: $(SRCPATH)/templatedecaysolver.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/templatedecaysolver.cxx -o $(OBJPATH)/templatedecaysolver.o

$(OBJPATH)/tempVar.o: $(SRCPATH)/tempVar.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/tempVar.cxx -o $(OBJPATH)/tempVar.o

$(OBJPATH)/wanaresultsincl.o: $(SRCPATH)/wanaresultsincl.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/wanaresultsincl.cxx -o $(OBJPATH)/wanaresultsincl.o

clean:
	 rm $(OBJPATH)/*.o decay decaycompare decaysolver landaus plotgraphs templatedecaysolver tempVar wanaresultsincl 
