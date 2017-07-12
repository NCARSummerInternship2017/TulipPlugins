//
// Created by anantat on 7/12/17.
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
#include "routes.h"
#include "fabric.h"
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"


using namespace tlp;
using namespace std;

PLUGIN(ImportInfinibandRoutes)

static const char * paramHelp[] = {
        // File to Open
        HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "pathname" ) \
  HTML_HELP_BODY() \
  "Path to ibdiagnet2.fdbs file to import" \
  HTML_HELP_CLOSE()
};

InfinibandAnalysis::InfinibandAnalysis(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{


}





bool ImportInfinibandRoutes::run()
{
    assert(graph);

    static const size_t STEPS = 2;
    if(pluginProgress)
    {
        pluginProgress->showPreview(false);
        pluginProgress->setComment("Starting to Import Routes");
        pluginProgress->progress(0, STEPS);
    }





    if(pluginProgress)
    {
        pluginProgress->setComment("Processing..");
        pluginProgress->progress(1, STEPS);
    }




    if(pluginProgress)
    {
        pluginProgress->progress(2, STEPS);
        pluginProgress->setComment("Generating random nodes..");
    }








   //To select to random nodes
    BooleanProperty * pick = graph->getLocalProperty<BooleanProperty >("viewSelection");

    tlp:Iterator<node> *itnodes = graph->getNodes();

    int v = 0; //Number of nodes

    while(itnodes->hasNext()){
        v++;
    }

    int randSource = 0;
    int randDestination = 0;
    while (true){
        randSource = (rand()%v);
        randDestination = (rand()%v);
        if ((randSource<v) && (randDestination!=randSource) && (randDestination<v)){
            break;
        }
    }

    int maxID = max(randSource,randDestination);

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






    if(pluginProgress)
    {
        pluginProgress->setComment("Calculating Route oversubscription complete.");
        pluginProgress->progress(STEPS, STEPS);
    }

    return true;
}

