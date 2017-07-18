# Tulip Infiniband Plugins
Plugins to import and work with Infiniband networks in Tulip

![Example Cluster](https://raw.githubusercontent.com/nateucar/tulip_infiniband/master/tulip_infinband_example.jpeg)

* NCAR: https://ncar.ucar.edu/
* Tulip Project: http://tulip.labri.fr/
* Tulip Plugin Development: http://tulip.labri.fr/Documentation/current/tulip-dev/html/plugin_development.html

NCAR has created a patch for the seamless import of Infiniband network topologies and related data. This project currently consists of 3 tulip plugins that utilize libibautils. It has been tested against 4.7.0 release. It does not require linking against OFED as this was found to be the most compatable way to work across multiple clusters. This plugin set has been tested on several 4+k node Infiniband clusters. The three new plugins are described as follows:
* Infiniband Topology Import:
 * This plugin will import the entire Infiniband fabric based on the output of the 'ibnetdiscover -p' command. Each Infiniband chip is created as a node and each physical cable is created as 2 directional edges.
* Infiniband CSV Importer:
 * This plugin imports CSV files created by the commonly created by Infiniband Monitoring applications that produce aggregated hardware counter values. The generally come in the form of hex encoded GUID, decimal port number and then a value (or set of them). This plugin exists to correctly import or correlate the CSV to the existing IB fabric that has already been loaded into Tulip. The current use of this import to get the traffic measurements for running fabrics.
* Infiniband Topology Import Routes:
 * This plugin imports the file ibdiagnet2.fdbs created by 'ibdiagnet -r' command. Currently, it fills out the ibRoutesOutbound field with number of routes outbound on a given cable (directional edge).

Special thanks to Patrick Mary for his help with this project.

Addition:
2017 SIParCS Intern: Zhenzhen Liu & Ananta Thapaliya
This summer project currently consists of 4 plugins that utilize libibautils. These four plugins are aimed to analyse the static topology of infiniband. The four new plugins are described as follows: 
* HelloWorld:
 * Tutorial of how to extend tulip and make a tulip plugin. It will print "Hello World in the terminal"
* Dijkstra Algorithm:
 * This plugin implements Dijkstra Algorithm. Firstly,for a selected source node in the tulip graph, the algorithm finds the    shortest path between that node and every other node.
   Second, it can also be used for finding the shortest paths from a single node to another single destination node by    
   storing the information of shortest path of each node and then tracing back from the destination to the source.

* Real Route Path:
 * This plugin imports the file ibdiagnet2.fdbs created by 'ibdiagnet -r' command. It will find the real routing path about this two nodes in the infiniband.
 
