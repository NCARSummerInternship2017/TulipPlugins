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
 *
 * Created by zliu58 on 6/27/17.
 * Update by zliu58 on 7/5/17.
 * Update by zliu58 on 7/11/17.
 * Update by zliu58 on 7/20/17.
 */

#include <tulip/TulipPluginHeaders.h>
#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include "fabric.h"
#include <map>

#ifndef TULIPTEST_ROUTEANALYSIS_H
#define TULIPTEST_ROUTEANALYSIS_H

namespace ib = infiniband;

class RouteAnalysis: public tlp::Algorithm{
    PLUGININFORMATION("Real Routes",
                      "zz",
                      "06/27/2017",
                      "print out the real routing path",
                      "alphe",
                      "Infiniband");
public:
    RouteAnalysis(tlp::PluginContext* context);

    bool run();
    
    const ib::entity_t * getMyEntity(const tlp::node node,ib::tulip_fabric_t * const fabric);
};
#endif //TULIPTEST_ROUTEANALYSIS_H
