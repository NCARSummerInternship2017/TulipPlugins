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
#include <tulip/GlScene.h>
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"
#include <stdlib.h> 
#include <ctime>
#include <cstdlib>

#include <tulip/BooleanProperty.h>

using namespace tlp;
using namespace std;

PLUGIN(randomNodes)

        //Constructor 
randomNodes::randomNodes(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{
    //addInParameter<std::string>("file::filename", paramHelp[0],"");

}

//Tulip's Main Function 
bool randomNodes::run()
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

    //Calculating the total number of nodes    
    tlp::Iterator<node> *itnod = graph->getNodes();
    unsigned int v = 0;
    
    while( itnod->hasNext()){
        const node &n = itnod->next();
        v++;
    }
    
    cout << "Hello World! \nThere are " << v << " nodes in this graph\n";
     
   //Applying Tulip's selection algorithm...
    BooleanProperty * pick = graph->getLocalProperty<BooleanProperty >("viewSelection");

    tlp:Iterator<node> *itnodes = graph->getNodes();

    unsigned int randSource = 0; //Random source initialized to 0
    unsigned int randDestination = 0; //Random destination initialized to 0
    
    //Creating random numbers of a suitable range
    while (true){
        srand(time(NULL));
        randSource = rand()%v;
        randDestination = (rand()%v);
        if ((randSource<v) && (randDestination!=randSource) && (randDestination<v)){
            break;
        }
    }
    cout << "Random source node is " << randSource << " .\nRandom destination node is " << randDestination << ".\n";

    unsigned int maxID = max(randSource,randDestination);

    //Selecting the nodes corresponding the two random integers (ids) generated above 
    while(itnodes->hasNext()){
        const node &mynode = itnodes->next();
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
    //END
        
    if(pluginProgress)
    {
        pluginProgress->setComment("Two random nodes generated and highlighted for testing..");
        pluginProgress->progress(STEPS, STEPS);
    }

    return true;
}
