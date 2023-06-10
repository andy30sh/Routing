#ifndef DISTANCEVECTOR_H
#define DISTANCEVECTOR_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <tuple>
#include <iomanip>
#include <sstream>

using namespace std;

const int COLUMN_WIDTH = 5;
const string LINK_DOWN = "INF";
const int INFINITIVE = 999;

struct Router {
    string name;
    map<string, Router*> neighbors;
    map<string, int> distances;
    map<string, tuple<string, int>> routes;    
};

static string console_input;
static map<string, Router*> nodes;
static int round = 0;

string getDistanceKey(string source, string destination, string interface);
string getRouteKey(string source, string destination);
void calculateDistance();
void calculateRoute();
void printDistance();
void printRoute();
bool endsWith(string str, string ending);
void show(bool dist = true, bool rout = true, bool neig = true); 

#endif