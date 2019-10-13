#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <math.h>
#include <list>
#include <vector>

#define CN_INFINITY 1000000
#define CN_EXPONENT_K 5

struct Node
{
    int i;
    int j;
    float g;
    float h;
    float f;
    Node *parent;
    Node(int _i = -1, int _j = -1, float _g=CN_INFINITY, float _h=CN_INFINITY, Node *_parent=nullptr)
    {
        this->i = _i;
        this->j = _j;
        this->g = _g;
        this->h = _h;
        this->f = this->h + this->g;
        this->parent = _parent;
    }
};

struct Graph
{
    std::vector<std::vector<int>> matrix;
    int height;
    int width;
    Graph(int _width, int _height)
    {
        this->height = _height;
        this->width = _width;
        matrix.resize(height);
        for(int i = 0; i < width; i++)
            matrix[i].resize(width, 0);
    }
    void add_obstacle(int i, int j)
    {
        matrix[i][j] = 1;
    }
    bool is_obstacle(int i, int j)
    {
        std::cout<<i<<" "<<j<<" check node\n";
        if(matrix[i][j] == 1)
            return true;
        else
            return false;
    }

    void print_path(std::list<Node> path)
    {
        for (auto &it : path) 
		{
          matrix[it.i][it.j] = 2;
          draw(it, it.parent);
        }

        for(int i=0; i<height; i++)
        {
            for(int j=0; j<width; j++)
            {
                if(matrix[i][j] == 2)
                    std::cout<<"* ";
                else if(matrix[i][j] == 0)
                    std::cout<<"_ ";
                else if(matrix[i][j] == 1)
                    std::cout<<"& ";
                else
                    std::cout <<"1";
            }
            std::cout<<"\n";
        }
    }

    bool wall_checker(int i1, int j1, int i2, int j2)
    {
        int delta_i = abs(i1 - i2);
        int delta_j = abs(j1 - j2);
        int offset_i, offset_j, error(0);
        if (i1 < i2)
            offset_i = -1;
        else
            offset_i = 1;
        if (j1 < j2)
            offset_j = -1;
        else
            offset_j = 1;
        int i = i2, j = j2;
        if(delta_i > delta_j)
        {
            int s = delta_j;
            for(int k = 0; k <= delta_i; k++)
            {
                if(is_obstacle(i,j) == true)
                    return false;
                i += offset_i;
                error += s;
                if (2*error >= delta_i)
                {
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
                if is_obstacle(i,j) == true)
                    return false;
                j += offset_j;
                error +=s;
                if (2*error >= delta_j)
                {
                    i += offset_i;
                    error -= delta_j;
                }
            }

        }
        return true;
    }

    std::list<Node> get_neighbors(int i, int j) //модифицировать функцию для работы с 2^k соседями
    {
        std::list<Node> neighbors;
        std::list<std::pair<int, int>> offsets;
        if (CN_EXPONENT_K == 2) offsets = {{0,1}, {1,0}, {-1,0}, {0, -1}};
        if (CN_EXPONENT_K == 3) offsets = {{0,1}, {1,0}, {-1,0}, {0, -1},
                                         {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
        if (CN_EXPONENT_K == 4) offsets = {{-2, -2}, {-2, -1}, {-2, 0}, {-2, 1}, {-2, 2},
                                         {-1, -2}, {-1, -1}, {-1, 0}, {-1, 1}, {-1, 2},
                                         {0, -2}, {0, -1}, {0, 0}, {0, 1}, {0, 2},
                                         {1, -2}, {1, -1}, {1, 0}, {1, 1}, {1, 2},
                                         {2, -2}, {2, -1}, {2, 0}, {2, 1}, {2, 2}};
        if (CN_EXPONENT_K == 5) offsets = {{-3, -3}, {-3, -2}, {-3, -1}, {-3, 0}, {-3, 1}, {-3, 2}, {-3, 3},
                                         {-2, -3}, {-2, -2}, {-2, -1}, {-2, 0}, {-2, 1}, {-2, 2}, {-2, 3},
                                         {-1, -3}, {-1, -2}, {-1, -1}, {-1, 0}, {-1, 1}, {-1, 2}, {-1, 3},
                                         {0, -3}, {0, -2}, {0, -1}, {0, 0}, {0, 1}, {0, 2}, {0, 3},
                                         {1, -3}, {1, -2}, {1, -1}, {1, 0}, {1, 1}, {1, 2}, {1, 3},
                                         {2, -3}, {2, -2}, {2, -1}, {2, 0}, {2, 1}, {2, 2}, {2, 3},
                                         {3, -3}, {3, -2}, {3, -1}, {3, 0}, {3, 1}, {3, 2}, {3, 3}};
        ;
        for(std::list<std::pair<int, int>>::iterator it = offsets.begin(); it != offsets.end(); it++)
            if(i + it->first >= 0 && i + it->first < height && j + it->second >= 0 && j + it->second < width)
                if(matrix[i+it->first][j+it->second] == 0 && wall_checker(i, j, i + it->first, j + it->second)){
                    neighbors.push_back(Node(i+it->first, j+it->second));
                }
        return neighbors;
    }

    double cost(int i1, int j1, int i2, int j2)//меняем на дабл с флот
    {
        double dist = 0;
        if(CN_EXPONENT_K || CN_EXPONENT_K >= 4) dist = sqrt((i1-i2)*(i1-i2) +(j1-j2)*(j1-j2));//Евклид
        else if(CN_EXPONENT_K == 2) dist = abs(i1 - i2) + abs(j1 - j2);//Манхэттен
        else if(CN_EXPONENT_K == 3) {//Диагонали
            if (i1 == i2 || j1 == j2)
                dist = 1;
            else dist = sqrt(2);
        }

        return dist;
    }


    void draw(Node current, Node* _parent)
    {
        if (_parent == nullptr)
            return;
        Node parent = *_parent;
        int delta_i = abs(current.i - parent.i);
        int delta_j = abs(current.j - parent.j);
        int offset_i, offset_j, error(0);
        if (current.i < parent.i)
            offset_i = -1;
        else
            offset_i = 1;
        if (current.j < parent.j)
            offset_j = -1;
        else
            offset_j = 1;
        int i = parent.i, j = parent.j;
        if (delta_i > delta_j)
        {
            int s = delta_j;
            for (int k = 0; k <= delta_i; k++)
            {
                matrix[i][j] = 2;
                i += offset_i;
                error += s;
                if (2*error >= delta_i)
                {
                    j += offset_j;
                    error -= delta_i;
                }

            }
        }
        else
        {
            int s = delta_i;
            for (int k = 0; k <= delta_j; k++)
            {
                matrix[i][j] = 2;
                j += offset_j;
                error +=s;
                if (2*error >= delta_j)
                {
                    i += offset_i;
                    error -= delta_j;
                }
            }

        }
    }

};

#endif //GRAPH_H
