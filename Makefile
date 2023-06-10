CC = g++
CFLAGS = -Wall -std=c++11

all: DistanceVector SplitHorizon

DistanceVector: ./DistanceVector.cpp ./DistanceVector.h
	$(CC) $(CFLAGS) -o ./DistanceVector ./DistanceVector.cpp

SplitHorizon: ./SplitHorizon.cpp ./DistanceVector.h
	$(CC) $(CFLAGS) -o ./SplitHorizon ./SplitHorizon.cpp

clean:
	rm -f ./DistanceVector ./SplitHorizon