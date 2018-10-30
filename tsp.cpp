#include <fstream>
#include <iostream>
#include <cstdio>
#include <vector>
#include <tuple>
#include <algorithm>


using namespace std;


int main(){

    //Retrieve all the nodes as a vector of tuples
    //Node 0 is the 0th node in the vector, node 1 is the 1st vectors etc.
    int points;
    cin >> points;
    vector < tuple <double, double> > v;
    for(int i = 0; i < points; i++){
        double value1, value2;
        cin >> value1 >> value2;
        auto foo = make_tuple(value1,value2);
        v.push_back(foo);
    }

    //Build a Nx(N-1) matrix consisting of all the routes from each node to all the others
    vector < vector <int> > distanceMatrix(points);
    for(int i = 0; i < points; i++){
        double x = get<0>(v[i]);
        double y = get<1>(v[i]);
        for(int j = 0; j < points; j++){
            double xOther = get<0>(v[j]);
            double yOther = get<1>(v[j]);
            double xDist = pow(abs(x-xOther),2);
            double yDist = pow(abs(y-yOther),2);
            int distance = sqrt(xDist + yDist);
            distanceMatrix[i].push_back(distance);
        }
    }


    //Return the nodes
    for(int i = 0; i < points; i++){
        cout << get<0>(v[i]) << " " << get<1>(v[i]) << " ";
        cout << "\n";
    }
    cout << "\n";
    //Return the matrix
    for(int i = 0; i < points; i++){
        for(int j = 0; j < points; j++){
            cout << distanceMatrix[i][j] << " ";
        }
        cout << "\n";

    }
    cout << "\n";
}