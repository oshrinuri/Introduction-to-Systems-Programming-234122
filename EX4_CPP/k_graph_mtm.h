#ifndef K_GRAPH_MTM_H
#define K_GRAPH_MTM_H

#include <vector>
#include <map>
#include "exceptions.h"

#define USED 0

namespace mtm {
// Requirements: KeyType::opertor<,
//               KeyType::operator==,
//               KeyType and ValueType copy c'tor
    template<typename KeyType, typename ValueType, int k> class KGraph {

    protected:

        // A node. Represents the basic data unit in a kGraph. Has a key, a value, and
        // connected to at most k other nodes through k edges numbered from 0 to k-1.
        class Node {
        private:

            KeyType key;
            ValueType value;
            std::vector<Node*> arcs;

        public:
            // Constructs a new node with the given key and value.
            //
            // @param key key of the new node.
            // @param value value of the new node.
            Node(KeyType const &key, ValueType const &value):
                    key(key), value(value), arcs(k) {};

            // A destructor.
            ~Node() = default;

            // Returns the key of the node.
            //
            // @return the key of the node.
            KeyType const& Key() const {
                return this->key;
            }

            // Returns the value of the node.
            //
            // @return the value of the node.
            ValueType& Value() {
                return this->value;
            }
            ValueType const& Value() const {
                return this->value;
            }

            // Returns a reference to the pointer to the neighbor node connected through
            // edge i.
            //
            // @return (reference to) a pointer to the node connected through edge i.
            Node*& operator[](int i) {
                return arcs[i];
            }
            const Node* const operator[](int i) const {
                return arcs[i];
            }
        }; // End of Node Class

    public:
        class const_iterator;  // forward declaration

        // An iterator. Used to iterate over the data in a kGraph. At every given
        // moment, the iterator points either to one of the nodes in the graph, or to
        // the end of the graph.
        class iterator {

        friend class const_iterator;

        private:

            Node* current_node_ptr;
            KGraph* current_kgraph;

        public:
            // Constructs a new iterator that points to a given node in the given graph.
            //
            // @param node the node the new iterator points to.
            // @param graph the kGraph over which the iterator iterates.
            iterator(Node* node, KGraph* graph) {
                Node* node_ptr = graph->graph_map[node->Key()];
                current_node_ptr = node_ptr;
                current_kgraph = graph;
            }

            // A copy constructor.
            //
            // @param it the iterator to copy.
            iterator(const iterator& it) {
                this->current_node_ptr = it.current_node_ptr;
            }

            // A destructor.
            ~iterator() = default;

            // Moves the iterator to point to the node that is connected to the current
            // node through edge i.
            //
            // @param i the edge over which to move.
            // @return a reference to *this (the same iterator) after moving it.
            // @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1]
            // @throw KGraphIteratorReachedEnd when trying to move an iterator that
            //        points to the end of the graph.
            iterator& Move(int i) {
                if (i < 0 || i > k) throw KGraphEdgeOutOfRange();
                if ((*current_node_ptr)[i] == nullptr) {
                    throw KGraphIteratorReachedEnd();
                }
                current_node_ptr = (*current_node_ptr)[i];
                return *this;
            }

            // Dereferne operator. Return the key of the node pointed by the iterator.
            //
            // @return the key of the node to which the iterator points.
            // @throw KGraphIteratorReachedEnd when trying to dereference an iterator
            //        that points to the end of the graph.
            KeyType const& operator*() const {
                if (current_node_ptr == nullptr) {
                    throw KGraphIteratorReachedEnd();
                }
                return current_node_ptr->Key();
            }

            // Equal operator. Two iterators are equal iff they either point to the same
            // node in the same graph, or to the end of the same graph.
            //
            // @param rhs righ hand side operand.
            // @return true iff the iterators are equal.
            bool operator==(const iterator& rhs) const {
                if (this->current_node_ptr == nullptr &&
                    rhs.current_node_ptr == nullptr &&
                    this->current_kgraph == rhs.current_kgraph) {
                    return true;
                }
                return this->current_node_ptr == rhs.current_node_ptr;
            }

            // Not equal operator (see definition of equality above).
            //
            // @param rhs righ hand side operand.
            // @return true iff the iterators are not equal.
            bool operator!=(const iterator& rhs) const {
                return !(*this == rhs);
            }

            // Equal operator for a const iterator as the right-hand side operand.
            //
            // @param rhs righ hand side operand.
            // @return true iff the iterators are equal.
            bool operator==(const const_iterator& rhs) const {
                return *this == rhs;
            }

            // Not equal operator for a const iterator as the right-hand side operand.
            //
            // @param rhs righ hand side operand.
            // @return true iff the iterators are not equal.
            bool operator!=(const const_iterator& rhs) const {
                return !(*this == rhs);
            }
        };

        // A const iterator. Used to iterate over the data in a constant kGraph.
        // Similarly to a regular iterator, at every given moment, the iterator points
        // either to one of the nodes in the graph, or to the end of the graph.
        class const_iterator {

        private:

            const Node* current_node_ptr;
            const KGraph* current_kgraph;

        public:
            // Constructs a new const iterator that points to a given node in the given
            // graph.
            //
            // @param node the node the new iterator points to.
            // @param graph the kGraph over which the iterator iterates.
            const_iterator(const Node* node, const KGraph* graph) {
                const Node* node_ptr = graph->graph_map[node->Key()];
                current_node_ptr = node_ptr;
                current_kgraph = graph;
            }
            // A copy constructor.
            //
            // @param it the iterator to copy.
            const_iterator(const const_iterator& it) = default;

            // Conversion from a regular iterator. Constructs a new const iterator that
            // points to the same node as the given iterator.
            //
            // @param it the iterator we would like to convert to const iterator.
            const_iterator(const iterator& it) {
                current_node_ptr = it.current_node_ptr;
                current_kgraph = it.current_kgraph;
            }

            // A destructor.
            ~const_iterator() = default;

            // Moves the iterator to point to the node that is connected to the current
            // node through edge i.
            //
            // @param i the edge over which to move.
            // @return a reference to *this (the same iterator) after moving it.
            // @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1]
            // @throw KGraphIteratorReachedEnd when trying to move an iterator that
            //        points to the end of the graph.
            const_iterator& Move(int i) {
                if (i < 0 || i > k) throw KGraphEdgeOutOfRange();
                if (current_node_ptr->operator[](i) == nullptr) {
                    throw KGraphIteratorReachedEnd();
                }
                current_node_ptr = current_node_ptr->operator[](i);
                return *this;
            }

            // Dereferne operator. Return the key of the node pointed by the iterator.
            //
            // @return the key of the node to which the iterator points.
            // @throw KGraphIteratorReachedEnd when trying to dereference an iterator
            //        that points to the end of the graph.
            KeyType const& operator*() const {
                return this->current_node_ptr->Key();
            }

            // Equal operator. Two iterators are equal iff they either point to the same
            // node in the same graph, or to the end of the same graph.
            //
            // @param rhs righ hand side operand.
            // @return true iff the iterators are equal.
            bool operator==(const const_iterator& rhs) const {
                if (this->current_node_ptr == nullptr &&
                    rhs.current_node_ptr == nullptr) {
                    return true;
                }
                return this->current_node_ptr == rhs.current_node_ptr;
            }

            // Not equal operator (see definition of equality above).
            //
            // @param rhs righ hand side operand.
            // @return true iff the iterators are not equal.
            bool operator!=(const const_iterator& rhs) const {
                return !(*this == rhs);
            }
        };

    public:
        typedef typename std::map<KeyType,Node*>::const_iterator Map_Iterator;
        typedef typename std::map<KeyType,Node*>::const_iterator Const_Map_Iterator;
        // Constructs a new empty kGraph with the given default value.
        //
        // @param default_value the default value in the graph.
        explicit KGraph(ValueType const& default_value):
                default_value(default_value) {};

        // A copy constructor. Copies the given graph. The constructed graph will have
        // the exact same structure with copied data.
        //
        // @param k_graph the graph to copy.
        KGraph(const KGraph& k_graph) = default;

        // A destructor. Destroys the graph together with all resources allocated.
        ~KGraph() = default;

        // Returns an iterator to the node with the given key.
        //
        // @param i the key of the node which the returned iterator points to.
        // @return iterator the newly constructed iterator.
        // @throw KGraphKeyNotFoundException when the given key is not found in the
        //        graph.
        iterator BeginAt(KeyType const& i) {
            Const_Map_Iterator key_itr;
            key_itr = graph_map.find(i);
            if (graph_map.find(i) == graph_map.end()) {
                throw KGraphKeyNotFoundException();
            }
            iterator it(key_itr->second,this);
            return it;
        }

        const_iterator BeginAt(KeyType const& i) const {
            Const_Map_Iterator key_itr;
            key_itr = graph_map.find(i);
            if (graph_map.find(i) == graph_map.end()) {
                throw KGraphKeyNotFoundException();
            }
            const_iterator it(key_itr->second,this);
            return it;
        }

        // Returns an iterator to the end of the graph.
        //
        // @return iterator an iterator to the end of the graph.
        const_iterator End() const {
            return nullptr;
        }

        // Inserts a new node with the given data to the graph.
        //
        // @param key the key to be assigned to the new node.
        // @param value the value to be assigned to the new node.
        // @throw KGraphKeyAlreadyExistsExpection when trying to insert a node with a
        //        key that already exists in the graph.
        void Insert(KeyType const& key, ValueType const& value) {
            if (graph_map.find(key) != graph_map.end()) {
                throw KGraphKeyAlreadyExistsExpection();
            }
            Node* new_node = new Node(key, value);
            graph_map.insert(std::pair<KeyType, Node*>(key, new_node));
        }

        // Inserts a new node with the given key and the default value to the graph.
        //
        // @param key the key to be assigned to the new node.
        // @throw KGraphKeyAlreadyExistsExpection when trying to insert a node with a
        //        key that already exists in the graph.
        void Insert(KeyType const& key) {
            Insert(key,this->default_value);
        }

        // Removes the node with the given key from the graph.
        //
        // @param key the key of the node to be removed.
        // @throw KGraphKeyNotFoundException when trying to remove a key that cannot
        //        be found in the graph.
        void Remove(KeyType const& key) {
            if (graph_map.find(key) == graph_map.end()) {
                throw KGraphKeyNotFoundException();
            }
            graph_map.erase(key);
        }

        // Removes the node pointed by the given iterator from the graph. If the
        // given iterator neither points to a node in this graph nor to the end of
        // this graph, the behaviour of this function is undefined.
        //
        // @param it the iterator that points to the node to be removed.
        // @throw KGraphIteratorReachedEnd when the given iterator points to the end
        //        of the graph.
        void Remove(const iterator& it) {
            KeyType key_to_remove = it.operator*();
            Remove(key_to_remove);
        }

        // The subscript operator. Returns a reference to the value assigned to
        // the given key in the graph. If the key does not exist, inserts a new node
        // to the graph with the given key and the default value, then returns a
        // refernce to its value.
        //
        // @param key the key to return its value.
        // @return the value assigned to the given key.
        ValueType& operator[](KeyType const& key) {
            Const_Map_Iterator key_itr = graph_map.find(key);
            if (key_itr != graph_map.end()) { // Key was found
                return key_itr->second->Value();
            } else {
                Insert(key);
                return operator[](key);
            }
        }

        // A const version of the subscript operator. Returns the value assigned to
        // the given key in the graph. If the key does not exist, throws an exception.
        //
        // @param key the key to return its value.
        // @return the value assigned to the given key.
        // @throw KGraphKeyNotFoundException if the given key cannot be found in the
        //        graph.
        ValueType const& operator[](KeyType const& key) const {
            Const_Map_Iterator key_itr = graph_map.find(key);
            if (key_itr == graph_map.end()) { // Key wasn't found
                throw KGraphKeyNotFoundException();
            }
            return key_itr->second->Value();
        }

        // Checks whether the graph contains the given key.
        //
        // @param key
        // @return true iff the graph contains the given key.
        bool Contains(KeyType const& key) const {
            return graph_map.count(key) == 1;
        }

        // Connects two nodes in the graph with an edge.
        //
        // @param key_u the key of the first node.
        // @param key_v the key of the second node.
        // @param i_u the index of the new edge at the first node.
        // @param i_v the index of the new edge at the second node.
        // @throw KGraphKeyNotFoundException if at least one of the given keys cannot
        //        be found in the graph.
        // @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1].
        // @throw KGraphNodesAlreadyConnected if the two nodes are already connected.
        // @throw KGraphEdgeAlreadyInUse if at least one of the indices of the edge at
        //        one of the nodes is already in use.
        void Connect(KeyType const& key_u, KeyType const& key_v, int i_u, int i_v) {
            if (!Contains(key_u) || !Contains(key_v)) {
                throw KGraphKeyNotFoundException();
            }
            if (i_u < 0 || i_u > k || i_v < 0 || i_v > k) {
                throw KGraphEdgeOutOfRange();
            }
            Const_Map_Iterator u_itr = graph_map.find(key_u);
            Const_Map_Iterator v_itr = graph_map.find(key_v);
            if (areConnectedNodes(u_itr->second,v_itr->second)) {
                throw KGraphNodesAlreadyConnected();
            }
            if ((*u_itr->second)[i_u] != nullptr ||
                (*v_itr->second)[i_v] != nullptr) {
                throw KGraphEdgeAlreadyInUse();
            }
            (*u_itr->second)[i_u] = v_itr->second;
            (*v_itr->second)[i_v] = u_itr->second;
        }

        // Connects a node to itself via a self loop.
        //
        // @param key the key of the node.
        // @param i the index of the self loop.
        // @throw KGraphKeyNotFoundException if the given keys cannot be found in the
        //        graph.
        // @throw KGraphEdgeOutOfRange if i is not in the range [0,k-1]
        // @throw KGraphNodesAlreadyConnected if the node is already self connected.
        // @throw KGraphEdgeAlreadyInUse if the index of the self loop is already in
        //        use.
        void Connect(KeyType const& key, int i) {
            if (!Contains(key)) {
                throw KGraphKeyNotFoundException();
            }
            if (i < 0 || i > k) {
                throw KGraphEdgeOutOfRange();
            }
            Const_Map_Iterator itr = graph_map.find(key);
            if ((*itr->second)[i] == itr->second) {
                throw KGraphNodesAlreadyConnected();
            }
            if ((*itr->second)[i] != USED) {
                throw KGraphEdgeAlreadyInUse();
            }
            (*itr->second)[i] = itr->second;
        }

        // Disconnects two connected nodes.
        //
        // @param key_u the key of the first node.
        // @param key_v the key of the second node.
        // @throw KGraphKeyNotFoundException if at least one of the given keys cannot
        //        be found in the graph.
        // @throw kGraphNodesAreNotConnected if the two nodes are not connected.
        void Disconnect(KeyType const& key_u, KeyType const& key_v) {
            if (!Contains(key_u) || !Contains(key_v)) {
                throw KGraphKeyNotFoundException();
            }
            Const_Map_Iterator u_itr = graph_map.find(key_u);
            Const_Map_Iterator v_itr = graph_map.find(key_v);
            if (!areConnectedNodes(u_itr->second,v_itr->second)) {
                throw kGraphNodesAreNotConnected();
            }
            for (int i=0;i<k;++i) {
                if ((*u_itr->second)[i] == v_itr->second) {
                    (*u_itr->second)[i] = nullptr;
                }
                if ((*v_itr->second)[i] == u_itr->second) {
                    (*v_itr->second)[i] = nullptr;
                }
            }
        }

    private:
        ValueType default_value;
        std::map<KeyType,Node*> graph_map;

        bool areConnectedNodes(Node* nodeA,Node* nodeB) {
            Const_Map_Iterator map_iterator = graph_map.find(nodeA->Key());
            for (int i=0 ; i<k ; ++i) {
                if ((*map_iterator->second)[i] == nodeB) {
                    return true;
                }
            }
            return false;
        }
    };

}  // namespace mtm

#endif  // K_GRAPH_MTM_H