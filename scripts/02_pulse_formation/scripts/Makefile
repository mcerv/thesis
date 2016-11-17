CC = g++
CFLAGS = -g
LFLAGS = 
OBJPATH = obj
SRCPATH = src
EXECUTABLE0 = bandwidth
EXECUTABLE1 = chargedecay
EXECUTABLE2 = Efwhm
EXECUTABLE3 = RCchargedecay
BOOST = 
BOOST_INCLUDES = 
INCLUDES =  `root-config --cflags`
LIBS =  `root-config --ldflags --glibs`
OBJECTS = $(OBJPATH)/drawfuns.o 
OBJMAIN0 = $(OBJPATH)/bandwidth.o 
OBJMAIN1 = $(OBJPATH)/chargedecay.o 
OBJMAIN2 = $(OBJPATH)/Efwhm.o 
OBJMAIN3 = $(OBJPATH)/RCchargedecay.o 

all: exec

exec: $(OBJECTS) $(OBJMAIN0) $(OBJMAIN1) $(OBJMAIN2) $(OBJMAIN3) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN0) -o $(EXECUTABLE0) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN1) -o $(EXECUTABLE1) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN2) -o $(EXECUTABLE2) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN3) -o $(EXECUTABLE3) 


$(OBJPATH)/drawfuns.o: $(SRCPATH)/drawfuns.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/drawfuns.cpp -o $(OBJPATH)/drawfuns.o

$(OBJPATH)/bandwidth.o: $(SRCPATH)/bandwidth.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/bandwidth.cxx -o $(OBJPATH)/bandwidth.o

$(OBJPATH)/chargedecay.o: $(SRCPATH)/chargedecay.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/chargedecay.cxx -o $(OBJPATH)/chargedecay.o

$(OBJPATH)/Efwhm.o: $(SRCPATH)/Efwhm.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/Efwhm.cxx -o $(OBJPATH)/Efwhm.o

$(OBJPATH)/RCchargedecay.o: $(SRCPATH)/RCchargedecay.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/RCchargedecay.cxx -o $(OBJPATH)/RCchargedecay.o

clean:
	 rm $(OBJPATH)/*.o bandwidth chargedecay Efwhm RCchargedecay 
