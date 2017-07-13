//
// Created by anantat on 7/12/17.
//**

#pragma once

#include <tulip/TulipPluginHeaders.h>
#include <iostream>
#include <algorithm>
#include "pickRandomNodes.h"
#include "realRoutes.h"

#ifndef IB_ROUTES_H
#define IB_ROUTES_H

class testRandom: public tlp::Algorithm, public randomNodes, public RouteAnalysis_All {
public:
    PLUGININFORMATION("Test random nodes",
    "Ananta Thapaliya",
    "06/16/15",
    "Picks any two random nodes from the graph for testing purposes.",
    "alpha",
    "Infiniband")

    testRandom(tlp::PluginContext* context);

    /**
     * @brief import infiniband routes
     * @warning currently only works if static data is retained from import
     */
    bool run();
};

#endif // IB_ROUTES_H



