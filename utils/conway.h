#include <iostream>
#include <string>
#include <array>
#include <memory>


namespace aoc {


template <int SIZE, template<typename> typename TEST>
class Conway2D
{
public:
    using grid_t  = std::array<std::array<bool, SIZE>, SIZE>;
    using alive_t = TEST<grid_t>;

    explicit Conway2D()
    {
        // Heap-based grids to not blow the stack. 
        m_grids[0] = std::make_unique<grid_t>();
        m_grids[1] = std::make_unique<grid_t>();
    }

    grid_t& curr_grid() const { return *m_grids[m_curr]; }
    grid_t& prev_grid() const { return *m_grids[(m_curr + 1) % 2]; }

    void step()
    {
        m_curr = (m_curr + 1) % 2;

        alive_t alive;
        for (int x = 1; x < (SIZE-1); ++x)
        {
            for (int y = 1; y < (SIZE-1); ++y)
            {
                curr_grid()[x][y] = alive(prev_grid(), x, y);
            }
        }
    }

    bool get(int x, int y) const
    {
        return curr_grid()[x][y];
    }

    void set(int x, int y)
    {
        curr_grid()[x][y] = true;
    }

    int count() const
    {
        int sum = 0;
        for (int x = 1; x < (SIZE-1); ++x)
        {
            for (int y = 1; y < (SIZE-1); ++y)
            {
                sum += curr_grid()[x][y];
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
                std::cout << (curr_grid()[x][y] ? 'O' : '.') << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

private:
    std::unique_ptr<grid_t> m_grids[2]{};
    int                     m_curr = 0;
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


} // namespace aoc {

