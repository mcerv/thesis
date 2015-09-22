CC = g++
CFLAGS = -g
LFLAGS = 
OBJPATH = obj
SRCPATH = src
EXECUTABLE0 = colours
EXECUTABLE1 = decay
EXECUTABLE2 = decaycompare
EXECUTABLE3 = decaysolver
EXECUTABLE4 = funs
EXECUTABLE5 = grab_hystc2
EXECUTABLE6 = grab_longtermc2
EXECUTABLE7 = grab_longtermc2_lpf
EXECUTABLE8 = grab_setoffc2
EXECUTABLE9 = grab_voltincreasec2
EXECUTABLE10 = grabpulses
EXECUTABLE11 = grabvalues
EXECUTABLE12 = hyst_hyst
EXECUTABLE13 = hyst_longterm
EXECUTABLE14 = hysteresisLongterm
EXECUTABLE15 = landaus
EXECUTABLE16 = plotgraphs
EXECUTABLE17 = scLifetimeElecs
EXECUTABLE18 = scLifetimeHoles
EXECUTABLE19 = templatedecaysolver
EXECUTABLE20 = tempVar
EXECUTABLE21 = wanaresultsincl
BOOST = 
BOOST_INCLUDES = 
INCLUDES =  `root-config --cflags`
LIBS =  `root-config --ldflags --glibs`
OBJECTS = $(OBJPATH)/drawfuns.o $(OBJPATH)/waveform.o $(OBJPATH)/waveformana.o $(OBJPATH)/cluster.o $(OBJPATH)/event.o $(OBJPATH)/hit.o $(OBJPATH)/plane.o $(OBJPATH)/storageio.o $(OBJPATH)/track.o 
OBJMAIN0 = $(OBJPATH)/colours.o 
OBJMAIN1 = $(OBJPATH)/decay.o 
OBJMAIN2 = $(OBJPATH)/decaycompare.o 
OBJMAIN3 = $(OBJPATH)/decaysolver.o 
OBJMAIN4 = $(OBJPATH)/funs.o 
OBJMAIN5 = $(OBJPATH)/grab_hystc2.o 
OBJMAIN6 = $(OBJPATH)/grab_longtermc2.o 
OBJMAIN7 = $(OBJPATH)/grab_longtermc2_lpf.o 
OBJMAIN8 = $(OBJPATH)/grab_setoffc2.o 
OBJMAIN9 = $(OBJPATH)/grab_voltincreasec2.o 
OBJMAIN10 = $(OBJPATH)/grabpulses.o 
OBJMAIN11 = $(OBJPATH)/grabvalues.o 
OBJMAIN12 = $(OBJPATH)/hyst_hyst.o 
OBJMAIN13 = $(OBJPATH)/hyst_longterm.o 
OBJMAIN14 = $(OBJPATH)/hysteresisLongterm.o 
OBJMAIN15 = $(OBJPATH)/landaus.o 
OBJMAIN16 = $(OBJPATH)/plotgraphs.o 
OBJMAIN17 = $(OBJPATH)/scLifetimeElecs.o 
OBJMAIN18 = $(OBJPATH)/scLifetimeHoles.o 
OBJMAIN19 = $(OBJPATH)/templatedecaysolver.o 
OBJMAIN20 = $(OBJPATH)/tempVar.o 
OBJMAIN21 = $(OBJPATH)/wanaresultsincl.o 

all: exec

exec: $(OBJECTS) $(OBJMAIN0) $(OBJMAIN1) $(OBJMAIN2) $(OBJMAIN3) $(OBJMAIN4) $(OBJMAIN5) $(OBJMAIN6) $(OBJMAIN7) $(OBJMAIN8) $(OBJMAIN9) $(OBJMAIN10) $(OBJMAIN11) $(OBJMAIN12) $(OBJMAIN13) $(OBJMAIN14) $(OBJMAIN15) $(OBJMAIN16) $(OBJMAIN17) $(OBJMAIN18) $(OBJMAIN19) $(OBJMAIN20) $(OBJMAIN21) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN0) -o $(EXECUTABLE0) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN1) -o $(EXECUTABLE1) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN2) -o $(EXECUTABLE2) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN3) -o $(EXECUTABLE3) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN4) -o $(EXECUTABLE4) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN5) -o $(EXECUTABLE5) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN6) -o $(EXECUTABLE6) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN7) -o $(EXECUTABLE7) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN8) -o $(EXECUTABLE8) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN9) -o $(EXECUTABLE9) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN10) -o $(EXECUTABLE10) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN11) -o $(EXECUTABLE11) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN12) -o $(EXECUTABLE12) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN13) -o $(EXECUTABLE13) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN14) -o $(EXECUTABLE14) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN15) -o $(EXECUTABLE15) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN16) -o $(EXECUTABLE16) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN17) -o $(EXECUTABLE17) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN18) -o $(EXECUTABLE18) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN19) -o $(EXECUTABLE19) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN20) -o $(EXECUTABLE20) 
	$(CC) $(LFLAGS) $(LIBS) $(OBJECTS) $(OBJMAIN21) -o $(EXECUTABLE21) 


$(OBJPATH)/drawfuns.o: $(SRCPATH)/drawfuns.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/drawfuns.cpp -o $(OBJPATH)/drawfuns.o

$(OBJPATH)/waveform.o: $(SRCPATH)/waveform.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/waveform.cpp -o $(OBJPATH)/waveform.o

