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
#include "testRandom.h"
#include "fabric.h"
#include <tulip/GlScene.h>
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"
#include <stdlib.h>

#include <tulip/BooleanProperty.h>


using namespace tlp;
using namespace std;

/*PLUGIN(ImportInfinibandRoutes)*/
PLUGIN(Stochastic)

static const char * paramHelp[] = {
        // File to Open
        HTML_HELP_OPEN() \
  /*HTML_HELP_DEF( "type", "pathname")*/ \
  HTML_HELP_BODY() \
  "Hello World" \
  HTML_HELP_CLOSE()
};

Stochastic::Stochastic(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{
    //addInParameter<std::string>("file::filename", paramHelp[0],"");

}

namespace ib = infiniband;
namespace ibp = infiniband::parser;




bool Stochastic::run()
{
    assert(graph);

    static const size_t STEPS = 5;
    if(pluginProgress)
    {
        pluginProgress->showPreview(false);
        pluginProgress->setComment("Starting to Import Routes");
        pluginProgress->progress(0, STEPS);
    }



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




    if(pluginProgress)
    {
        pluginProgress->progress(2, STEPS);
        pluginProgress->setComment("Generating random nodes..");
    }



    if(pluginProgress)
    {
        pluginProgress->setComment("Creating a suitable range for random nodes..");
        pluginProgress->progress(3, STEPS);
    }





    if(pluginProgress)
    {
        pluginProgress->setComment("Waiting..");
        pluginProgress->progress(4, STEPS);
    }

    tlp::BooleanProperty * selectBool = graph->getLocalProperty<tlp::BooleanProperty>("viewSelection");

    for(int i =0;i<1000;i++){
        cout<<"Test: " <<i <<endl;
        graph->applyAlgorithm("Picks two random nodes from the graph", "Sorry! Something went wrong.", NULL, NULL);
        graph->applyAlgorithm("Dijkstra", "Sorry! Something went wrong", NULL, NULL);
        selectBool->setAllNodeValue(true, graph);

    }












    if(pluginProgress)
    {
        pluginProgress->setComment("Two random nodes generated and highlighted for testing..");
        pluginProgress->progress(STEPS, STEPS);
    }

    return true;
}