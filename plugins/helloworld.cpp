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

#include<fstream>
#include <algorithm>
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
  HTML_HELP_DEF( "type", "pathname")\
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

  static const size_t STEPS = 6;
  if(pluginProgress)
  {
    pluginProgress->showPreview(false);
    pluginProgress->setComment("Starting to Import Routes");
    pluginProgress->progress(0, STEPS);
  }

  /**
   * while this does not import
   * nodes/edges, it imports properties
   * for an existing fabric
   */

  if(pluginProgress)
  {
    pluginProgress->setComment("Counting the nodes");
    pluginProgress->progress(1, STEPS);
  }

  /**
   * Open file to read and import per type
   */
 

  if(pluginProgress)
  {
    pluginProgress->progress(2, STEPS);
    pluginProgress->setComment("Parsing Routes.");
  }
  
  

  if(pluginProgress)
  {
    pluginProgress->setComment("Parsing Routes complete.");
    pluginProgress->progress(3, STEPS);
  }

  
      
  /**
   * calculate routes outbound
   * from every port on the fabric
   */
  

  if(pluginProgress)
  {
    pluginProgress->setComment("Calculating Route oversubscription.");
    pluginProgress->progress(4, STEPS);
  }

  
  tlp::Iterator<node> *itnod = graph->getNodes(); //Nodes Iterator
  tlp:Iterator<edge> *itedge = graph->getEdges(); //Edges Iterator
  int n_nodes = 0;
  int n_edges = 0;
  
  
  while( itnod->hasNext()){
    
    n_nodes++;
  }
  
  while (itedge->hasNext()){
    n_edges++;
  }
  
  cout << "Hello World!";
  cout << "There are " <<n_nodes<< " and" << n_edges << " edges in this graph";
  
  
  
  if(pluginProgress)
  {
    pluginProgress->setComment("Show the max min average steps");
    pluginProgress->progress(5, STEPS);
  }
  
  

  if(pluginProgress)
  {
    pluginProgress->setComment("Calculating Route oversubscription complete.");
    pluginProgress->progress(STEPS, STEPS);
  }

  return true;
}
