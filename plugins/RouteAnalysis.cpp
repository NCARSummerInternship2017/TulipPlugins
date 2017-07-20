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
#include <algorithm>
#include <string>
#include <set>
#include <vector>
#include "RouteAnalysis.h"

#include <tulip/GlScene.h>
#include <tulip/BooleanProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include "fabric.h"
#include "ibautils/ib_fabric.h"
#include "ibautils/ib_parser.h"
#include "ibautils/ib_port.h"
#include "ibautils/regex.h"

using namespace std;
using namespace tlp;

PLUGIN(RouteAnalysis)

static const char * paramHelp[] = {

  // File to Open
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "pathname" ) \
  HTML_HELP_BODY() \
  "Path to ibdiagnet2.fdbs file to import" \
  HTML_HELP_CLOSE()

};

//Constructor 
RouteAnalysis::RouteAnalysis(tlp::PluginContext* context)
        : tlp::Algorithm(context)
{
    addInParameter<std::string>("file::filename", paramHelp[0],"");
}

namespace ib = infiniband;
namespace ibp = infiniband::parser;

//A method to get entity from node
const ib::entity_t * RouteAnalysis::getMyEntity(const tlp::node node,ib::tulip_fabric_t * const fabric){
    for(ib::tulip_fabric_t::entity_nodes_t::iterator it1 = fabric->entity_nodes.begin(); it1 != fabric->entity_nodes.end(); ++it1)
        if(it1->second.id == node.id)
            return it1->first;

    return nullptr;
}

