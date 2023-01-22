#include <game/Board.hpp>
#include <iostream>

using namespace BOJ;

int main()
{
    Board board;
    board.Randomize();

    std::cout << board << "\n";
    std::cout << "score: " << board.GetScore() << std::endl;

    return EXIT_SUCCESS;
}
