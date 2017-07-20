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

class testRandom: public tlp::Algorithm {
public:
    PLUGININFORMATION("Stochastic Analysis",
    "Ananta Thapaliya",
    "06/16/15",
    "Runs Dijkstra on the graph thousand times for two random nodes each time. ",
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



