#include "DistanceVector.h"

/* Router functions */
void parseInput() {
    Router* router;
    map<string, string> routers;
    istringstream stream(console_input);    
    string line;
    string source, dest;
    int cost;
    int status = 0;
    bool isUpdate = false;

    while (getline(stream, line)) {
        if (line.empty()) {
            continue;
        } if (line == "END") {   
            if (isUpdate) {    
                calculateRoute();                           
                calculateDistance();
            }
            break;
        } else if (line == "DISTANCEVECTOR") {
            status = 1;
            for (const auto& source : nodes) {
                router = source.second;
                for (const auto& dest : nodes) {
                    if (source.first != dest.first) {
                        router->routes.emplace(getRouteKey(source.first, dest.first), make_tuple(dest.first, INFINITIVE));
                        for (const auto& interface : nodes) {
                            if (source.first != interface.first) {
                                router->distances.emplace(getDistanceKey(source.first, dest.first, interface.first), INFINITIVE);
                            }
                        }
                    }
                }
            }                   
        } else if (line == "UPDATE") {            
            status = 2;
            calculateDistance();
        } else {
            if (status == 0) {
                router = new Router();
                router->name = line;
                nodes.emplace(line, router);
            } else if (status == 1) {
                istringstream iss(line);
                if (iss >> source >> dest >> cost) {  
                    nodes[source]->distances[getDistanceKey(source, dest, dest)] = cost < 0 ? INFINITIVE : cost;
                    nodes[source]->routes[getRouteKey(source, dest)] = make_tuple(dest, cost < 0 ? INFINITIVE : cost);                    
                    nodes[source]->neighbors[dest] = nodes[dest];
                    nodes[dest]->distances[getDistanceKey(dest, source, source)] = cost < 0 ? INFINITIVE : cost;
                    nodes[dest]->routes[getRouteKey(dest, source)] = make_tuple(source, cost < 0 ? INFINITIVE : cost);
                    nodes[dest]->neighbors[source] = nodes[source];
                    if (cost > 0) {
                        nodes[source]->neighbors[dest] = nodes[dest];
                        nodes[dest]->neighbors[source] = nodes[source];
                    } else {
                        nodes[source]->neighbors.erase(dest);
                        nodes[dest]->neighbors.erase(source);
                    }
                }
            } else if (status == 2) {
                istringstream iss(line);
                if (iss >> source >> dest >> cost) {
                    int distance, distance_update;
                    if (cost > 0) {                        
                        router = nodes[source];
                        distance = router->distances[getDistanceKey(source, dest, dest)];                        
                        distance_update = cost - distance;
                        for (const auto& distance : nodes[source]->distances) {
                            if (endsWith(distance.first, dest + "]")) {
                                nodes[source]->distances[distance.first] += distance_update;                               
                            }
                        }
                        for (const auto& distance : nodes[dest]->distances) {
                            if (endsWith(distance.first, source + "]")) {
                                nodes[dest]->distances[distance.first] += distance_update;  
                            }
                        }
                        nodes[source]->neighbors[dest] = nodes[dest];
                        nodes[dest]->neighbors[source] = nodes[source];
                    } else {                        
                        for (const auto& distance : nodes[source]->distances) {
                            if (endsWith(distance.first, dest + "]")) {
                                nodes[source]->distances[distance.first] = INFINITIVE;
                            }
                        }
                        for (const auto& distance : nodes[dest]->distances) {
                            if (endsWith(distance.first, source + "]")) {
                                nodes[dest]->distances[distance.first] = INFINITIVE;
                            }
                        }
                        nodes[source]->neighbors.erase(dest);
                        nodes[dest]->neighbors.erase(source);
                    }
                }
                isUpdate = true;
            }
        }       
    }     
}

