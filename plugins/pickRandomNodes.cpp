//
// Created by anantat on 6/22/17.
//
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
#include "pickRandomNodes.h"
#include "fabric.h"
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"

#include <tulip/BooleanProperty.h>


using namespace tlp;
using namespace std;


PLUGIN(randomNodes)

static const char * paramHelp[] = {
        // File to Open
        HTML_HELP_OPEN() \
  /*HTML_HELP_DEF( "type", "pathname")*/ \
  HTML_HELP_BODY() \
  "Hello World" \
  HTML_HELP_CLOSE()
};

randomNodes::randomNodes(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{
    addInParameter<std::string>("file::filename", paramHelp[0],"");

}

namespace ib = infiniband;
namespace ibp = infiniband::parser;



        
        



bool randomNodes::run()
{
    assert(graph);

    static const size_t STEPS = 5;
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

    ibp::ibdiagnet_fwd_db parser;
    

    if(pluginProgress)
    {
        pluginProgress->setComment("Parsing Routes complete.");
        pluginProgress->progress(3, STEPS);
    }

    ifs.close();

    

    if(pluginProgress)
    {
        pluginProgress->setComment("Calculating Route oversubscription.");
        pluginProgress->progress(4, STEPS);
    }
        
       //To select to random nodes
    
    BooleanProperty *pick = graph->getLocalProperty<BooleanProperty >("viewSelection");
    cout<<"Yes";

    Iterator<node> *itnodes = graph->getNodes();
    cout<<"Yes";

    unsigned int v = 0; //Number of nodes
    cout<<"Yes";
    while(itnodes->hasNext()){
        v++;
    }
    cout<<"Yes";

    unsigned int randSource = 0;
    unsigned int randDestination = 0;
    while (true){
        randSource = (rand()%v);
        randDestination = (rand()%v);
        if ((randSource<v) && (randDestination!=randSource) && (randDestination<v)){
            break;
        }
    }
    cout<<"Yes";

    unsigned int maxID = max(randSource,randDestination);
    cout<<"Yes";
    Iterator<node> *itnode = graph->getNodes();
    while(itnode->hasNext()){
        const tlp::node &mynode = itnodes->next();
        if (mynode.id == randSource){
            pick->setNodeValue(mynode,true);
        }

        if(mynode.id == randDestination){
            pick->setNodeValue(mynode,true);
        }

        if (mynode.id > maxID){
            break;
        }
    }
    cout<<"YES";


   
    if(pluginProgress)
    {
        pluginProgress->setComment("Implementation of Dijkstra's Algorithm complete...");
        pluginProgress->progress(STEPS, STEPS);
    }

    return true;
}

