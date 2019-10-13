#include "graph_lesha.h"
#include "pathfinder_lesha.h"
using namespace std;

int main()
{
    Node start(0,0,0,CN_INFINITY,nullptr);
    Node goal(5,5);
    Graph Map(6,6);
    Map.add_obstacle(2,2);
    std::list<Node> path = find_path(Map, start, goal);
    Map.print_path(path);
    std::cout<<"Path:\n";
    for(Node n:path)
        std::cout<<n.i<<" "<<n.j<<" "<<n.g<<"\n";
}