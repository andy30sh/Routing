# **Routing Algorithm**
### 1. Distance Vector (DV)
### 2. Distance Vector with Split Horizon (SP) 


## **Part 1 (DV algorithm)**

Produce a program in C++ that:
Reads information about a topology/updates to the topology from the standard input.
Uses DV algorithm or DV with SH algorithm, as appropriate, to bring the simulated routers to convergence.
    a) Output the distance tables in the required format for each router at each step/round.
    b) Output the final routing tables in the required format once convergence is reached.
Repeats the above steps until no further input is provided.
The DV algorithm program named DistanceVector.

## **Part 2 (DV with SH algorithm)**

Modify a second version of the program that uses Split Horizon.
The DV with SH algorithm program named SplitHorizon.

## **Sample Input**
```
X
Y
Z
DISTANCEVECTOR
X Z 8
X Y 2
Y Z 3
UPDATE
Y Z -1
X Z 3
END 
```

### Topology
Initial configuration:

![Topology 1](https://github.com/andy30sh/Routing/blob/main/Topology1.png)

Updated configuration

![Topology 1](https://github.com/andy30sh/Routing/blob/main/Topology1.png)

### Input explanation
The input begins with the name of each router/node in the topology.

    Each name is on a new line
    Router names will be a letter from the alphabet only
    Router names are case-sensitive
    This section ends with the keyword "DISTANCEVECTOR"

The input continues with the details of each link/edge in the topology.

    Written as the names of two routers/nodes followed by the weight of that link/edge.
    Weight values should always be integers
    A weight value of -1 indicates a link/edge to remove from the topology if present.
    This section ends with the keyword "UPDATE".
    Your algorithm should run when the keyword "UPDATE" is inputted and bring your simulated routers' routing tables and distance tables to convergence. Then, show the Expected Output for each router.

The input continues with the update details of each link/edge in the topology given a link and cost. 

    The values in each line of input in this section should be used to update the current topology.
    In this section, links can be added or removed while routers can be added only.
    As an example, a weight value of -1 indicates a link/edge to remove from the topology if present.
    As an example, if an unseen new router/node name has been inputted in this section, your program should be able to add this new router to the topology.

    Y A 10

    From the example input given above, your program should add A as a new router into the topology where it has a link with a cost of 10 to Y. 
    A user may input 0 or more lines in the "UPDATE" section. This section ends with the keyword "END".
    If there is input in the "UPDATE" section, your implementation should run when the keyword "END" is inputted and bring your simulated routers' routing tables and distance tables to convergence. Then, show the Expected Output for each router. After that, the program exits normally.
    If there is no input in the "UPDATE" section, the program exits normally when the keyword "END" is inputted.

## **Sample Output**
```
X Distance Table at t=0
     Y    Z    
Y    2    INF  
Z    INF  8  

Y Distance Table at t=0
     X    Z    
X    2    INF  
Z    INF  3  
 
Z Distance Table at t=0
     X    Y    
X    8    INF  
Y    INF  3

...

X Distance Table at t=2
     Y    Z    
Y    2    11   
Z    5    8

Y Distance Table at t=2
     X    Z    
X    2    8    
Z    7    3 
 
Z Distance Table at t=2
     X    Y    
X    8    5    
Y    10   3
 
X Routing Table:
Y,Y,2
Z,Y,5

Y Routing Table:
X,X,2
Z,Z,3

Z Routing Table:
X,Y,5
Y,Y,3

X Distance Table at t=3
     Y    Z    
Y    2    6    
Z    5    3
 
Y Distance Table at t=3
     X    Z    
X    2    INF  
Z    7    INF
 
Z Distance Table at t=3
     X    Y    
X    3    INF  
Y    5    INF  

...

X Routing Table:
Y,Y,2
Z,Z,3

Y Routing Table:
X,X,2
Z,X,5

Z Routing Table:
X,X,3
Y,X,5
```