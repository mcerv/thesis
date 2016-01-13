CC = g++
CFLAGS = -g
LFLAGS = 
OBJPATH = obj
SRCPATH = src
EXECUTABLE0 = plot4alpha
BOOST = 
BOOST_INCLUDES = 
INCLUDES =  `root-config --cflags`
LIBS =  `root-config --ldflags --glibs`
OBJECTS = $(OBJPATH)/drawfuns.o $(OBJPATH)/histana.o $(OBJPATH)/plots.o 
OBJMAIN0 = $(OBJPATH)/plot4alpha.o 

all: exec

exec: $(OBJECTS) $(OBJMAIN0) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN0) -o $(EXECUTABLE0) 


$(OBJPATH)/drawfuns.o: $(SRCPATH)/drawfuns.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/drawfuns.cpp -o $(OBJPATH)/drawfuns.o

$(OBJPATH)/histana.o: $(SRCPATH)/histana.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/histana.cpp -o $(OBJPATH)/histana.o

$(OBJPATH)/plots.o: $(SRCPATH)/plots.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plots.cpp -o $(OBJPATH)/plots.o

$(OBJPATH)/plot4alpha.o: $(SRCPATH)/plot4alpha.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plot4alpha.cxx -o $(OBJPATH)/plot4alpha.o

clean:
	 rm $(OBJPATH)/*.o plot4alpha 
