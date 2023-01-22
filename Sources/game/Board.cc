#include <game/Board.hpp>

#include <bitset>
#include <effolkronium/random.hpp>
#include <queue>
#include <stdexcept>
#include <string>

namespace BOJ
{
void Board::Randomize()
{
    using Random = effolkronium::random_static;

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        board_[i] = Random::get(0, 9);
    }
}

std::uint8_t Board::At(int x, int y) const
{
    return At(pointToIndex(x, y));
}

std::uint8_t Board::At(int index) const
{
    return board_[index];
}

void Board::SetValue(int x, int y, int value)
{
    SetValue(pointToIndex(x, y), value);
}

void Board::SetValue(int index, int value)
{
    if (value < 0 || value > 9)
    {
        throw std::invalid_argument("Value must be between 0 and 9");
    }

    board_[index] = value;
}

int Board::GetScore() const
{
    constexpr int SEARCH_LIMIT = 10000;  // 10K
    std::bitset<SEARCH_LIMIT> marker;

    using Point = std::tuple<int, int, int>;  // x, y, len

    for (int score = 0; score < SEARCH_LIMIT; ++score)
    {
        if (marker[score])
            continue;

        const std::string str = std::to_string(score);
        const int length = str.size();

        std::queue<Point> Q;
        for (int y = 0; y < BOARD_HEIGHT; ++y)
        {
            for (int x = 0; x < BOARD_WIDTH; ++x)
            {
                if (At(x, y) == str[0] - '0')
                {
                    Q.emplace(x, y, 1);
                }
            }
        }

        while (!Q.empty())
        {
            auto [x, y, len] = Q.front();
            Q.pop();

            if (len == length)
            {
                const int reversedScore = std::stoi(std::string(rbegin(str), rend(str)));

                marker[score] = true;
                marker[reversedScore] = true;
                break;
            }

            for (int dx = -1; dx <= 1; ++dx)
            {
                for (int dy = -1; dy <= 1; ++dy)
                {
                    if (dx == 0 && dy == 0)
                        continue;

                    const int nx = x + dx;
                    const int ny = y + dy;

                    if (nx < 0 || nx >= BOARD_WIDTH || ny < 0 || ny >= BOARD_HEIGHT)
                        continue;

                    if (At(nx, ny) == str[len] - '0')
                    {
                        Q.emplace(nx, ny, len + 1);
                    }
                }
            }
        }
    }

    for (int score = 1; score < SEARCH_LIMIT; ++score)
    {
        if (!marker[score])
        {
            return score - 1;
        }
    }

    throw std::runtime_error("there is a problem");
}

const Board::Matrix& Board::GetBoard() const
{
    return board_;
}

std::ostream& operator<<(std::ostream& os, const Board& board)
{
    for (int y = 0; y < Board::BOARD_HEIGHT; ++y)
    {
        for (int x = 0; x < Board::BOARD_WIDTH; ++x)
        {
            os << static_cast<int>(board.At(x, y)) << " ";
        }
        os << std::endl;
    }
    return os;
}
}  // namespace BOJ
