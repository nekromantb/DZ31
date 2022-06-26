#include "task2.h"
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_set>
#include <string>
#include <map>


class IGraph {
protected:
    std::map <std::string, int> vertices_names;
    std::map <int, std::string> vertices_names_out;
public:
    std::map <std::string, int> get_map()
    {
        return vertices_names;
    };

    std::map <int, std::string> get_map_inv()
    {
        return vertices_names_out;
    };


    bool orient_input_check()
    {
        while (1)
        {
            std::string answer;
            std::cout << "Is edge oriented? (y/n): ";
            std::cin >> answer;
            if (answer != "y" && answer != "n")
            {
                std::cout << "Wrong input!" << std::endl;
                continue;
            }
            else
            {
                return answer == "y";
            }
        }
    };

    void Add_Graph_from_input(IGraph* graph, int& counter)
    {
        std::string from, to;
        int r;
        std::cout << "Graph edges number: ";
        std::cin >> r;
        std::cout << "Edges input:" <<std::endl;
        for (int i = 0; i <r; ++i)
        {
            std::cout << "Input edge vertices (from to): ";
            std::cin >> from >> to;
            if (orient_input_check()) {
                graph->AddEdge(from, to);
            }
            else {
                graph->AddEdge(from, to);
                graph->AddEdge(to, from);
            }
        }
        std::cout << "Graph was added."<< std::endl;
    };

    virtual ~IGraph() {};
    IGraph() {};
    IGraph(IGraph *oth) {};
    virtual void GraphOutput() = 0;// Метод предназначен для вывода пользователю графа в том его виде, в котором он хранится
    virtual void AddEdge(std::string from, std::string to) = 0; // Метод принимает вершины начала и конца ребра и добавляет ребро
    virtual int VerticesCount() const = 0; // Метод должен считать текущее количество вершин
    virtual void GetNextVertices(std::string vertex, std::vector<std::string> &vertices) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной
    virtual void GetPrevVertices(std::string vertex, std::vector<std::string> &vertices) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, из которых можно дойти по ребру в данную
};

class MatrixGraph;

class ListGraph : public IGraph
{
    std::vector<std::unordered_set<std::string>> graph;
public:

    std::vector<std::unordered_set<std::string>> get_graph()
    {
        return graph;
    }
    void AddEdge(std::string from, std::string to) override
    {
        auto it = vertices_names.find(from);
        if (it == vertices_names.end())
        {
            vertices_names.insert(std::make_pair(from,vertices_names.size()));
            vertices_names_out.insert(std::make_pair(vertices_names_out.size(), from));
            it = vertices_names.find(from);
            graph.resize(vertices_names.size());
        }
        graph[it->second].insert(to);
        auto it1 = vertices_names.find(to);
        if (it1 == vertices_names.end())
        {
            vertices_names.insert(std::make_pair(to,vertices_names.size()));
            vertices_names_out.insert(std::make_pair(vertices_names_out.size(), to));
            graph.resize(vertices_names.size());
        }
    };

    int VerticesCount() const override
    {
        return vertices_names.size();
    };

    void GetPrevVertices(std::string vertex, std::vector<std::string>& vertices) const override
    {
        vertices.clear();
        auto it = vertices_names.find(vertex);
        if (it == vertices_names.end())
        {
            std::cerr << "No such vertex in graph!" << std::endl;
        }
        else
        {
            for (int i = 0; i < vertices_names.size();++i)
            {
                if (it->second != i)
                {
                    if(graph[i].contains(vertex))
                    {
                        vertices.push_back(it->first);
                    }
                }
            }
        }
    };

    void GetNextVertices(std::string vertex, std::vector<std::string>& vertices) const override
    {
        vertices.clear();
        auto it = vertices_names.find(vertex);
        if (it == vertices_names.end())
        {
            std::cerr << "No such vertex in graph!" << std::endl;
        }
        else
        {
            int j = 0;
            for (auto i : graph[it->second])
            {
                vertices[j] = i;
                ++j;
            }
        }
    };

    void GraphOutput() override
    {
        for (auto i : vertices_names)
        {
            std::cout << i.first << " | ";
            for (auto j : graph[i.second])
                std::cout << j << " ";
            std::cout << std::endl;
        }
    }

    ListGraph() {
        int counter = 0;
        std::cout << "Graph input in graph adjacency list format initiated." << std::endl;
        Add_Graph_from_input(this, counter);
    };

    ListGraph(ListGraph *oth) {
        if (!graph.empty()){
            graph.clear();
            vertices_names.clear();
            vertices_names_out.clear();
        }
        vertices_names = oth->get_map();
        vertices_names_out = oth->get_map_inv();
        graph = oth->get_graph();
    };

    ListGraph& operator= (const ListGraph& oth) {
        if (this == &oth)
            return *this;

        if (!graph.empty()){
            graph.clear();
            vertices_names.clear();
            vertices_names_out.clear();
        }
        vertices_names = oth.vertices_names;
        vertices_names_out = oth.vertices_names_out;
        graph = oth.graph;
        return *this;
    };


    ListGraph(MatrixGraph &oth);
    ListGraph& operator= (MatrixGraph& oth);
};