void calculateDistance() {
    Router* router;
    int distance, distance_to_neighbor, neighbor_to_dest;    
    bool isChanged = true;

    while(isChanged) {
        isChanged = false;              
        printDistance();
        for (const auto& source : nodes) {
            router = source.second;
            for (const auto& dest : nodes) {
                if (source.first != dest.first) {                    
                    for (const auto& interface : nodes) {
                        if (source.first != interface.first) {
                            distance = router->distances[getDistanceKey(source.first, dest.first, interface.first)];                            
                            for (const auto& neighbor : router->neighbors) {
                                if (neighbor.first == interface.first) {
                                    distance_to_neighbor = source.second->distances[getDistanceKey(source.first, neighbor.second->name, neighbor.second->name)];
                                    neighbor_to_dest = get<1>(neighbor.second->routes[getRouteKey(neighbor.second->name, dest.first)]);
                                    if (neighbor_to_dest < INFINITIVE) {                                        
                                        router->distances[getDistanceKey(source.first, dest.first, interface.first)] = distance_to_neighbor + neighbor_to_dest;
                                        if (distance > distance_to_neighbor + neighbor_to_dest) {
                                            isChanged = true;
                                        }
                                    } else {
                                        router->distances[getDistanceKey(source.first, dest.first, interface.first)] = INFINITIVE;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        calculateRoute();               
        round++;
    }    
    printRoute();
}

void calculateRoute() {
    Router* router;
    string passby;
    int distance, tmpDistance;

    for (const auto &source : nodes) {
        router = source.second;
        for (const auto &dest : nodes) {
            if (source.first != dest.first) {   
                passby = dest.first;
                distance = INFINITIVE;
                for (const auto &interface : nodes) {
                    if (source.first != interface.first) {                        
                        tmpDistance = router->distances[getDistanceKey(source.first, dest.first, interface.first)];
                        if (tmpDistance < distance) {
                            passby = interface.first;
                            distance = tmpDistance;
                        }
                    }
                }
                router->routes[getRouteKey(source.first, dest.first)] = make_tuple(passby, distance);
            }
        }
    }
}

/* Main function */
int main() {    
    string input;
    
    while (getline(cin, input)) { 
        console_input += input + '\n';
        if (input == "END") {
            break;
        }        
    }    
    parseInput();      
    return 0;
}

/* Common functions */
string getDistanceKey(string source, string destination, string interface) {
    return "[" + source + "|" + destination + "|" + interface + "]";
}

string getRouteKey(string source, string destination) {
    return "[" + source + "|" + destination + "]";
}

void printDistance() {
    int distance;

    for (const auto& source : nodes) {
        cout << source.second->name << " Distance Table at t=" << round << endl;        
        cout << left << setw(COLUMN_WIDTH) << "";
        for (const auto& dest : nodes) {            
            if (source.first != dest.first)                
                cout << left << setw(COLUMN_WIDTH) << dest.first;
        }
        cout << endl;
        for (const auto& dest : nodes) {
            if (source.first != dest.first) {
                cout << left << setw(COLUMN_WIDTH) << dest.first ;
                for (const auto& interface : nodes) {
                    if (source.first != interface.first) {
                        distance = source.second->distances[getDistanceKey(source.first, dest.first, interface.first)];
                        if (distance < INFINITIVE)
                            cout << left << setw(COLUMN_WIDTH) << distance;
                        else
                            cout << left << setw(COLUMN_WIDTH) << LINK_DOWN;
                    }
                }
                cout << endl;
            }
        }
        cout << endl;
    }
}

void printRoute() {
    string interface;
    int distance;

    for (const auto& source : nodes) {
        cout << source.second->name << " Routing Table:" << endl;        
        for (const auto& dest : nodes) {
            if (source.first != dest.first) {
                tuple<string, int> r = source.second->routes[getRouteKey(source.first, dest.first)];                
                interface =  get<0>(r);
                distance = get<1>(r);
                if (distance < INFINITIVE)
                    cout << dest.first << "," << interface << "," << distance << endl;
                else
                    cout << dest.first << "," << LINK_DOWN << "," << LINK_DOWN << endl;
            }
        }  
        cout << endl;      
    }
}

bool endsWith(string str, string ending) {
    if (str.length() >= ending.length()) {
        return str.compare(str.length() - ending.length(), ending.length(), ending) == 0;
    }
    return false;
}

void show(bool dist, bool rout, bool neig) {
    cout << "=== Router Information ===" << endl;    
    for (const auto& node : nodes) {
        cout << "Router ===[" << node.second->name << "]===" << endl;
        if (dist) {            
            cout << "1. Distances: (" << node.second->distances.size() << ")" << endl;
            for (const auto& distance : node.second->distances) {
                cout << "Key: " << distance.first << " Cost: " << distance.second << endl;
            }
        }
        if (rout) {
            cout << "2. Routes: (" << node.second->routes.size() << ")" << endl;
            for (const auto& route : node.second->routes) {
                if (get<1>(route.second) > 0) {
                    cout << "Key: " << route.first << " By: " << get<0>(route.second) << " Cost: " << get<1>(route.second) << endl;
                }
            }
        }
        if (neig) {
            cout << "3. Neighbors: (" << node.second->neighbors.size() << ") " << endl;
            for (const auto& neighbor : node.second->neighbors) {
                cout << neighbor.second->name << " ";
            }
        }
        cout << endl << endl;
    }
}