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

static const char * paramHelp[] = {
  // File to Open
  HTML_HELP_OPEN() \
  /*HTML_HELP_DEF( "type", "pathname")*/ \
  HTML_HELP_BODY() \
  "Hello World" \
  HTML_HELP_CLOSE()
};

HelloWorld::HelloWorld(tlp::PluginContext* context)
  : tlp::Algorithm(context)
{
  addInParameter<std::string>("file::filename", paramHelp[0],"");
    
}

namespace ib = infiniband;
namespace ibp = infiniband::parser;







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

  ib::tulip_fabric_t * const fabric = ib::tulip_fabric_t::find_fabric(graph, false);
  if(!fabric)
  {
    if(pluginProgress)
      pluginProgress->setError("Unable find fabric. Make sure to preserve data when importing data.");

    return false;
  }

  if(pluginProgress)
  {
    pluginProgress->setComment("Found Fabric");
    pluginProgress->progress(1, STEPS);
  }

  /**
   * Open file to read and import per type
   */
  std::string filename;
  
  dataSet->get("file::filename", filename);
  std::ifstream ifs(filename.c_str());
  if(!ifs)
  {
    if(pluginProgress)
      pluginProgress->setError("Unable open source file.");

    return false;
  }

  if(pluginProgress)
  {
    pluginProgress->progress(2, STEPS);
    pluginProgress->setComment("Parsing Routes.");
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

  
  tlp::Iterator<node> *itnod = graph->getNodes();
  tlp::Iterator<edge> *itedge = graph->getEdges();
  int num_nodes = 0; 
  int num_edges = 0;
  
  
  while( itnod->hasNext()){
    
    num_nodes++;
  }
  
  
  
  while (itedge->hasNext()){
   num_edges++;
  }
 
 
  cout << "Hello World!"<<endl; 
  cout << "This graph has "<< num_nodes <<" nodes and " <<  num_edges<<" edges." <<endl; 

  if(pluginProgress)
  {
    pluginProgress->setComment("Calculating Route oversubscription complete.");
    pluginProgress->progress(STEPS, STEPS);
  }

  return true;
}
