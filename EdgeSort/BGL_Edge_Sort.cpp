#include <boost/assign.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

typedef boost::property<boost::edge_weight_t, float> EdgeWeightProperty;

typedef boost::undirected_graph<boost::no_property, EdgeWeightProperty> Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor g_traits;

typedef boost::graph_traits<Graph>::edge_iterator edge_iter;

typedef boost::graph_traits<Graph>::edge_descriptor Edge;

typedef typename boost::property_map<Graph, boost::edge_weight_t>::type EdgeWeightType;

typedef typename boost::property_traits<EdgeWeightType>::value_type w_value;

typedef boost::indirect_cmp<EdgeWeightType, std::greater<w_value> > weight_greater;

void ConstructGraph(Graph& g)
{
	 g_traits* v_prev = new g_traits;
	 *v_prev = g.add_vertex();
	for(unsigned int i = 1; i < 100; ++i)
	{
		boost::graph_traits<Graph>::vertex_descriptor v_new = g.add_vertex();
		EdgeWeightProperty w = 100.0f - (float)i;
		g.add_edge(*v_prev, v_new, w);
		*v_prev = v_new;
	}
}

std::priority_queue<Edge, std::vector<Edge>, weight_greater> SortEdges(Graph& g)
{
	EdgeWeightType edgeWeightMap = get(boost::edge_weight_t(), g);
	
	weight_greater wl(edgeWeightMap);

	std::priority_queue<Edge, std::vector<Edge>, weight_greater> Q(wl);

	typename boost::graph_traits<Graph>::edge_iterator ei, eiend;
	for(boost::tie(ei, eiend) = boost::edges(g); ei != eiend; ++ei){
		Q.push(*ei);
	}

	return Q;
}

int main(int,char*[])
{
	// Create a graph object
	Graph g;

	ConstructGraph(g);

	EdgeWeightType edgeWeightMap = get(boost::edge_weight_t(), g);

	std::cout << "\n\n unsorted: \n\n" << std::endl;
	std::pair<edge_iter, edge_iter> edgePair;
	for(edgePair = edges(g); edgePair.first != edgePair.second; ++edgePair.first)
	{
	    std::cout << edgeWeightMap[*edgePair.first] << " ";
	}

	std::cout << "\n\n sorted: \n\n" << std::endl;
	std::priority_queue<Edge, std::vector<Edge>, weight_greater> Q = SortEdges(g);

	while(!Q.empty())
	{
	    float w = edgeWeightMap(Q.top());
		std::cout << w << " ";
		Q.pop();
	}
	std::cout << std::endl;

	return 0;
}
