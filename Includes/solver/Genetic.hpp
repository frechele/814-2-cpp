#pragma once

#include <vector>

#include <game/Board.hpp>

namespace Solver
{
struct Gene final
{
    BOJ::Board::Matrix board;
    int score{ -1 };
};

namespace GeneUtils
{
Gene Crossover(const Gene& gene1, const Gene& gen2, double prob);
Gene Mutation(const Gene& gene, double prob);
}  // namespace GeneUtils

class Bank final
{
 public:
    Bank(int capacity);

    void Randomize();

    void SortBank();

    void Resize(int newCapacity);
    int GetSize() const;

    void SetGene(int index, Gene gene);
    const Gene& GetGene(int index) const;
    void SetGeneScore(int index, int score);
    int GetGeneScore(int index) const;

    void DumpStats() const;

 private:
    std::vector<Gene> genes_;
};
}  // namespace Solver
