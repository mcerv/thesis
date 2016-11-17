CC = g++
CFLAGS = -g
LFLAGS = 
OBJPATH = obj
SRCPATH = src
EXECUTABLE0 = kurfuerst
EXECUTABLE1 = plotCceCharge
EXECUTABLE2 = plotCharge
BOOST = 
BOOST_INCLUDES = 
INCLUDES =  `root-config --cflags`
LIBS =  `root-config --ldflags --glibs`
OBJECTS = $(OBJPATH)/drawfuns.o 
OBJMAIN0 = $(OBJPATH)/kurfuerst.o 
OBJMAIN1 = $(OBJPATH)/plotCceCharge.o 
OBJMAIN2 = $(OBJPATH)/plotCharge.o 

all: exec

exec: $(OBJECTS) $(OBJMAIN0) $(OBJMAIN1) $(OBJMAIN2) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN0) -o $(EXECUTABLE0) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN1) -o $(EXECUTABLE1) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN2) -o $(EXECUTABLE2) 


$(OBJPATH)/drawfuns.o: $(SRCPATH)/drawfuns.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/drawfuns.cpp -o $(OBJPATH)/drawfuns.o

$(OBJPATH)/kurfuerst.o: $(SRCPATH)/kurfuerst.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/kurfuerst.cxx -o $(OBJPATH)/kurfuerst.o

$(OBJPATH)/plotCceCharge.o: $(SRCPATH)/plotCceCharge.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotCceCharge.cxx -o $(OBJPATH)/plotCceCharge.o

$(OBJPATH)/plotCharge.o: $(SRCPATH)/plotCharge.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotCharge.cxx -o $(OBJPATH)/plotCharge.o

clean:
	 rm $(OBJPATH)/*.o kurfuerst plotCceCharge plotCharge 
