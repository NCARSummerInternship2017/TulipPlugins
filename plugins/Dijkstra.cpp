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

#include <fstream>
#include <algorithm>
#include <vector>

#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <tulip/GlScene.h>
#include <tulip/GraphIterator.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>

#include "Dijkstra.h"

#include "fabric.h"
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"

using namespace tlp;
using namespace std;

PLUGIN(InfinibandAnalysis)

//----------------------------------------*Constructor*-----------------------------------------------------

InfinibandAnalysis::InfinibandAnalysis(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{
    //addInParameter<std::string>("file::filename", paramHelp[0],"");

}

//-----------------------------------------*Implementing min_distance*-----------------------------------------
int InfinibandAnalysis::nodes_map::min_distance(map<int, InfinibandAnalysis::nodes_map::myNode*> map1, bool visited[]){
    int min = INT_MAX;
    int min_index = 0;

    for(int i = 0; i<v; i++){
        if(!visited[i] && map1[i]->getDist() < min)
            min = map1[i]->getDist(), min_index = i;
    }

    return min_index;
}

//-------------------------------------*Implementing printResult*----------------------------------------
void InfinibandAnalysis::nodes_map::printResult(map<int, InfinibandAnalysis::nodes_map::myNode*> map1) {
    for(int i = 0; i<v; i++){
        cout<<i<<" is from: "<<map1[i]->getFrom()<<" its distance is: "<<map1[i]->getDist()<<endl;
    }
}

//---------------------------------------*Implementing Dijkstra's Algorithm*------------------------------
map<int,InfinibandAnalysis::nodes_map::myNode*> InfinibandAnalysis::nodes_map::dijkstra(int src) {
    map<int, InfinibandAnalysis::nodes_map::myNode*> distmap;
    bool visited[v];

    for (int i = 0; i < v; i++) {
        distmap[i] = new InfinibandAnalysis::nodes_map::myNode(i, INT_MAX);
        visited[i] = false;
    }
    distmap[src]->setDist(0);

    for (int count = 0; count < v - 1; count++) {
        int u = min_distance(distmap, visited);
        visited[u] = true;

        for (int i = 0; i < v; i++) {
            if (!visited[i] && adjacent_matrix[u][i] && distmap[u]->getDist() != INT_MAX &&
                distmap[u]->getDist() + adjacent_matrix[u][i] < distmap[i]->getDist())
            {
                distmap[i]->setDist(distmap[u]->getDist() + adjacent_matrix[u][i]);
                distmap[i]->setFrom(u);
            }
        }
    }
    
    return distmap;
}
//----------------------------------*Tracing back the path suggested by Dijkstra*------------------------------------

vector<unsigned int> InfinibandAnalysis::nodes_map::tracePath(map<int, InfinibandAnalysis::nodes_map::myNode*> distmap, int target, int src){
    cout<<"the destination is: "<<target<<endl;
    vector<unsigned int> path;
    int pos = target;
    for(int i = 0; i<v-1; i++){
       path.push_back(pos);
       cout<<"next step is: "<<distmap[pos]->getFrom()<<" ";
       pos = distmap[pos]->getFrom();
       if(pos == src){
         path.push_back(pos);
         break;
       }
    }
    cout<<" "<<endl;
   
   return path;
}

//--------------------------------*Implementing the method to find node from its id*----------------------------------------------
const tlp::node & InfinibandAnalysis::find_node(unsigned int id){
     tlp::Iterator<tlp::node> *itnodes = graph->getNodes();
       
       while(itnodes->hasNext()){
          const tlp::node &node = itnodes->next();
             if(node.id == id)
                return node;  
       }
}

//----------------------------------------*Tulip's Main Function*----------------------------------------------
bool InfinibandAnalysis::run()
{
    assert(graph);

    static const size_t STEPS = 5;
   
   //PluginProcess interacts with users as the plugin runs
    if(pluginProgress)
    {
        pluginProgress->showPreview(false);
        pluginProgress->setComment("Starting to Import Routes");
        pluginProgress->progress(0, STEPS);
    }
        
    if(pluginProgress)
    {
        pluginProgress->setComment("Implementing Dijkstra's algorithm on the graph...");
        pluginProgress->progress(1, STEPS);
    }

    if(pluginProgress)
    {
        pluginProgress->progress(2, STEPS);
        pluginProgress->setComment("Finalizing the process..");
    }

    if(pluginProgress)
    {
        pluginProgress->setComment("Please wait..");
        pluginProgress->progress(3, STEPS);
    }

    if(pluginProgress)
    {
        pluginProgress->setComment("Calculating the minimum distances..");
        pluginProgress->progress(4, STEPS);
    }
   
   //Calculating the total number on nodes 'v'
    tlp::Iterator<tlp::node> *itnod = graph->getNodes();
    int v = 0; //Initialized to 0 

    while(itnod->hasNext()){
       itnod->next();
        v++;
    }
   
   //Tulip's selection algorithm
    BooleanProperty *selectBool = graph->getLocalProperty<BooleanProperty>("viewSelection"); //Tulip's Boolean Property to access "viewSelection."
   
//Get an iterator to access all the selected nodes(To select means to set the node value of viewSelection to be true)
    tlp::Iterator<node> *selections = selectBool->getNodesEqualTo(true,NULL); 
    
    int path_node[2]; //an array to store source node and destination node Ids. 
    path_node[1]=0;// Default source node is 0
    int path_id = 0;
    bool found_path = false;
   
    while(selections->hasNext()){
        const node &mynode = selections->next();
        path_node[path_id++] = mynode.id;
    }
   
   if(path_id >1) found_path = true;
   
    nodes_map *graphAnalysis = new nodes_map(graph,v);
    
    map<int, InfinibandAnalysis::nodes_map::myNode*> mymap = graphAnalysis->dijkstra(path_node[0]);

    int max = 1;
    
    //Print Distance and find out the max and min numbers
    for(int i = 0; i<v; i++){
        max = std::max(max,mymap[i]->getDist());
        cout<<"Node id: "<<i<<" ---- The shortest distance: "<<mymap[i]->getDist()<<endl;
    }
   
    cout<<"*************************************************************************"<<endl;
    cout<<""<<endl;
    std::vector<unsigned int> mypath;
   
    tlp::IntegerProperty * ibHop = graph->getProperty<tlp::IntegerProperty>("ibHop");
    assert(ibHop);
   
    if(pluginProgress)
    {
        pluginProgress->setComment("Show the max min average steps");
        pluginProgress->progress(5, STEPS);
    }

    tlp::Iterator<tlp::node> *itnodes = graph->getNodes();
    while(itnodes->hasNext()){
        const tlp::node &node = itnodes->next();
        const int &temp = mymap[node.id]->getDist();
        ibHop->setNodeValue(node, temp);
    }

    //Make the path found visible in graph
    if(found_path)
    {
       mypath = graphAnalysis->tracePath(mymap,path_node[1],path_node[0]);
       tlp::ColorProperty * resetColor = graph->getLocalProperty<tlp::ColorProperty>("viewColor");
       itnodes = graph->getNodes();
       
       while(itnodes->hasNext()){
          const tlp::node &node = itnodes->next();
          for(unsigned int ID : mypath){
             if(node.id == ID){
                resetColor->setNodeValue(node, Color::SpringGreen);
                selectBool->setNodeValue(node, true);
             }
          }
       }
       
      //Selects the edges that comprise Dijkstra's path 
       for(unsigned int i = 0; i<mypath.size()-1; i++){
            const tlp::node &source = find_node(mypath[i]);
            tlp::Iterator<tlp::edge> *itedges = graph->getOutEdges(source);
            while(itedges->hasNext()){
                const tlp::edge &edge = itedges->next();
                if(graph->target(edge).id == mypath[i+1])
                    selectBool->setEdgeValue(edge, true);
            }
        }
      
    }
   
    if(pluginProgress)
    {
        pluginProgress->setComment("Dijkstra's Algorithm Implemented");
        pluginProgress->progress(STEPS, STEPS);
    }

    return true;
}
