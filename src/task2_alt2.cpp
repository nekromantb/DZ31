#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <numeric>

class IGraph {
public:
    virtual ~IGraph() {}

    IGraph() {}; // нужно реализовать в наследниках тоже

    IGraph(const IGraph& _oth) {}; // нужно реализовать в наследниках тоже

    virtual void AddEdge(int from, int to) = 0; // Метод принимает вершины начала и конца ребра и добавляет ребро

    virtual int VerticesCount() const = 0; // Метод должен считать текущее количество вершин

    virtual std::vector<int> Vertices() const = 0; // Метод должен вернуть текущие список текущих вершин

    virtual std::vector<int> GetNextVertices(int vertex) const = 0; // Для конкретной вершины метод возращает вектор с вершинами, в которые можно дойти по ребру из данной

    virtual std::vector<int> GetPrevVertices(int vertex) const = 0; // Для конкретной вершины метод возращает вектор с вершинами, из которых можно дойти по ребру в данную
};

class ListGraph: public IGraph {
    std::unordered_map<int, std::unordered_set<int>> _nextVert;
    std::unordered_map<int, std::unordered_set<int>> _prevVert;
    std::unordered_set<int> _vert;

public:
    ListGraph() = default;
    ListGraph(const ListGraph&) = default;

    ListGraph(const IGraph& _oth): IGraph(_oth), _nextVert(), _prevVert() {
        auto vertices = _oth.Vertices();
        for (auto from: vertices) {
            _vert.insert(from);
            for (auto to: _oth.GetNextVertices(from)) {
                _nextVert[from].insert(to);
                _prevVert[to].insert(from);
            }
        }
    }

    void AddEdge(int from, int to) override {
        _vert.insert(from), _vert.insert(to);
        _nextVert[from].insert(to);
        _prevVert[to].insert(from);
    }

    int VerticesCount() const override {
        return _vert.size();
    }

    std::vector<int> Vertices() const override {
        return {_vert.begin(), _vert.end()};
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        if (auto pos = _nextVert.find(vertex); pos != _nextVert.end()) {
            return {pos->second.begin(), pos->second.end()};
        }

        return {};
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        if (auto pos = _prevVert.find(vertex); pos != _prevVert.end()) {
            return {pos->second.begin(), pos->second.end()};
        }

        return {};
    }
};

std::ostream& operator<<(std::ostream& out, const std::vector<int>& v) {
    for (auto item: v) std::cout<<item<<' ';
    return out;
}

void test(const IGraph& g) {
    std::cout<< g.GetPrevVertices(2)<<'\n';
    std::cout<< g.GetNextVertices(1)<<'\n';

    std::cout<< g.Vertices() <<'\n';

    std::cout<<g.VerticesCount()<<'\n';
    std::cout<<"End test\n";
}

int main() {
    ListGraph g1;
    g1.AddEdge(1, 2);
    g1.AddEdge(1, 4);
    g1.AddEdge(1, 2);
    g1.AddEdge(100, 100);
    g1.AddEdge(100, 100);

    IGraph& g2 = g1;

    {
        std::cout<<"Test ListGraph\n";
        test(ListGraph(g1));
        test(ListGraph(g2));
    }
}