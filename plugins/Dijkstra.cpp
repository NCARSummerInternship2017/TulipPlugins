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
       cout<<"<--"<<distmap[pos]->getFrom()<<" ";
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
   
   
    //Get the total number of nodes 'v'
    tlp::Iterator<tlp::node> *itnod = graph->getNodes();

    int v = 0; 
    while(itnod->hasNext()){
       itnod->next();
        v++;
    }
   
        
    //Tulip's selection algorithm
    BooleanProperty *selectBool = graph->getLocalProperty<BooleanProperty>("viewSelection"); //Tulip's Boolean Property to access "viewSelection."
   
    //Get an iterator to access all the selected nodes(To select means to set the node value of viewSelection to be true)
    tlp::Iterator<node> *selections = selectBool->getNodesEqualTo(true,NULL); 
    
    int path_node[1]; //an array to store source node and destination node Ids. 
    path_node[0]=0;// Default source node is 0
    int path_id = 0;
   
    while(selections->hasNext()){
        const node &mynode = selections->next();
      
         //If more than one node are selected show the error
         if(path_id>=1)
         {
           if(pluginProgress)
             pluginProgress->setError("More than one node are selected");

           return false;
         }
      
        path_node[path_id++] = mynode.id;
    }

    //If no node is selected show the error
    if(path_id == 0)
    {
      if(pluginProgress)
        pluginProgress->setError("No node is selected");

      return false;
    }

   // If select two nodes then find the shortest path between them     
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
   
    if(pluginProgress)
    {
        pluginProgress->setComment("Dijkstra's Algorithm Implemented");
        pluginProgress->progress(STEPS, STEPS);
    }

    return true;
}