//Tulip's Main Function
bool RouteAnalysis::run(){
    assert(graph);

    static const size_t STEPS = 5;
    if(pluginProgress)
    {
        pluginProgress->showPreview(false);
        pluginProgress->setComment("Starting to Import Routes");
        pluginProgress->progress(0, STEPS);
    }

    /**
     * while this does not import
     * nodes/edges, it imports properties
     * for an existing fabric
     */

    ib::tulip_fabric_t * const fabric = ib::tulip_fabric_t::find_fabric(graph, false);
    if(!fabric)
    {
        if(pluginProgress)
            pluginProgress->setError("Unable find fabric. Make sure to preserve data when importing data.");

        return false;
    }

    if(pluginProgress)
    {
        pluginProgress->setComment("Found Fabric");
        pluginProgress->progress(1, STEPS);
    }

    /**
     * Open file to read and import per type
     */
    std::string filename;

    dataSet->get("file::filename", filename);
    std::ifstream ifs(filename.c_str());
    if(!ifs)
    {
        if(pluginProgress)
            pluginProgress->setError("Unable open source file.");

        return false;
    }

    if(pluginProgress)
    {
        pluginProgress->progress(2, STEPS);
        pluginProgress->setComment("Parsing Routes.");
    }

    ibp::ibdiagnet_fwd_db parser;
    if(!parser.parse(*fabric, ifs))
    {
        if(pluginProgress)
            pluginProgress->setError("Unable parse routes file.");

        return false;
    }

    if(pluginProgress)
    {
        pluginProgress->setComment("Parsing Routes complete.");
        pluginProgress->progress(3, STEPS);
    }

    ifs.close();


    //Finding the source node and target node
    BooleanProperty *selectBool = graph->getLocalProperty<BooleanProperty>("viewSelection");
    IntegerProperty *getPortNum = graph->getLocalProperty<IntegerProperty>("ibPortNum");
    vector<tlp::node> nodes;

    tlp::Iterator<tlp::node> *selections = selectBool->getNodesEqualTo(true,NULL);

    while(selections->hasNext()){
        const tlp::node &mynode = selections->next();
        nodes.push_back(mynode);
    }
  
    cout<<"Test the size of vector: "<<nodes.size()<<endl;
    cout<<"source_node_id: "<<nodes[0].id<<endl;
    cout<<"target_node_id: "<<nodes[1].id<<endl;
    cout<<" -------------------------------- "<<endl;

    const ib::entity_t * source_entity = getMyEntity(nodes[0],fabric);
    const ib::entity_t * target_entity = getMyEntity(nodes[1],fabric);

    cout<<"source_guid: "<<source_entity->guid<<endl;
    cout<<"target_guid: "<<target_entity->guid<<endl;
    cout<<" -------------------------------- "<<endl;
  
    
    std::vector<ib::entity_t *> tmp;

    //Number of hops initialized to 0
    unsigned int count_hops = 0;

    //Check if the source node is HCA(Host Channel Adapter). 
    if(getPortNum->getNodeValue(nodes[0])== 1){
        //Find the only port in that HCA
        const ib::entity_t::portmap_t::const_iterator Myport = source_entity->ports.begin();

        //Use the typedef std::map<port_t*, tlp::edge> port_edges_t to find the corresponding edge. 
        ib::tulip_fabric_t::port_edges_t::iterator Myedge = fabric->port_edges.find(Myport->second);
        selectBool->setEdgeValue(Myedge->second, true);
      
        if(graph->source(Myedge->second).id == nodes[0].id){
            const tlp::edge &e = Myedge->second;
            tmp.push_back(const_cast<ib::entity_t *> (getMyEntity(graph->target(e),fabric)));
            selectBool->setNodeValue(graph->target(e), true);
        }else{
            const tlp::edge &e = Myedge->second;
            tmp.push_back(const_cast<ib::entity_t *> (getMyEntity(graph->source(e),fabric)));
            selectBool->setNodeValue(graph->source(e), true);
        }
    }else{

        tmp.push_back(const_cast<ib::entity_t *> (source_entity));
    }

    cout<<"tmp vector size: "<<tmp.size()<<endl;
    cout<<tmp.back()->guid<<endl;
    cout<<"----------Test source entity ends---------------"<<endl;
    
    //Check if the target is an HCA(Host Channel Adapter). 
    if(getPortNum->getNodeValue(nodes[1])== 1){
        //Find the only port in HCA( HCA has only one port). 
        const ib::entity_t::portmap_t::const_iterator Myport = target_entity->ports.begin();

        //Use the typedef std::map<port_t*, tlp::edge> port_edges_t to find the edge. 
        ib::tulip_fabric_t::port_edges_t::iterator Myedge = fabric->port_edges.find(Myport->second);
        
        //Applies Tulip's selection algorithm to the edge. 
        selectBool->setEdgeValue(Myedge->second, true);
      
        if(graph->source(Myedge->second).id == nodes[1].id){
            const tlp::edge &e = Myedge->second;
            const ib::entity_t * real_target = getMyEntity(graph->target(e),fabric);
            ib::lid_t target_lid = real_target->lid();
            while(tmp.back()->guid!= real_target->guid) {
                const ib::entity_t & temp = *tmp.back();
                cout<<"The "<<count_hops<<" Step: "<<temp.guid<<endl;
                for (
                        ib::entity_t::routes_t::const_iterator
                                ritr = temp.get_routes().begin(),
                                reitr = temp.get_routes().end();
                        ritr != reitr;
                        ++ritr
                        ) {
                    std::set<ib::lid_t>::const_iterator itr = ritr->second.find(target_lid);
                    if (itr != ritr->second.end()) {
                        const ib::entity_t::portmap_t::const_iterator port_itr = temp.ports.find(ritr->first);
                        if (port_itr != temp.ports.end()) {
                            const ib::port_t *const port = port_itr->second;
                            const ib::tulip_fabric_t::port_edges_t::const_iterator edge_itr = fabric->port_edges.find(
                                    const_cast<ib::port_t *>(port));
                            if (edge_itr != fabric->port_edges.end()) {
                                const tlp::edge &edge = edge_itr->second;
                                selectBool->setEdgeValue(edge, true);
                                const ib::entity_t * node = getMyEntity(graph->target(edge),fabric);
                                selectBool->setNodeValue(graph->target(edge), true);
                                tmp.push_back(const_cast<ib::entity_t *> (node));
                                count_hops++;
                            }
                        }
                    }
                }
            }
            cout<<"The "<<count_hops<<" step: "<<real_target->guid<<endl;

        }else{
            //Check if the target is an HCA(Host Channel Adapter). 
            const ib::entity_t::portmap_t::const_iterator Myport = target_entity->ports.begin();

            //Use the typedef std::map<port_t*, tlp::edge> port_edges_t to find the corresponding edge. 
            ib::tulip_fabric_t::port_edges_t::iterator Myedge = fabric->port_edges.find(Myport->second);
            const tlp::edge &e = Myedge->second;
            selectBool->setEdgeValue(e, true);
            const ib::entity_t * real_target = getMyEntity(graph->source(e),fabric);
            ib::lid_t target_lid = real_target->lid();
          
            while(tmp.back()->guid!= real_target->guid) {
                const ib::entity_t & temp = *tmp.back();
                cout<<"The "<<count_hops<<" step: "<<temp.guid<<endl;
                for (
                        ib::entity_t::routes_t::const_iterator
                                ritr = temp.get_routes().begin(),
                                reitr = temp.get_routes().end();
                        ritr != reitr;
                        ++ritr
                        ) {

                    std::set<ib::lid_t>::const_iterator itr = ritr->second.find(target_lid);
                    if (itr != ritr->second.end()) {
                        const ib::entity_t::portmap_t::const_iterator port_itr = temp.ports.find(ritr->first);
                        if (port_itr != temp.ports.end()) {
                            const ib::port_t *const port = port_itr->second;
                            const ib::tulip_fabric_t::port_edges_t::const_iterator edge_itr = fabric->port_edges.find(
                                    const_cast<ib::port_t *>(port));
                            if (edge_itr != fabric->port_edges.end()) {
                                const tlp::edge &edge = edge_itr->second;
                                selectBool->setNodeValue(graph->target(edge), true);
                                selectBool->setEdgeValue(edge, true);
                                const ib::entity_t * node = getMyEntity(graph->target(edge),fabric);
                                tmp.push_back(const_cast<ib::entity_t *> (node));
                                count_hops++;
                            }
                        }
                    }else{

                        cout<<"Do not find routesmap"<<endl;
                    }
                }
            }
            cout<<"The "<<count_hops<<" step: "<<real_target->guid<<endl;

        }
        cout<<"The "<<++count_hops<<" step: "<<target_entity->guid<<endl;
        cout<<" ------------------------" <<endl;
        cout<<"The total hops: "<<count_hops<<endl;
    }
    else{
        ib::lid_t target_lid = target_entity->lid();
        while(tmp.back()->guid!= target_entity->guid) {
            const ib::entity_t *temp = tmp.back();
            bool find_next = false;
            cout<<"The "<<count_hops<<" step: "<<temp->guid<<endl;
            for (
                    ib::entity_t::routes_t::const_iterator
                            ritr = temp->get_routes().begin(),
                            reitr = temp->get_routes().end();
                    ritr != reitr;
                    ++ritr
                    ) {
                std::set<ib::lid_t>::const_iterator itr = ritr->second.find(target_lid);
                
                if (itr != ritr->second.end()) {
                    const ib::entity_t::portmap_t::const_iterator port_itr = temp->ports.find(ritr->first);
                    if (port_itr != temp->ports.end()) {
                        
                        const ib::port_t *const port = port_itr->second;
                        const ib::tulip_fabric_t::port_edges_t::const_iterator edge_itr = fabric->port_edges.find(
                                const_cast<ib::port_t *>(port));
                        if (edge_itr != fabric->port_edges.end()) {
                            
                            const tlp::edge &edge = edge_itr->second;
                            //setColor->setEdgeValue(edge, tlp::Color::SpringGreen);
                            selectBool->setEdgeValue(edge, true);
                            selectBool->setNodeValue(graph->target(edge), true);
                            const ib::entity_t * node =  getMyEntity(graph->target(edge),fabric);
                            tmp.push_back(const_cast<ib::entity_t *> (node));
                            count_hops++;
                            find_next = true;
                        }
                    }
                }
            }
            if(!find_next)
            break;
        }


        cout<<"The "<<count_hops<<" step: "<<target_entity->guid<<endl;
        cout<<" ------------------------" <<endl;
        cout<<"The total hops: "<<count_hops<<endl;

    }
    

    if(pluginProgress)
    {
        pluginProgress->setComment("Print the Real hops");
        pluginProgress->progress(STEPS, STEPS);
    }

    return true;
}

