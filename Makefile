OBJS = SiteMatch.o MatchItem.o FASTAFileHandle.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

nimble: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o nimble

SiteMatch.o: 
	$(CC) $(CFLAGS) SiteMatch.cpp
MatchItem.o:
	$(CC) $(CFLAGS) MatchItem.cpp MatchItem.h
FASTAFileHandle.o:
	$(CC) $(CFLAGS) FASTAFileHandle.cpp FASTAFileHandle.h