$(OBJPATH)/waveformana.o: $(SRCPATH)/waveformana.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/waveformana.cpp -o $(OBJPATH)/waveformana.o

$(OBJPATH)/cluster.o: $(SRCPATH)/converters/storage/cluster.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/storage/cluster.cpp -o $(OBJPATH)/cluster.o

$(OBJPATH)/event.o: $(SRCPATH)/converters/storage/event.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/storage/event.cpp -o $(OBJPATH)/event.o

$(OBJPATH)/hit.o: $(SRCPATH)/converters/storage/hit.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/storage/hit.cpp -o $(OBJPATH)/hit.o

$(OBJPATH)/plane.o: $(SRCPATH)/converters/storage/plane.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/storage/plane.cpp -o $(OBJPATH)/plane.o

$(OBJPATH)/storageio.o: $(SRCPATH)/converters/storage/storageio.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/storage/storageio.cpp -o $(OBJPATH)/storageio.o

$(OBJPATH)/track.o: $(SRCPATH)/converters/storage/track.cpp
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/converters/storage/track.cpp -o $(OBJPATH)/track.o

$(OBJPATH)/colours.o: $(SRCPATH)/colours.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/colours.cxx -o $(OBJPATH)/colours.o

$(OBJPATH)/decay.o: $(SRCPATH)/decay.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/decay.cxx -o $(OBJPATH)/decay.o

$(OBJPATH)/decaycompare.o: $(SRCPATH)/decaycompare.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/decaycompare.cxx -o $(OBJPATH)/decaycompare.o

$(OBJPATH)/decaysolver.o: $(SRCPATH)/decaysolver.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/decaysolver.cxx -o $(OBJPATH)/decaysolver.o

$(OBJPATH)/funs.o: $(SRCPATH)/funs.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/funs.cxx -o $(OBJPATH)/funs.o

$(OBJPATH)/grab_hystc2.o: $(SRCPATH)/grab_hystc2.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/grab_hystc2.cxx -o $(OBJPATH)/grab_hystc2.o

$(OBJPATH)/grab_longtermc2.o: $(SRCPATH)/grab_longtermc2.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/grab_longtermc2.cxx -o $(OBJPATH)/grab_longtermc2.o

$(OBJPATH)/grab_longtermc2_lpf.o: $(SRCPATH)/grab_longtermc2_lpf.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/grab_longtermc2_lpf.cxx -o $(OBJPATH)/grab_longtermc2_lpf.o

$(OBJPATH)/grab_setoffc2.o: $(SRCPATH)/grab_setoffc2.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/grab_setoffc2.cxx -o $(OBJPATH)/grab_setoffc2.o

$(OBJPATH)/grab_voltincreasec2.o: $(SRCPATH)/grab_voltincreasec2.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/grab_voltincreasec2.cxx -o $(OBJPATH)/grab_voltincreasec2.o

$(OBJPATH)/grabpulses.o: $(SRCPATH)/grabpulses.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/grabpulses.cxx -o $(OBJPATH)/grabpulses.o

$(OBJPATH)/grabvalues.o: $(SRCPATH)/grabvalues.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/grabvalues.cxx -o $(OBJPATH)/grabvalues.o

$(OBJPATH)/hyst_hyst.o: $(SRCPATH)/hyst_hyst.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/hyst_hyst.cxx -o $(OBJPATH)/hyst_hyst.o

$(OBJPATH)/hyst_longterm.o: $(SRCPATH)/hyst_longterm.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/hyst_longterm.cxx -o $(OBJPATH)/hyst_longterm.o

$(OBJPATH)/hysteresisLongterm.o: $(SRCPATH)/hysteresisLongterm.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/hysteresisLongterm.cxx -o $(OBJPATH)/hysteresisLongterm.o

$(OBJPATH)/landaus.o: $(SRCPATH)/landaus.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/landaus.cxx -o $(OBJPATH)/landaus.o

$(OBJPATH)/plotgraphs.o: $(SRCPATH)/plotgraphs.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/plotgraphs.cxx -o $(OBJPATH)/plotgraphs.o

$(OBJPATH)/scLifetimeElecs.o: $(SRCPATH)/scLifetimeElecs.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/scLifetimeElecs.cxx -o $(OBJPATH)/scLifetimeElecs.o

$(OBJPATH)/scLifetimeHoles.o: $(SRCPATH)/scLifetimeHoles.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/scLifetimeHoles.cxx -o $(OBJPATH)/scLifetimeHoles.o

$(OBJPATH)/templatedecaysolver.o: $(SRCPATH)/templatedecaysolver.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/templatedecaysolver.cxx -o $(OBJPATH)/templatedecaysolver.o

$(OBJPATH)/tempVar.o: $(SRCPATH)/tempVar.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/tempVar.cxx -o $(OBJPATH)/tempVar.o

$(OBJPATH)/wanaresultsincl.o: $(SRCPATH)/wanaresultsincl.cxx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $(SRCPATH)/wanaresultsincl.cxx -o $(OBJPATH)/wanaresultsincl.o

clean:
	 rm $(OBJPATH)/*.o colours decay decaycompare decaysolver funs grab_hystc2 grab_longtermc2 grab_longtermc2_lpf grab_setoffc2 grab_voltincreasec2 grabpulses grabvalues hyst_hyst hyst_longterm hysteresisLongterm landaus plotgraphs scLifetimeElecs scLifetimeHoles templatedecaysolver tempVar wanaresultsincl 
