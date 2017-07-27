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

#include <fstream>
#include "testRandom.h"
#include "fabric.h"
#include <tulip/GlScene.h>
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"
#include <stdlib.h> 
#include <ctime>
#include <cstdlib>
#include <chrono>

#include <tulip/BooleanProperty.h>

using namespace tlp;
using namespace std;

PLUGIN(Stochastic)

        //Constructor 
Stochastic::Stochastic(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{
    //addInParameter<std::string>("file::filename", paramHelp[0],"");

}

//Tulip's Main Function 
bool Stochastic::run()
{
    assert(graph);
        
    static const size_t STEPS = 5;
    if(pluginProgress)
    {
        pluginProgress->showPreview(false);
        pluginProgress->setComment("Starting to generate random Node Ids...");
        pluginProgress->progress(0, STEPS);
    }

    if(pluginProgress)
    {
        pluginProgress->setComment("Random numbers generated..");
        pluginProgress->progress(1, STEPS);
    }

    if(pluginProgress)
    {
        pluginProgress->progress(2, STEPS);
        pluginProgress->setComment("Generating random nodes..");
    }

    if(pluginProgress)
    {
        pluginProgress->setComment("Creating a suitable range for the random nodes..");
        pluginProgress->progress(3, STEPS);
    }

    if(pluginProgress)
    {
        pluginProgress->setComment("Waiting..");
        pluginProgress->progress(4, STEPS);
    }

   
    
    
     
   //Applying Tulip's selection algorithm...
    BooleanProperty * pick = graph->getLocalProperty<BooleanProperty >("viewSelection");
    std::string errMsg; 
    
    for(int i =0;i<1000;i++)
    {
        cout<<"Test: " <<i <<endl;
        graph->applyAlgorithm("Picks two random nodes from the graph", errMsg);
        graph->applyAlgorithm("Dijkstra", errMsg);
        pick->setAllNodeValue(false,graph);
        std::this_thread::sleep_for (std::chrono::seconds(1));
    }
   
        
    if(pluginProgress)
    {
        pluginProgress->setComment("Two random nodes generated and highlighted for testing..");
        pluginProgress->progress(STEPS, STEPS);
    }

    return true;
}
