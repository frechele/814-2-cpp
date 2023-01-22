#include <solver/Genetic.hpp>

#include <algorithm>
#include <effolkronium/random.hpp>
#include <iostream>

namespace Solver
{
namespace GeneUtils
{
Gene Crossover(const Gene& gene1, const Gene& gene2, double prob)
{
    using Random = effolkronium::random_thread_local;

    BOJ::Board::Matrix newGene;

    for (int i = 0; i < BOJ::Board::BOARD_SIZE; ++i)
    {
        if (Random::get(0.0, 1.0) < prob)
        {
            newGene[i] = gene1.board[i];
        }
        else
        {
            newGene[i] = gene2.board[i];
        }
    }

    return Gene{ std::move(newGene), -1 };
}

Gene Mutation(const Gene& gene, double prob)
{
    using Random = effolkronium::random_thread_local;

    BOJ::Board::Matrix newGene = gene.board;

    for (int i = 0; i < BOJ::Board::BOARD_SIZE; ++i)
    {
        if (Random::get(0.0, 1.0) < prob)
        {
            newGene[i] = Random::get(0, 9);
        }
    }

    return Gene{ std::move(newGene), -1 };
}
}  // namespace GeneUtils

Bank::Bank(int capacity) : genes_(capacity)
{
}

void Bank::Randomize()
{
    BOJ::Board board;

    for (auto& gene : genes_)
    {
        board.Randomize();

        gene.board = board.GetBoard();
        gene.score = board.GetScore();
    }

    SortBank();
}

void Bank::Resize(int newCapacity)
{
    genes_.resize(newCapacity);
}

int Bank::GetSize() const
{
    return genes_.size();
}

void Bank::SetGene(int index, Gene gene)
{
    genes_[index] = std::move(gene);
}

const Gene& Bank::GetGene(int index) const
{
    return genes_[index];
}

void Bank::SetGeneScore(int index, int score)
{
    genes_[index].score = score;
}

int Bank::GetGeneScore(int index) const
{
    return genes_[index].score;
}

void Bank::SortBank()
{
    std::sort(genes_.begin(), genes_.end(),
              [](const Gene& gene1, const Gene& gene2) {
                  return gene1.score > gene2.score;
              });
}

void Bank::DumpStats() const
{
    std::cout << "[Bank status]\n"
              << "Size: " << genes_.size() << '\n'
              << "Max score: " << genes_.front().score << '\n'
              << "Min score: " << genes_.back().score << std::endl;
}
}  // namespace Solver