class MatrixGraph : public IGraph
{
    std::vector<std::vector<int>> graph;
public:
    std::vector<std::vector<int>> get_graph()
    {
        return graph;
    }
    void AddEdge(std::string from, std::string to) override
    {
        auto it = vertices_names.find(from);
        if (it == vertices_names.end())
        {
            vertices_names.insert(std::make_pair(from,vertices_names.size()));
            vertices_names_out.insert(std::make_pair(vertices_names_out.size(), from));
            it = vertices_names.find(from);
            graph.resize(vertices_names.size());
            for (int i = 0; i < vertices_names.size(); ++i)
            {
                graph[i].resize(vertices_names.size());
            }
        }
        auto it1 = vertices_names.find(to);
        if (it1 == vertices_names.end())
        {
            vertices_names.insert(std::make_pair(to,vertices_names.size()));
            vertices_names_out.insert(std::make_pair(vertices_names_out.size(), to));
            it1 = vertices_names.find(to);
            graph.resize(vertices_names.size());
            for (int i = 0; i < vertices_names.size(); ++i)
            {
                graph[i].resize(vertices_names.size());
            }
        }
        graph[it1->second][it->second] = 1;
    };

    int VerticesCount() const override
    {
        return vertices_names.size();
    };

    void GetPrevVertices(std::string vertex, std::vector<std::string>& vertices) const override
    {
        vertices.clear();
        auto it = vertices_names.find(vertex);
        if (it == vertices_names.end())
        {
            std::cerr << "No such vertex in graph!" << std::endl;
        }
        else
        {
            for (int i = 0; i < vertices_names.size();++i)
            {
                if (graph[i][it->second] == 1)
                {
                    auto it_out = vertices_names_out.find(i);
                    vertices.push_back(it_out->second);
                }
            }
        }
    };

    void GetNextVertices(std::string vertex, std::vector<std::string>& vertices) const override
    {
        vertices.clear();
        auto it = vertices_names.find(vertex);
        if (it == vertices_names.end())
        {
            std::cerr << "No such vertex in graph!" << std::endl;
        }
        else
        {
            for (int i = 0; i < vertices_names.size();++i)
            {
                if (graph[it->second][i] == 1)
                {
                    auto it_out = vertices_names_out.find(i);
                    vertices.push_back(it_out->second);
                }
            }
        }
    };

    void GraphOutput() override
    {
        std::cout << "       ";
        for (int i = 0; i < vertices_names_out.size();++i) {
            auto it_out1 = vertices_names_out.find(i);
            std::cout << it_out1->second << " | ";
        }
        std::cout << std::endl;
        for (int i = 0; i < vertices_names_out.size();++i)
        {
            auto it_out2 = vertices_names_out.find(i);
            std::cout << it_out2->second << "  ->  ";
            for (int j = 0; j < vertices_names.size();++j)
            {
                std::cout << graph[j][i] << " | ";
            }
            std::cout << std::endl;
        }
    }

    MatrixGraph() {
        int counter = 0;
        std::cout << "Graph input in graph adjacency matrix format initiated." << std::endl;
        Add_Graph_from_input(this, counter);
    };

    MatrixGraph(MatrixGraph *oth) {
        if (!graph.empty()){
            graph.clear();
            vertices_names.clear();
            vertices_names_out.clear();
        }
        vertices_names = oth->get_map();
        vertices_names_out = oth->get_map_inv();
        graph = oth->get_graph();
    };

    MatrixGraph(ListGraph *oth) {
        if (!graph.empty()) {
            graph.clear();
            vertices_names.clear();
            vertices_names_out.clear();
        }
        auto vertices_names1 = oth.get_map();
        std::vector<std::unordered_set<std::string>> input_graph = oth.get_graph();
        for (auto i : vertices_names1)
            for (auto j : input_graph[i.second]){
                AddEdge(i.first, j);
            }

    };

    MatrixGraph& operator= (const MatrixGraph& oth) {
        if (!graph.empty()){
            graph.clear();
            vertices_names.clear();
            vertices_names_out.clear();
        }
        vertices_names = oth.vertices_names;
        vertices_names_out = oth.vertices_names_out;
        graph = oth.graph;
        return *this;
    };

    MatrixGraph& operator= (ListGraph& oth) {

        if (!graph.empty()){
            graph.clear();
            vertices_names.clear();
            vertices_names_out.clear();
        }
        auto vertices_names1 = oth.get_map();
        std::vector<std::unordered_set<std::string>> input_graph = oth.get_graph();
        for (auto i : vertices_names1)
            for (auto j : input_graph[i.second]){
                AddEdge(i.first, j);
            }
    };
};

ListGraph::ListGraph(MatrixGraph &oth)
{
    if (!graph.empty()) {
        graph.clear();
        vertices_names.clear();
        vertices_names_out.clear();
        }
    auto vertices_names1 = oth.get_map();
    std::vector<std::vector<int>> input_graph = oth.get_graph();
    for (auto i : vertices_names1)
        for (auto j : vertices_names1)
            if (input_graph[j.second][i.second] == 1) {
                AddEdge(i.first, j.first);
            }
};

ListGraph& ListGraph::operator=(MatrixGraph &oth) {
    if (!graph.empty()){
        graph.clear();
        vertices_names.clear();
        vertices_names_out.clear();
    }
    auto vertices_names1 = oth.get_map();
    std::vector<std::vector<int>> input_graph = oth.get_graph();
    for (auto i : vertices_names1)
        for (auto j : vertices_names1)
            if (input_graph[j.second][i.second] == 1) {
                AddEdge(i.first, j.first);
            }
};

void task2() {
    MatrixGraph g1;
    ListGraph g2 = g1;
    g1.GraphOutput();
    g2.GraphOutput();
}