//
// Created by anantat on 7/12/17.
//**

#pragma once

#include <tulip/TulipPluginHeaders.h>
#include <iostream>
#include <algorithm>

#ifndef IB_ROUTES_H
#define IB_ROUTES_H

class randomNodes: public tlp::Algorithm {
public:
    PLUGININFORMATION("Pick Random Nodes",
    "Ananta Thapaliya",
    "06/16/15",
    "Picks any two random nodes from the graph for testin.",
    "alpha",
    "Infiniband")

    ImportInfinibandRoutes(tlp::PluginContext* context);

    /**
     * @brief import infiniband routes
     * @warning currently only works if static data is retained from import
     */
    bool run();
};

#endif // IB_ROUTES_H

