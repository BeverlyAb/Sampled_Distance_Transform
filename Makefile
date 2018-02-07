INCDIR = -I.
DBG    = -g
OPT    = -O3
CPP    = g++
STD    = -std=gnu++11
CFLAGS = $(DBG) $(OPT) $(INCDIR) $(STD)
LINK   = -lm 

.cpp.o:
	$(CPP) $(CFLAGS) -c $< -o $@

all: dt

dt: dt.cpp 
	$(CPP) $(CFLAGS) -o dt dt.cpp $(LINK)

clean:
	/bin/rm -f dt *.o

clean-all: clean
	/bin/rm -f *~ 



