#include <effolkronium/random.hpp>
#include <iostream>

#include <solver/Genetic.hpp>
#include <solver/ThreadPool.hpp>

using namespace BOJ;

constexpr int MAX_BANK_SIZE = 50;

constexpr int N_CROSSOVER = 100;
constexpr int N_MUTATION = 50;

constexpr int N_EPOCHS = 1000;

Solver::Bank makeNewSeeds(const Solver::Bank& bank);
void copyToBank(Solver::Bank& bank, const Solver::Bank& newBank);

int main()
{
    Solver::ThreadPool pool;
    pool.SetAsMainPool();

    Solver::Bank bank(MAX_BANK_SIZE);
    bank.Randomize();

    for (int epoch = 0; epoch < N_EPOCHS; ++epoch)
    {
        auto newBank = makeNewSeeds(bank);

        Solver::BlockInfo blockInfo(0, newBank.GetSize(), 1);
        Solver::parallel_for(
            blockInfo, [&newBank](unsigned blockID, unsigned blockBegin,
                                  unsigned blockEnd) {
                for (unsigned i = blockBegin; i < blockEnd; ++i)
                {
                    BOJ::Board board(newBank.GetGene(i).board);
                    newBank.SetGeneScore(i, board.GetScore());
                }
            });

        newBank.SortBank();

        copyToBank(bank, newBank);

        if (epoch % 100 == 0)
        {
            std::cout << "After " << epoch << " epochs\n";
            bank.DumpStats();
            std::cout << std::endl;
        }
    }

    std::cout << "BEST GENE (score: " << bank.GetGeneScore(0) << ")\n"
              << bank.GetGene(0).board << std::endl;

    return EXIT_SUCCESS;
}

Solver::Bank makeNewSeeds(const Solver::Bank& bank)
{
    Solver::Bank newBank(MAX_BANK_SIZE + N_CROSSOVER + N_MUTATION);

    int currentIdx = 0;
    for (int nCrossover = 0; nCrossover < N_CROSSOVER; ++nCrossover)
    {
        const int idx1 = effolkronium::random_static::get(0, MAX_BANK_SIZE);
        const int idx2 = effolkronium::random_static::get(0, MAX_BANK_SIZE);

        const Solver::Gene& gene1 = bank.GetGene(idx1);
        const Solver::Gene& gene2 = bank.GetGene(idx2);

        Solver::Gene newGene = Solver::GeneUtils::Crossover(gene1, gene2, 0.5);
        newBank.SetGene(currentIdx, newGene);
        ++currentIdx;
    }

    for (int nMutation = 0; nMutation < N_MUTATION; ++nMutation)
    {
        const int idx1 = effolkronium::random_static::get(0, MAX_BANK_SIZE);

        const Solver::Gene& gene1 = bank.GetGene(idx1);

        Solver::Gene newGene = Solver::GeneUtils::Mutation(gene1, 0.2);
        newBank.SetGene(currentIdx, newGene);
        ++currentIdx;
    }

    for (int n = 0; n < MAX_BANK_SIZE; ++n)
    {
        newBank.SetGene(currentIdx, bank.GetGene(n));
        ++currentIdx;
    }

    return newBank;
}

void copyToBank(Solver::Bank& bank, const Solver::Bank& newBank)
{
    for (int n = 0; n < MAX_BANK_SIZE; ++n)
    {
        bank.SetGene(n, newBank.GetGene(n));
    }
}
