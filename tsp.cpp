#include <fstream>
#include <iostream>
#include <cstdio>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cmath>
#include <unistd.h>
#include <stack>


using namespace std;
//g++ -std=c++11 tsp.cpp -o tsp
vector < vector <double> > distanceMatrix;

//print function for matrices
void print(vector<vector<double> > matrix){
    int rows = matrix.size();
    int cols = matrix[0].size();
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

//Read values and create distanceMatrix for the nodes.
vector<vector<double>> init(){
 //Retrieve all the nodes as a vector of tuples
    //Node 0 is the 0th node in the vector, node 1 is the 1st vectors etc.
    int points; //N - nodes
    cin >> points;
    vector < tuple <double, double> > v; // v: vector with all coordinate tuples [(x1,y1), (x2,y2), ... ]
    for(int i = 0; i < points; i++){
        double value1, value2;
        cin >> value1 >> value2;
        auto foo = make_tuple(value1,value2);
        v.push_back(foo);
    }

    //Build a NxN matrix consisting of all the routes/distances from each node to all the others (Euclidian distance)
    vector < vector <double> > distanceMatrix(points);
    for(int i = 0; i < points; i++){
        double x = get<0>(v[i]);
        double y = get<1>(v[i]);
        for(int j = 0; j < points; j++){
            double xOther = get<0>(v[j]);
            double yOther = get<1>(v[j]);
            double xDist = pow(abs(x-xOther),2);
            double yDist = pow(abs(y-yOther),2);
            double distance = sqrt(xDist + yDist);
            distanceMatrix[i].push_back(distance);
        }
    }
    return distanceMatrix;
}

//Return value from distanceMatrix
double getDist(int i, int j){
    if(i <0 || j < 0 || i > distanceMatrix.size() || j > distanceMatrix.size()){
        cerr << "dist function, nodes out of bounds";
    }
    return distanceMatrix[i][j];
}

//help function for 2-opt
double getChange(int i, int j, int iplus, int jplus){
    double change = getDist(i, j) + getDist(iplus, jplus) - getDist(i, iplus) - getDist(j, jplus);
    return change;
}


vector<int> twoOpt(vector<int> path){
    int nodes = distanceMatrix.size(); 
    int mini;
    int minj;
    double minChange;
    int killLoop = 0;

    do{
        //check if two edges are worth swapping
        minChange = 0;
        for (int i = 0; i < nodes-2; ++i){
            for (int j = i+2; j < nodes; ++j){
                int iplus = (i+1) % nodes;    //
                int jplus = (j+1) % nodes;    // close the circle, if j = length then j+1 should be index 0
                double change = getChange(path[i], path[j], path[iplus], path[jplus]);
                if (minChange > change){    
                    minChange = change;
                    mini = i; minj = j;
                }
            }
        }
        //apply move of mini minj, aka swap edges
        //i to j and j+1 to i+1
        stack <int> s;
        for (int i = 0; i < path.size(); ++i){
            if(i > mini && i <= minj){
                s.push(path[i]);
            }
        }
        for (int i = mini+1; i <= minj; ++i){
            path[i] = s.top();
            s.pop();
        }
        //set a limit on iterations
        ++killLoop;
    } while(minChange < 0 && killLoop < 100);

    return path;
}

vector<int> nearestNeighbor(int nodes){
    int current = 0;
    vector<int> returnPath;
    vector<bool> visited = vector<bool>(nodes);
    
    returnPath.push_back(current);
    visited[current] = true;

    do{
        double min = INT8_MAX; 
        int mini = 0;
        for (int i = 0; i < nodes; ++i){
            double dist = distanceMatrix[current][i];
            if(dist < min && (visited[i] != true)){
                min = dist;
                mini = i;
            }
        }
        current = mini;
        returnPath.push_back(current);
        visited[current] = true;    
    }while(returnPath.size() < nodes);

    return returnPath;
}

//initialize random paths
vector<int> randomPath(vector<int> path){
    random_shuffle(path.begin(), path.end());
    return path;
}



int main(){
    distanceMatrix = init();
    // print(distanceMatrix);
    int nodes = distanceMatrix.size(); 

    // if small graph, quickly output
    if(nodes <= 3){
        for (int i = 0; i < nodes; ++i){
            cout << i << "\n";
        }
        return 0;
    }

    // set a path to input order
    vector<int> initPath;
    for (int i = 0; i < nodes; ++i){
        initPath.push_back(i);
    }

    // vector<int> path = randomPath(initPath);
    // vector<int> path = nearestNeighbor(nodes);
    vector<int> path = initPath;

    // do 2-opt
    vector<int> result = twoOpt(path);

    // print out result
    for (int i = 0; i < result.size(); ++i){
        cout << result[i] << "\n";
    }

}
