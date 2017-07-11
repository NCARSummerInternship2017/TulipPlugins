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

#include "helloworld.h"

#include "fabric.h"
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"

using namespace tlp;
using namespace std;

/*PLUGIN(ImportInfinibandRoutes)*/
PLUGIN(HelloWorld)
/**
static const char * paramHelp[] = {
   // File to Open
   HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "pathname") \
  HTML_HELP_BODY() \
  "Hello World" \
  HTML_HELP_CLOSE()
};
**/

HelloWorld::HelloWorld(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{
    //addInParameter<std::string>("file::filename", paramHelp[0],"");

}








bool HelloWorld::run()
{
    assert(graph);

    static const size_t STEPS = 4;
    if(pluginProgress)
    {
        pluginProgress->showPreview(false);
        pluginProgress->setComment("Implementing Dijkstra's algorithm on the nodes...");
        pluginProgress->progress(0, STEPS);
    }

    /**
     * while this does not import
     * nodes/edges, it imports properties
     * for an existing fabric
     */

    
   
    if(pluginProgress)
    {
        pluginProgress->setComment("Running...");
        pluginProgress->progress(1, STEPS);
    }

    /**
     * Open file to read and import per type
     */
    

    
    if(pluginProgress)
    {
        pluginProgress->progress(2, STEPS);
        pluginProgress->setComment("Calculating the number of nodes..");
    }

   
  
  

   
    tlp::Iterator<tlp::node> *itnodes = graph->getNodes();
    tlp::Iterator<tlp::edge> *itedges = graph->getEdges();
    int num_nodes = 0;
    int num_edges = 0;

    while(itnodes->hasNext()){
      
        num_nodes++;
    }
  
    while (itedges->hasNext()){
      num_edges++;
    }
  
    cout <<"Hello World!";
    cout << "The graph has: \n "<< num_nodes << " number of nodes, and \n" << num_edges <<"number of edges";
   
   if(pluginProgress)
    {
        pluginProgress->progress(3, STEPS);
        pluginProgress->setComment("Using BooleanProperty to update viewSelection..");
    }
   
   
   
  
   
    if(pluginProgress)
    {
        pluginProgress->setComment("Dijkstra's Algorithm implemented...");
        pluginProgress->progress(STEPS, STEPS);
    }

    return true;
}
