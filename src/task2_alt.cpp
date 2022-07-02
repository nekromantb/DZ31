#include <iostream>
#include <vector>
#include <map>
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

    virtual std::vector<int> GetNextVertices(int vertex) const = 0; // Для конкретной вершины метод возвращает вектор с вершинами, в которые можно дойти по ребру из данной

    virtual std::vector<int> GetPrevVertices(int vertex) const = 0; // Для конкретной вершины метод возвращает вектор с вершинами, из которых можно дойти по ребру в данную
};

class ListGraph: public IGraph {
    std::map<int, std::vector<int>> _nextVert;
    std::map<int, std::vector<int>> _prevVert;

public:
    ListGraph() = default;
    ListGraph(const ListGraph&) = default;

    ListGraph(const IGraph& _oth): IGraph(_oth), _nextVert(), _prevVert(){
        for (auto from: _oth.Vertices()) {
            for (auto to: _oth.GetNextVertices(from)) {
                _nextVert[from].push_back(to);
                _prevVert[to].push_back(from);
            }
        }
    }

    void AddEdge(int from, int to) override {
        auto& from_vertices = _nextVert[from];

        if (auto pos = std::find(from_vertices.begin(), from_vertices.end(), to); pos == from_vertices.end()) {
            from_vertices.push_back(to);
            _prevVert[to].push_back(from);
        }
    }

    int VerticesCount() const override {
        return std::accumulate(_nextVert.begin(), _nextVert.end(), _prevVert.size(),
                               [this](auto res, const auto& pair){return res + (_prevVert.find(pair.first) == _prevVert.end());});
    }

    std::vector<int> Vertices() const override {
        static auto get_first = [](const auto &pair){return pair.first;};

        static auto keys = [](const auto& map){
            std::vector<int> keys(map.size());
            std::transform(map.begin(), map.end(), keys.begin(), get_first);

            return keys;
        };

        auto nextV = keys(_nextVert);
        auto prevV = keys(_prevVert);
        std::vector<int> res(prevV.size() + nextV.size());
        std::merge(prevV.begin(), prevV.end(), nextV.begin(), nextV.end(), res.begin());
        auto last = std::unique(res.begin(), res.end());
        res.erase(last, res.end());

        return res;
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        std::vector<int> vertices;
        if (auto pos = _nextVert.find(vertex); pos != _nextVert.end()) {
            vertices = pos->second;
        }

        return vertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        std::vector<int> vertices;
        if (auto pos = _prevVert.find(vertex); pos != _prevVert.end()) {
            vertices = pos->second;
        }

        return vertices;
    }
};


class MatrixGraph: public IGraph {
    std::vector<int> _index2Vertex;
    std::map<int, int> _vertex2Index;
    std::vector<std::vector<int>> _matrix;

    int _getIndex(int vertex) {
        if (auto pos = _vertex2Index.find(vertex); pos != _vertex2Index.end()) {
            return pos->second;
        }

        const int res = _index2Vertex.size();

        _index2Vertex.push_back(vertex);
        _vertex2Index[vertex] = res;

        for (auto& row: _matrix) row.push_back(0);
        _matrix.emplace_back(res + 1, 0);

        return res;
    }

public:
    MatrixGraph() = default;
    MatrixGraph(const MatrixGraph&) = default;

    MatrixGraph(const IGraph& _oth):  _index2Vertex(_oth.Vertices()), _vertex2Index(), _matrix(_oth.VerticesCount(), std::vector<int>(_oth.VerticesCount(), 0)) {
        for (int i = 0; i < _index2Vertex.size(); ++i) {
            _vertex2Index[_index2Vertex[i]] = i;
        }

        for (auto from_vertex: _index2Vertex) {
            for (auto to_vertex: _oth.GetNextVertices(from_vertex)) {
                _matrix[_vertex2Index[from_vertex]][_vertex2Index[to_vertex]] = 1;
            }
        }
    }

    void AddEdge(int from, int to) override {
        auto from_idx = _getIndex(from), to_idx = _getIndex(to);
        _matrix[from_idx][to_idx] = 1;
    }

    int VerticesCount() const override {
        return _index2Vertex.size();
    }

    std::vector<int> Vertices() const override {
        return _index2Vertex;
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        std::vector<int> vertices;

        if (auto pos = _vertex2Index.find(vertex); pos != _vertex2Index.end()) {
            const auto& next_idxs = _matrix[pos->second];

            for (int i = 0; i < next_idxs.size(); ++i) {
                if (next_idxs[i]) {
                    vertices.push_back(_index2Vertex[i]);
                }
            }
        }

        return vertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        std::vector<int> vertices;

        if (auto pos = _vertex2Index.find(vertex); pos != _vertex2Index.end()) {
            for (int i = 0; i < _index2Vertex.size(); ++i) {
                if (_matrix[i][pos->second]) {
                    vertices.push_back(_index2Vertex[i]);
                }
            }
        }

        return vertices;
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

int task2_alt() {
    MatrixGraph g1;
    g1.AddEdge(1, 2);
    g1.AddEdge(1, 4);
    g1.AddEdge(1, 2);
    g1.AddEdge(100, 100);
    g1.AddEdge(100, 100);
    g1.AddEdge(-1, 1);

    ListGraph g2 = g1;

    {
        std::cout<<"Test MatrixGraph\n";
        test(MatrixGraph(g1));
        test(MatrixGraph(g2));
    }
    {
        std::cout<<"Test ListGraph\n";
        test(ListGraph(g1));
        test(ListGraph(g2));
    }
}