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
    int N; //N - nodes
    cin >> N;
    vector < tuple <double, double> > v; // v: vector with all coordinate tuples [(x1,y1), (x2,y2), ... ]
    for(int i = 0; i < N; i++){
        double value1, value2;
        cin >> value1 >> value2;
        auto foo = make_tuple(value1,value2);
        v.push_back(foo);
    }

    //Build a NxN matrix consisting of all the routes/distances from each node to all the others (Euclidian distance)
    vector < vector <double> > distanceMatrix(N);
    for(int i = 0; i < N; i++){
        double x = get<0>(v[i]);
        double y = get<1>(v[i]);
        for(int j = 0; j < N; j++){
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
    if(i < 0 || j < 0 || i > distanceMatrix.size() || j > distanceMatrix.size()){
        cerr << "dist function, nodes out of bounds";
    }
    return distanceMatrix[i][j];
}

//help function for 2-opt
double getChange(int i, int j, int iplus, int jplus){
    double change = getDist(i, j) + getDist(iplus, jplus) - getDist(i, iplus) - getDist(j, jplus);
    return change;
}

double lengthOfPath(vector<int> route){
    double sum;
    for(int i = 0; i < route.size()-1; i++){
        sum += distanceMatrix[route[i]][route[i+1]];
    }
    sum += distanceMatrix[route[0]][route[route.size()-1]];
    return sum;
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
        
        //cout << lengthOfPath(path);

    } while(minChange < 0 && killLoop < 220);
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


vector<int> greedyAlgorithm(){
    int nodes = distanceMatrix.size();
    vector<int> tour = vector<int>(nodes);
    vector<bool> used = vector<bool>(nodes);
    int best;

    tour[0] = 0;
    used[0] = true;

    //error om best = -1?
    for (int i = 1; i < nodes; ++i){
        best = -1;
        for (int j = 0; j < nodes; ++j){

            if (!used[j] && best == -1){
                best = j;

            }else if(distanceMatrix[tour[i-1]][j] < distanceMatrix[tour[i-1]][best] && !used[j]){
                best = j;
            }
        }
        tour[i] = best;
        used[best] = true;
    }

    // cout << "\n";
    // for (int i = 0; i < tour.size(); ++i){
    //     cout << tour[i] << " ";
    // }
    // cout << "\n";

    return tour;
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


    vector<int> path =  greedyAlgorithm();
    // print out greedy path
    //cout << "\n" << "greedy" << "\n";
    // for (int i = 0; i < path.size(); ++i){
    //     cout << path[i] << "\n";
    // }

    // // set a path to input order
    // vector<int> initPath;
    // for (int i = 0; i < nodes; ++i){
    //     initPath.push_back(i);
    // }

    // // vector<int> path = randomPath(initPath);
    // // vector<int> path = nearestNeighbor(nodes);
    // vector<int> path = initPath;

    // // do 2-opt
    vector<int> result = twoOpt(path);

    // print out result
    //cout << "\n" << "twoOpt" << "\n";

    for (int i = 0; i < result.size(); ++i){
        cout << result[i] << "\n";
    }

    //print path lengths
    //cout << "greedy: " << "lengthOfPath(path) << "\n";
    //cout << "2opt: " << "lengthOfPath(result) << "\n";

}
