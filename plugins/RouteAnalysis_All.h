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

#include <tulip/TulipPluginHeaders.h>
#include <tulip/StringProperty.h>
#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include "fabric.h"
#include <map>

#ifndef TULIPTEST_ROUTEANALYSIS_H
#define TULIPTEST_ROUTEANALYSIS_H

namespace ib = infiniband;

class RouteAnalysis_All: public tlp::Algorithm{
    PLUGININFORMATION("Count Real Routes - All Nodes",
                      "zz",
                      "06/27/2017",
                      "print out the real routing path",
                      "alphe",
                      "Infiniband");
public:
    RouteAnalysis_All(tlp::PluginContext* context);

    bool run();
    const ib::entity_t * getMyEntity(const tlp::node node,ib::tulip_fabric_t * const fabric);
    int count_hops(tlp::node source_node, tlp::node target_node,tlp::Graph * const graph);
    int help_count(ib::tulip_fabric_t * const fabric, tlp::Graph * const graph,
                            std::vector<ib::entity_t *> &tmp, const ib::entity_t * real_target);
};
#endif //TULIPTEST_ROUTEANALYSIS_H
