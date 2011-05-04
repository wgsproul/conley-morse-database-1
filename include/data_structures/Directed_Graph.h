/*
 *  Directed_Graph.h
 */

#ifndef _CMDP_DIRECTED_GRAPH_
#define _CMDP_DIRECTED_GRAPH_

#include <stack>

#include <map>
#include <vector>
#include <set>
#include <iostream>

template < class Toplex >
class DirectedGraph : public std::map < typename Toplex::Top_Cell, 
                                        typename Toplex::Subset > 
{
public:
  typedef typename Toplex::Top_Cell Vertex;
  typedef typename Toplex::Subset Component;
  typedef std::vector<Component> Components;

  // Constructor
  DirectedGraph() { };

  // Destructor
  ~DirectedGraph() { };

  // Remove Vertex v from the graph
  void removeVertex(Vertex);
      
  // Functional Notation when Directed Graph is considered as a Combinatorial Map
  typename Toplex::Subset & operator () ( const typename Toplex::Top_Cell & );
  const typename Toplex::Subset & operator () ( const typename Toplex::Top_Cell & ) const;
  typename Toplex::Subset operator () ( const typename Toplex::Subset & );

private:
};

template < class Toplex, class Map >
DirectedGraph<Toplex> compute_directed_graph (const Toplex & my_toplex, 
                                              const Map & f);

template < class Toplex, class Map >
DirectedGraph<Toplex> compute_directed_graph (const typename Toplex::Subset & my_subset,
                                              const Toplex & my_toplex, 
                                              const Map & f);

template < class Toplex, class Map >
DirectedGraph<Toplex> compute_directed_graph (const std::vector < typename Toplex::Subset > & sets,
                                              const Toplex & my_toplex, 
                                              const Map & f);
// Collapse components
template < class Toplex >
DirectedGraph<Toplex> collapseComponents (
    const DirectedGraph<Toplex> & G,
    const typename DirectedGraph<Toplex>::Components & Components,
    std::vector<typename DirectedGraph<Toplex>::Vertex> & Representatives);


// Computation of reachability information given a DAG and vertices of interest
template < class Graph >
void compute_reachability ( std::vector < std::vector < typename Graph::Vertex > > * output, 
                      const Graph & H, 
                      const std::vector < typename Graph::Vertex > & representatives );

// Compute Morse sets
template < class Toplex >
void compute_morse_sets (std::vector < typename Toplex::Subset > * morse_sets,
                         const DirectedGraph<Toplex> & G);

// Compute SCC using boost  (actually, pathSCC =: combinatorial morse sets)
template < class Toplex >
void computeSCC 
(std::vector < typename Toplex::Subset > * morse_sets,
 std::vector < typename Toplex::Subset > * entrance_sets,
 std::vector < typename Toplex::Subset > * exit_sets,
 const typename Toplex::Subset & entrance,
 const typename Toplex::Subset & exit,
 const DirectedGraph<Toplex> & G);

// Return the length (number of edges inside the Graph) of the
// longest path connecting v to w, assuming the graph is acyclic.
// The result is -1 iff there is no such path, 0 iff v = w, etc.
template < class Toplex >
size_t computeLongestPathLength(
    DirectedGraph<Toplex> & G,
    typename DirectedGraph<Toplex>::Vertex v,
    typename DirectedGraph<Toplex>::Vertex w);

// Function to compute the longest path lengths (of a fixed graph).
// Call with 'SCC', which stores the strongly connected components,
// and 'Entrance' set and 'Exit' set.
//
// This function computes the length of the following paths:
//
//   1. The longest direct path between components.
//      The resule will be stored in 'ConnectingPathBounds', which is
//      assumed to be a vector of length n * n (n = number of SCC).
//
//   2. The longest path from 'Entrance' to components.
//      The result will be stored in 'EntrancePathBounds', a vector
//      of length n.
//
//   3. The longest path from components to 'Exit'
//      The result will be stored in 'ExitPathBounds', a vector of
//      length n.
//
//   4. The longest path
//
// Remark: the 'length' is defined to be the number of edges inside
// the graph
template < class Toplex >
void computePathBounds(std::vector<size_t> * ConnectingPathBounds,
                       std::vector<size_t> * EntrancePathBounds,
                       std::vector<size_t> * ExitPathBounds,
                       size_t * ThruPathBound,
                       const DirectedGraph<Toplex> & G,
                       const typename DirectedGraph<Toplex>::Components & SCC,
                       const typename Toplex::Subset & Entrance,
                       const typename Toplex::Subset & Exit );

// Some declarations to precede inclusion of boost graph headers

namespace DirectedGraph_detail {
  
  template < class Toplex >
  class DirectedGraph_out_edge_iterator;
  
  template < class Toplex >
  class DirectedGraph_vertex_iterator;
  
  struct graph_traversal_category;
  
} /* namespace DirectedGraph_detail */

namespace boost {
  
  template < class Toplex >
  struct graph_traits;
  
  template < class Toplex >
  std::pair< typename graph_traits< DirectedGraph<Toplex> >::adjacency_iterator, 
             typename graph_traits< DirectedGraph<Toplex> >::adjacency_iterator > 
  adjacent_vertices ( typename graph_traits< DirectedGraph<Toplex> >::vertex_descriptor v, 
                     const DirectedGraph<Toplex> & g );
  
  /* For some reason the BGL has already implemented the following and we get ambiguity.
   The problem is that these are overloads and not specialization. 
   Luckily, the defaults are precisely what we want. 
   */
  /*
   template < class Toplex >
   typename graph_traits< DirectedGraph<Toplex> >::vertex_descriptor 
   source(const typename graph_traits< DirectedGraph<Toplex> >::edge_descriptor & e, 
   const DirectedGraph<Toplex> & g);
   *//* source */
  
  /*
   template < class Toplex >
   typename graph_traits< DirectedGraph<Toplex> >::vertex_descriptor
   target(const typename graph_traits< DirectedGraph<Toplex> >::edge_descriptor & e, 
   const DirectedGraph<Toplex> & g);
   *//* target */
  
  template < class Toplex >
  std::pair<typename graph_traits< DirectedGraph<Toplex> >::out_edge_iterator, 
            typename graph_traits< DirectedGraph<Toplex> >::out_edge_iterator > 
  out_edges(const typename graph_traits< DirectedGraph<Toplex> >::vertex_descriptor & u, 
            const DirectedGraph<Toplex> & g);
  
  template < class Toplex >
  typename graph_traits< DirectedGraph<Toplex> >::degree_size_type  
  out_degree(const typename graph_traits< DirectedGraph<Toplex> >::vertex_descriptor & u, 
             const DirectedGraph<Toplex> & g);
  
  template < class Toplex >
  std::pair<typename graph_traits< DirectedGraph<Toplex> >::vertex_iterator, 
            typename graph_traits< DirectedGraph<Toplex> >::vertex_iterator> 
  vertices(const DirectedGraph<Toplex> & g);
  
  template < class Toplex >
  typename graph_traits< DirectedGraph<Toplex> >::vertices_size_type 
  num_vertices(const DirectedGraph<Toplex> & g);
  
} /* namespace boost */

#ifndef _DO_NOT_INCLUDE_HPP_
#include "data_structures/Directed_Graph.hpp"
#endif

#endif
