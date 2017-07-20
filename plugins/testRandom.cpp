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
#include "testRandom.h"
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/regex.h"


using namespace tlp;
using namespace std;

PLUGIN(testRandom)


testRandom::testRandom(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{


}





bool testRandom::run()
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









    BooleanProperty * pick = graph->getLocalProperty<BooleanProperty >("viewSelection");

    for(int i =0;i<1000;i++){
        cout<<"Test: " <<i <<endl;
        graph->applyAlgorithm("Picks two random nodes from the graph", "Sorry! Something went wrong.", NULL, NULL);
        graph->applyAlgorithm("Dijkstra", "Sorry! Something went wrong", NULL, NULL);
        selectBool->setAllNodeValue(true, graph);

    }





    if(pluginProgress)
    {
        pluginProgress->setComment("Calculating Route oversubscription complete.");
        pluginProgress->progress(STEPS, STEPS);
    }

    return true;
}
