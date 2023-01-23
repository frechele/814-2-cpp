#include <solver/LocalMinimizer.hpp>

#include <effolkronium/random.hpp>

namespace Solver
{
constexpr int PATIENCE = 30;
constexpr double TEMPERATURE = 1e-4;

Gene StochasticQuench(const Gene& gene)
{
    using Random = effolkronium::random_thread_local;

    Gene bestGene = gene;
    Gene currentGene = gene;

    int failed = 0;
    while (true)
    {
        currentGene.board[Random::get(0, BOJ::Board::BOARD_SIZE - 1)] =
            Random::get(0, 9);

        BOJ::Board board(currentGene.board);
        currentGene.score = board.GetScore();

        if (currentGene.score > bestGene.score)
        {
            bestGene = currentGene;
            failed = 0;
        }
        else if (Random::get(0.0, 1.0) >
                 std::exp((currentGene.score - bestGene.score) / TEMPERATURE))
        {
            ++failed;
            currentGene = bestGene;
        }

        if (failed > PATIENCE)
        {
            break;
        }
    }

    return bestGene;
}
}  // namespace Solver
