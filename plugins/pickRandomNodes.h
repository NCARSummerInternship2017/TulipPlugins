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

#pragma once

#include <tulip/TulipPluginHeaders.h>
#include <iostream>
#include <algorithm>

#ifndef IB_ROUTES_H
#define IB_ROUTES_H

class randomNodes: public tlp::Algorithm {
public:
    PLUGININFORMATION("Picks two random nodes from the graph",
    "Ananta Thapaliya & Zenzhen Liu",
    "06/16/15",
    "Picks two random nodes from the graph for testing purposes.",
    "alpha",
    "Infiniband")

//Constructor
    randomNodes(tlp::PluginContext* context);

//Tulip's Main Function 
    bool run();
};

#endif // IB_ROUTES_H

