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


  
    return true;
}

