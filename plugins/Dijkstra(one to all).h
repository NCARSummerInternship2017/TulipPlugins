/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux, University Corporation
 * for Atmospheric Research
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */


#include <tulip/TulipPluginHeaders.h>
#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <map>
#include <vector>

#ifndef TULIPTEST_MAIN_H
#define TULIPTEST_MAIN_H

using namespace tlp;
using namespace std;

class InfinibandAnalysis: public tlp::Algorithm{
public:
    PLUGININFORMATION("Dijkstra",
    "Zehzhen Liu & Ananta Thapaliya",
    "06/21/17",
    "Implement Dijkstra's algorithm on the graph",
    "alpha",
    "Infiniband")

    InfinibandAnalysis(tlp::PluginContext* context); //Constructor 
    
//Declaration of adjacency matrix
    class nodes_map{
    private:
        int v; //number of node;
        int **adjacent_matrix;
    public:
        class myNode{
           int from;
           int dist;
        public:
            myNode(int from, int dist = INT_MAX){
                this->from = from;
                this->dist = dist;
            };

            void setDist(int d){dist = d; }
            int getDist(){return dist;}

            void setFrom(int f){from = f;}
            int getFrom(){return from;}

        };

    nodes_map(const Graph *graph, int v){
        initMap(graph, v);
    }
    ~nodes_map(){
        delete adjacent_matrix[v];
    }

    void initMap(const Graph *graph, int v){
        this->v = v;
        adjacent_matrix = new int* [v];
        for(int i = 0; i<v; i++){
            adjacent_matrix[i] = new int[v];
        }

        //Initialize the adjacent_matrix
        for(int i = 0; i<v; i++){
            for(int j = 0; j<v; j++){
                adjacent_matrix[i][j] = 0;
            }
        }

        //Initialize the adjacency matrix
        tlp::Iterator<edge> *ite = graph->getEdges();
        while(ite->hasNext()){
            edge e = ite->next();
            int s = graph->source(e).id, t = graph->target(e).id;
            if(!adjacent_matrix[s][t]){
                adjacent_matrix[s][t] = 1;
                adjacent_matrix[t][s] = 1;
            }
        }
    }
 
//A method to calculate minimun distance between two nodes
    int min_distance(map<int, myNode*> map1, bool visited[]);
        
//A method that implements Dijkstra's algorithm on the graph
    map<int,myNode*> dijkstra(int src);
        
//A method that prints the results in the terminal
    void printResult(map<int, myNode*> map1);
        
//A method that traces the route suggested by Dijkstra's algorithm
    vector<unsigned int> tracePath(map<int, myNode*> distmap, int target,int src);
    };

//Main function for tulip
    bool run();
    
//A method that returns a node from its id
    const tlp::node & find_node(unsigned int id); //Get node form id
};
#endif //TULIPTEST_MAIN_H
