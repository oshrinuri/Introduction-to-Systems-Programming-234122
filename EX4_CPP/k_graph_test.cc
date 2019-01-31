#include "test_utils.h"
#include "exceptions.h"
#include "k_graph_mtm.h"

#define K_GRAPH_SIZE 5
#define DEFAULT_VAL 14.0
using namespace std;
using namespace mtm;

//------------------------------------------------------------------------------
bool TestKGraphConstructor() {
    KGraph<string, double, K_GRAPH_SIZE> k_graph(DEFAULT_VAL);
    int default_int_val = 5;
    KGraph<int *, int *, K_GRAPH_SIZE> k_graph_int_ptr(&default_int_val);
    const KGraph<float, float, K_GRAPH_SIZE> const_k_graph_float(3.14);
    return true;
}
//------------------------------------------------------------------------------
bool TestKGraphCopyConstructor() {
    const KGraph<string, double, K_GRAPH_SIZE> k_graph(DEFAULT_VAL);
    const KGraph<string, double, K_GRAPH_SIZE> copy(k_graph);
    return true;
}
//------------------------------------------------------------------------------
bool TestKGraphInsert() {
    KGraph<string, double, K_GRAPH_SIZE> k_graph(DEFAULT_VAL);
    ASSERT_NO_THROW(k_graph.Insert("Debbie", 5.0));
    ASSERT_NO_THROW(k_graph.Insert("Maggie", 4.0));
    ASSERT_NO_THROW(k_graph.Insert("Danny", 3.0));
    ASSERT_NO_THROW(k_graph.Insert("Lewis", 2.0));
    ASSERT_NO_THROW(k_graph.Insert("Moshe", 1.0));
    ASSERT_THROW(KGraphKeyAlreadyExistsExpection, k_graph.Insert("Moshe", 5.0));
    ASSERT_THROW(KGraphKeyAlreadyExistsExpection,
                 k_graph.Insert("Debbie", 13.0));
    ASSERT_NO_THROW(k_graph.Insert("Moran"));
    return true;
}
//------------------------------------------------------------------------------
bool TestKGraphRemove() {
    KGraph<string, double, K_GRAPH_SIZE> k_graph(DEFAULT_VAL);
    ASSERT_NO_THROW(k_graph.Insert("Debbie", 5.0));
    ASSERT_NO_THROW(k_graph.Insert("Maggie", 4.0));
    ASSERT_NO_THROW(k_graph.Insert("Danny", 3.0));
    ASSERT_NO_THROW(k_graph.Remove("Debbie"));
    ASSERT_NO_THROW(k_graph.Remove("Danny"));
    ASSERT_NO_THROW(k_graph.Remove("Maggie"));
    ASSERT_THROW(KGraphKeyNotFoundException, k_graph.Remove("Maggie"));
    ASSERT_THROW(KGraphKeyNotFoundException, k_graph.Remove("Danny"));
    ASSERT_THROW(KGraphKeyNotFoundException, k_graph.Remove("Debbie"));
    return true;
}
//------------------------------------------------------------------------------
bool TestKGraphSubscriptOperator() {
    KGraph<string, double, K_GRAPH_SIZE> k_graph(DEFAULT_VAL);
    ASSERT_NO_THROW(k_graph["Doron"]);
    ASSERT_THROW(KGraphKeyAlreadyExistsExpection, k_graph.Insert("Doron"));
    ASSERT_NO_THROW(k_graph.Remove("Doron"));
    ASSERT_NO_THROW(k_graph["Doron"]);
    double &doron_ref = k_graph["Doron"];
    ASSERT_EQUAL(14, doron_ref);
    return true;
}
//------------------------------------------------------------------------------
bool TestKGraphConstSubscriptOperator() {
    KGraph<string, double, K_GRAPH_SIZE> k_graph(DEFAULT_VAL);
    const string &first = "Debbie";
    const string &second = "Maggie";
    const string &third = "Danny";
    ASSERT_NO_THROW(k_graph.Insert(first, 30));
    ASSERT_NO_THROW(k_graph.Insert(second, 40));
    ASSERT_NO_THROW(k_graph.Insert(third, 50));
    const double &first_val_ref = k_graph["Debbie"];
    const double &second_val_ref = k_graph["Maggie"];
    const double &third_val_ref = k_graph["Danny"];
    ASSERT_EQUAL(30, first_val_ref);
    ASSERT_EQUAL(40, second_val_ref);
    ASSERT_EQUAL(50, third_val_ref);
    const KGraph<string, double, K_GRAPH_SIZE> const_k_graph(DEFAULT_VAL);
    ASSERT_THROW(KGraphKeyNotFoundException, const_k_graph["NoSuchPerson"]);
    return true;
}
//------------------------------------------------------------------------------
bool TestKGraphContains() {
    KGraph<string, double, K_GRAPH_SIZE> k_graph(DEFAULT_VAL);
    ASSERT_NO_THROW(k_graph.Insert("Debbie", 5.0));
    ASSERT_NO_THROW(k_graph.Insert("Maggie", 4.0));
    ASSERT_NO_THROW(k_graph.Insert("Danny", 3.0));
    ASSERT_NO_THROW(k_graph.Insert("Lewis", 2.0));
    ASSERT_NO_THROW(k_graph.Insert("Moshe", 1.0));
    ASSERT_EQUAL(true, k_graph.Contains("Debbie"));
    ASSERT_EQUAL(true, k_graph.Contains("Lewis"));
    ASSERT_EQUAL(true, k_graph.Contains("Moshe"));
    ASSERT_EQUAL(false, k_graph.Contains("NoSuchPerson"));
    ASSERT_EQUAL(false, k_graph.Contains(""));
    return true;
}
//------------------------------------------------------------------------------
bool TestKGraphConnect() {
    KGraph<string, double, K_GRAPH_SIZE> k_graph(DEFAULT_VAL);
    ASSERT_NO_THROW(k_graph.Insert("Debbie", 5.0));
    ASSERT_NO_THROW(k_graph.Insert("Maggie", 4.0));
    ASSERT_NO_THROW(k_graph.Insert("Danny", 3.0));
    ASSERT_NO_THROW(k_graph.Insert("Lewis", 2.0));
    ASSERT_NO_THROW(k_graph.Connect("Debbie", "Lewis", 0, 0));
    ASSERT_NO_THROW(k_graph.Connect("Danny", "Maggie", 2, 1));
    ASSERT_THROW(KGraphKeyNotFoundException,
                 k_graph.Connect("Debbie", "x", 0, 0));
    ASSERT_THROW(KGraphKeyNotFoundException,
                 k_graph.Connect("x", "Debbie", 0, 0));
    ASSERT_THROW(KGraphEdgeOutOfRange,
                 k_graph.Connect("Danny", "Debbie", 15, 0));
    ASSERT_THROW(KGraphEdgeOutOfRange,
                 k_graph.Connect("Danny", "Debbie", 0, -1));
    ASSERT_THROW(KGraphNodesAlreadyConnected,
                 k_graph.Connect("Debbie", "Lewis", 2, 2));
    ASSERT_THROW(KGraphEdgeAlreadyInUse,
                 k_graph.Connect("Lewis", "Maggie", 1, 1));
    ASSERT_THROW(KGraphEdgeAlreadyInUse,
                 k_graph.Connect("Debbie", "Danny", 1, 2));
    ASSERT_NO_THROW(k_graph.Connect("Lewis", 2));
    ASSERT_THROW(KGraphNodesAlreadyConnected, k_graph.Connect("Lewis", 2));
    ASSERT_THROW(KGraphEdgeOutOfRange, k_graph.Connect("Lewis", 12));
    ASSERT_THROW(KGraphEdgeOutOfRange, k_graph.Connect("Lewis", -13));
    ASSERT_THROW(KGraphKeyNotFoundException, k_graph.Connect("UnKnown", -13));
    ASSERT_THROW(KGraphEdgeAlreadyInUse, k_graph.Connect("Lewis", 0));
    return true;
}
//------------------------------------------------------------------------------
bool TestKGraphDisconnect() {
    KGraph<string, double, K_GRAPH_SIZE> k_graph(DEFAULT_VAL);
    ASSERT_NO_THROW(k_graph.Insert("Debbie", 5.0));
    ASSERT_NO_THROW(k_graph.Insert("Maggie", 4.0));
    ASSERT_NO_THROW(k_graph.Insert("Danny", 3.0));
    ASSERT_NO_THROW(k_graph.Insert("Lewis", 2.0));
    ASSERT_NO_THROW(k_graph.Connect("Debbie", "Lewis", 0, 0));
    ASSERT_NO_THROW(k_graph.Connect("Danny", "Maggie", 2, 1));
    ASSERT_NO_THROW(k_graph.Disconnect("Debbie", "Lewis"));
    ASSERT_NO_THROW(k_graph.Disconnect("Maggie", "Danny"));
    ASSERT_THROW(KGraphKeyNotFoundException, k_graph.Disconnect("X", "Y"));
    ASSERT_NO_THROW(k_graph.Connect("Debbie", "Lewis", 0, 0));
    ASSERT_NO_THROW(k_graph.Connect("Danny", "Maggie", 2, 1));
    ASSERT_THROW(kGraphNodesAreNotConnected,
                 k_graph.Disconnect("Debbie", "Maggie"));
    return true;
}
//------------------------------------------------------------------------------
int main() {
    RUN_TEST(TestKGraphConstructor);
    RUN_TEST(TestKGraphCopyConstructor);
    RUN_TEST(TestKGraphInsert);
    RUN_TEST(TestKGraphRemove);
    RUN_TEST(TestKGraphSubscriptOperator);
    RUN_TEST(TestKGraphConstSubscriptOperator);
    RUN_TEST(TestKGraphContains);
    RUN_TEST(TestKGraphConnect);
    RUN_TEST(TestKGraphDisconnect);
    return 0;
}
//------------------------------------------------------------------------------