#include <iostream>
#include <string>
#include <array>


template <int SIZE, template<typename> typename TEST>
class Conway2D
{
    using grid_t  = std::array<std::array<bool, SIZE>, SIZE>;
    using alive_t = TEST<grid_t>;

public:
    void step()
    {
        int prev = m_curr;
        int curr = (prev + 1) % 2;

        alive_t alive;
        for (int x = 1; x < (SIZE-1); ++x)
        {
            for (int y = 1; y < (SIZE-1); ++y)
            {
                m_grids[curr][x][y] = alive(m_grids[prev], x, y);
            }
        }

        m_curr = curr;
    }

    bool get(int x, int y) const
    {
        return m_grids[m_curr][x][y];
    }

    void set(int x, int y)
    {
        m_grids[m_curr][x][y] = true;
    }

    int count() const
    {
        int sum = 0;
        for (int x = 1; x < (SIZE-1); ++x)
        {
            for (int y = 1; y < (SIZE-1); ++y)
            {
                sum += m_grids[m_curr][x][y];
            }
        }
        return sum;
    }

    void print() const 
    {
        for (int y = 1; y < (SIZE-1); ++y)
        {
            for (int x = 1; x < (SIZE-1); ++x)
            {
                std::cout << (m_grids[m_curr][x][y] ? 'O' : '.') << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

private:
    grid_t m_grids[2]{};
    int    m_curr = 0;
};


template <typename GRID>
struct Conway2DTest
{
    bool operator()(const GRID& grid, int x, int y)
    {
        static constexpr std::array<int, 8> xoff{-1,  0,  1, -1,  1, -1,  0,  1};
        static constexpr std::array<int, 8> yoff{-1, -1, -1,  0,  0,  1,  1,  1};

        int sum = 0;
        for (int i = 0; i < yoff.size(); ++i)
            sum += grid[x+xoff[i]][y+yoff[i]];

        // Any live cell with two or three live neighbours survives.
        // Any dead cell with three live neighbours becomes a live cell.
        // All other live cells die in the next generation. Similarly, all other dead cells stay dead.
        switch (sum)
        {
            case 2: return grid[x][y];
            case 3: return true;
        }
        return false;

    }
};


template <typename GRID>
struct Conway2DTestHex
{
    bool operator()(const GRID& grid, int x, int y)
    {
        // Can use the 2D array to represent a hexagonal tesselation by
        // treating odd rows as shifted half a cell right. Relatvie indices of
        // tested cells depends on whether y is odd or even.
        //      0   1   2   3   4   5   6   7
        //    ---------------------------------
        // 0  |   |   |   |   | n | n |   |   |
        //    -----------------------------------
        // 1    | q | q |   | n | m | n |   |   |
        //    -----------------------------------
        // 2  | q | p | q |   | n | n |   |   |
        //    -----------------------------------
        // 3    | q | q |   |   |   |   |   |   |
        //      ---------------------------------
        static constexpr std::array<int, 6> yoff     {-1, -1,  0,  0,  1,  1};
        static constexpr std::array<int, 6> xoff_odd { 0,  1, -1,  1,  0,  1};
        static constexpr std::array<int, 6> xoff_even{-1,  0, -1,  1, -1,  0};

        const auto& xoff = (y % 2) ? xoff_odd : xoff_even;
        int sum = 0;
        for (int i = 0; i < yoff.size(); ++i)
            sum += grid[x+xoff[i]][y+yoff[i]];

        switch (sum)
        {
            case 2: return grid[x][y];
            case 3: return true;
        }
        return false;
    }
};
