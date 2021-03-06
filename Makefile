OBJS = SiteMatch.o MatchItem.o FASTAFileHandle.o GeneSite.o
CC = g++
DEBUG = -g
CFLAGS = -c -std=c++11 $(DEBUG)
LFLAGS = -Wall $(DEBUG) \
           -L/usr/lib/x86_64-linux-gnu/ \
	       -lboost_system \
	       -lboost_program_options \
	       -lboost_filesystem

all: nimble

nimble:$(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o nimble

SiteMatch.o:
	$(CC) $(CFLAGS) SiteMatch.cpp

MatchItem.o:
	$(CC) $(CFLAGS) MatchItem.cpp MatchItem.h

FASTAFileHandle.o:
	$(CC) $(CFLAGS) FASTAFileHandle.cpp FASTAFileHandle.h

GeneSite.o:
	$(CC) $(CFLAGS) GeneSite.cpp GeneSite.h

clean:
	rm -f *.o nimble *.gch


