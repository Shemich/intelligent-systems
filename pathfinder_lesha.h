#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <map>
#include "graph.h"
#include <iostream>

#define CN_USE_THETA 1 //Используем Theta*
#define CN_EXPONENT_K 5 //Выбор K связности
#define CN_D 2 //Взвешивание евристики

void add_to_Open(std::list<Node> &Open, Node current)
{
    Open.push_back(current);
	//return;
}

Node get_min(std::list<Node> &Open)
{
    Node min;
    std::list<Node>::iterator min_it;// = nullptr;
    for(std::list<Node>::iterator it = Open.begin(); it!= Open.end(); it++)
        if(min.f > it->f)
        {
            min = *it;
            min_it = it;
        }
    Open.erase(min_it);
    return min;
}

float count_h(Node current, Node goal) //(!)
{
    if (CN_USE_THETA || CN_EXPONENT_K > 3) return CN_D * sqrt(pow(current.i - goal.i, 2) + pow(current.j - goal.j, 2)); //расстояние Эвклид
    if (CN_EXPONENT_K == 2) return CN_D * abs(current.i - goal.i) + abs(current.j - goal.j); //расстояние Манхэттен
    if (CN_EXPONENT_K == 3) {
        return CN_D * (abs(current.i - goal.i) + abs(current.j - goal.j) + sqrt(2) * std::min((current.i - goal.i),(current.j - goal.j)));//расстояние диагонали
    }
    return 0;
}




std::list<Node> reconstruct_path(Node current, Graph& Map) //модифицировать восстановление пути на случай использования Theta*/k > 3
{
    std::list<Node> path;
    while(current.parent != nullptr)
    {
        path.push_front(current);
        Map.draw(current, current.parent);
        current = *current.parent;
    }
    path.push_front(current);
    return path;
}

bool line_of_sight(Graph &Map, Node current, Node* parent_)
{
    if(parent_ == nullptr)
        return false;
    Node parent = *parent_;
    int delta_i = abs(current.i - parent.i);
    int delta_j = abs(current.j - parent.j);
    int offset_i, offset_j, error(0);
    if(current.i < parent.i)
        offset_i = -1;
    else
        offset_i = 1;
    if(current.j < parent.j)
        offset_j = -1;
    else
        offset_j = 1;
    int i = parent.i, j = parent.j;
    if(delta_i > delta_j)
    {
        int s = delta_j;
        for(int k = 0; k <= delta_i; k++)
        {
            if(Map.is_obstacle(i,j) == true)
                return false;
            i += offset_i;
            error += s;
            if(2*error >= delta_i)
            {
				/*if(2*error - s != delta_i)
				{
					std::cout<<error<<" additional check ";
					if(Map.is_obstacle(i, j))
						return false;
				}*/
                j += offset_j;
                error -= delta_i;
            }
        }
    }
    else
    {
        int s = delta_i;
        for(int k = 0; k <= delta_j; k++)
        {
            if(Map.is_obstacle(i,j) == true)
                return false;
            j += offset_j;
            error +=s;
            if(2*error >= delta_j)
            {
				/*if(2*error - s != delta_i)
				{
					std::cout<<error<<" additional check ";
					if(Map.is_obstacle(i, j))
						return false;
				}*/
                i += offset_i;
                error -= delta_j;
            }
        }

    }
    return true;
}

std::list<Node> find_path(Graph Map, Node start, Node goal)
{
    std::list<Node> Open;
    std::map<std::pair<int,int> , Node> Closed;
    Open.push_back(start);
    while(!Open.empty())
    {
        Node current = get_min(Open);
        std::cout<<Open.size()<<" OPEN size\n";
        if(Closed.find(std::make_pair(current.i, current.j))!=Closed.end())
        {
            std::cout<<"Node "<<current.i<<" "<<current.j<<" is already in Closed\n";
            continue;
        }
        std::cout<<"current node: "<<current.i<<" "<<current.j<<" "<<current.g<<"\n";
        Closed.insert({std::make_pair(current.i,current.j), current});
        if(current.i == goal.i && current.j == goal.j)
        {
            std::list<Node> path = reconstruct_path(current, Map);
            return path;
        }
        std::list<Node> neighbors = Map.get_neighbors(current.i, current.j);
        for(Node n:neighbors)
        {
            if(Closed.find({n.i, n.j}) != Closed.end())
            {
                std::cout<<"Node "<<current.i<<" "<<current.j<<" is already in Closed\n";
                continue;
            }
            std::cout<<"Neighbor "<<n.i<<" "<<n.j<<"\n";
            n.g = current.g + Map.cost(current.i, current.j, n.i, n.j);
            n.h = count_h(n, goal);//считаем эвристику для A*
            n.f = n.g + n.h;//f-значение нужно для A*
            n.parent = &Closed.find({current.i, current.j})->second;
            if(CN_USE_THETA)
                if(line_of_sight(Map, n, current.parent))
                {
                    n.g = current.parent->g + Map.cost(current.parent->i, current.parent->j, n.i, n.j);
                    n.f = n.g + n.h;
                    n.parent = current.parent;
                }
            add_to_Open(Open, n);
        }
    }
    return std::list<Node> {};
}

#endif //PATHFINDER_H
