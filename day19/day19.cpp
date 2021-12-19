#include "utils.h"


struct Pos 
{
    int x{}; int y{}; int z{};
};
using Cube = set<Pos>;

bool operator<(Pos a, Pos b)
{
    if (a.x < b.x) return true;
    if ((a.x == b.x) && (a.y < b.y)) return true;
    if ((a.x == b.x) && (a.y == b.y) && (a.z < b.z)) return true;
    return false;
}
Pos operator+(Pos a, Pos b)
{
    return Pos{a.x + b.x, a.y + b.y, a.z + b.z};
}
Pos operator-(Pos a, Pos b)
{
    return Pos{a.x - b.x, a.y - b.y, a.z - b.z};
}
void print(Pos p)
{
    cout << "(" << p.x << "," << p.y << "," << p.z <<")\n";
}

struct CubeInfo
{
    Cube points2{};
    Cube points{};
    Cube rots{};   // Set of rotation - remove as not found
    Pos  offset{}; // From world (0,0,0) or something
    CubeInfo* parent{};
    int index{};
    int parent_idx{-1};

    Pos get_offset() const
    { 
        if (index == 0) 
        {
            cout << index << "\n" << endl;    
            return Pos{0, 0, 0};
        }
        if (parent == nullptr)
        {
            cout << "null parent " << index << endl;    
            return Pos{0, 0, 0};
        }
        cout << index << " -> ";
        return offset + parent->get_offset(); 
    }
};




void print(const Cube& c)
{
    for (auto [x, y, z]: c)
        cout << "(" << x << "," << y << "," << z <<") ";
    cout << '\n';
}


// set<Pos> find_rots()
// {
//     set<Pos> pos;
//     set<Pos> rot;
//     const Pos p{1, 2, 3};

//     for (int i: aoc::range(4))
//     {
//         for (int j: aoc::range(4))
//         {
//             for (int k: aoc::range(4))
//             {
//                 auto r = rotate(p, {i, j, k});
//                 auto [it, added] = pos.insert(r);
//                 if (added)
//                     rot.insert({i, j, k});
//             }
//         }
//     }

//     cout << rot.size() << '\n';
//     for (auto [i, j, k]: rot)
//         cout << "rota (" << i << "," << j << "," << k <<")\n";

//     return rot;
// }


const set<Pos> g_rots =
{
    {0, 0, 0},
    {0, 0, 1},
    {0, 0, 2},
    {0, 0, 3},
    {0, 1, 0},
    {0, 1, 1},
    {0, 1, 2},
    {0, 1, 3},
    {0, 2, 0},
    {0, 2, 1},
    {0, 2, 2},
    {0, 2, 3},
    {0, 3, 0},
    {0, 3, 1},
    {0, 3, 2},
    {0, 3, 3},
    {1, 0, 0},
    {1, 0, 1},
    {1, 0, 2},
    {1, 0, 3},
    {1, 2, 0},
    {1, 2, 1},
    {1, 2, 2},
    {1, 2, 3},
};


Pos rotate(const Pos& pos, const Pos& rot)
{
    Pos p = pos;

    for (int i: aoc::range(rot.x))       
    {
        // About x
        auto y = p.y; 
        p.y = -p.z;
        p.z = y;
    }

    for (int j: aoc::range(rot.y))
    {
        // About y
        auto x = p.x; 
        p.x = -p.z;
        p.z = x;
    }

    for (int k: aoc::range(rot.z))
    {
        // About z
        auto x = p.x; 
        p.x = -p.y;
        p.y = x;
    }

    return p;
}


Cube rotate(const Cube& cube, const Pos& rot)
{
    Cube res;
    for (auto p: cube)
        res.insert(rotate(p, rot));
    return res;
}


// Make points relative to one in the set.
Cube offset(const Cube& c, Pos p)
{
    Cube r;
    for (auto& [x, y, z]: c)
        r.insert({x-p.x, y-p.y, z-p.z});
    return r;
}


Cube common(const Cube& a, const Cube& b)
{
    Cube res;
    for (auto i: a) res.insert(i);
    for (auto i: b) res.insert(i);
    return res;
}


bool overlaps(const Cube& a, const Cube& b, Pos& off)
{
    for (Pos pa: a)
    {
        auto a2 = offset(a, pa);
        for (Pos pb: b)
        {
            auto b2 = offset(b, pb);
            auto c  = common(a2, b2);

            if ((c.size() + 12) <= (a.size() + b.size()))
            {
                Pos d{pa.x - pb.x, pa.y - pb.y, pa.z - pb.z};
                off = d;
                cout << "delta"; 
                print(d);
                cout << endl; 
                return true;
            }
        }
    }
    return false;
}


bool overlaps2(CubeInfo& a, CubeInfo& b)
{
    for (auto ra: a.rots)
    {
        auto ca = rotate(a.points, ra);
        for (auto rb: b.rots)
        {
            auto cb = rotate(b.points, rb);

            Pos offset{};
            if (overlaps(ca, cb, offset))
            {
                if (a.rots.size() > 1)
                {
                    a.rots.clear();
                    a.rots.insert(ra);
                    a.offset  = Pos{} - offset;
                    a.parent  = &b;
                    a.points2 = ca;
                    a.parent_idx = b.index;
                }

                if (b.rots.size() > 1)
                {
                    b.rots.clear();
                    b.rots.insert(rb);                    
                    b.offset  = offset;
                    b.parent  = &a;
                    b.points2 = cb;
                    b.parent_idx = a.index;
                }

                return true;
            }
        }
    }    
    return false;
}


int manhattan(Pos p1, Pos p2)
{
    return abs(p1.x - p2.x) + abs(p1.y - p2.y) + abs(p1.z - p2.z);
}


template <typename T>
auto part1(T input)
{ 
    set<size_t> unsolved;
    for (auto i: aoc::range(1U, input.size()))
        unsolved.insert(i);

    set<size_t> solved;
    for (auto i: aoc::range(1U, input.size()))
    {
        if (overlaps2(input[0], input[i]))
        {  
            solved.insert(i);
            solved.insert(0);
            unsolved.erase(i);
        } 
    }
    
    while (solved.size() < input.size())
    {
        //set<size_t> solved2;

        bool stop = false;
        for (auto i: solved)
        {
            //solved2.insert(i);
            //for (auto j: aoc::range(input.size()))
            for (auto j: unsolved)
            {
                if (solved.find(j) != solved.end()) continue;

                cout << "cubes " << solved.size() << ' ' << i << " " << j << '\n';
                if (overlaps2(input[i], input[j]))
                {  
                    //solved2.insert(j);
                    unsolved.erase(j);
                    solved.insert(j);
                    stop = true;
                    break;
                } 
            }

            if (stop) break;
        }
        
        //solved = solved2;
    }


    cout << "done" << endl;

    for (auto& c: input)
    {
        print(c.get_offset());
    }

    set<Pos> beacons;
    for (auto& s: input)
    {
        for (auto p: s.points2)
        {
            Pos off = s.get_offset();
            Pos b{p.x + off.x, p.y + off.y, p.z + off.z};
            print(b);
            beacons.insert(b);
        }
    }

    int max_dist = 0;
    for (auto i: aoc::range(input.size()))
    {
        auto p1 = input[i].get_offset();
        for (auto j: aoc::range(i+1, input.size()))
        {
            auto p2 = input[j].get_offset();
            auto dist = manhattan(p1, p2);
            cout << i << " " << j << " " << dist << '\n';
            max_dist = max(max_dist, dist);
        }
    }
    
    return max_dist;
}


template <typename T>
auto part2(T input)
{
    aoc::timer timer;
    return 0;
}


void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, false); 

    vector<CubeInfo> cubes;
    CubeInfo info;
    info.rots = g_rots;

    for (auto i: aoc::range(1U, lines.size()))
    {
        auto line = lines[i];

        if ((line[0] == '-') && (line[1] == '-'))
        {
            cubes.push_back(info);
            info.points.clear();
            ++info.index;
        }
        else
        {
            auto s = aoc::replace(line, ",", " ");
            istringstream is(s);
            Pos pos;
            is >> pos.x >> pos.y >> pos.z; 
            info.points.insert(pos);
        }
    }
    cubes.push_back(info);

    auto p1 = part1(cubes);
    cout << "Part1: " << p1 << '\n';
    //aoc::check_result(p1, 3647);

    auto p2 = part2(cubes);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 4600);
}


int main(int argc, char** argv)
{
    aoc::timer timer;
    try
    {
        if (argc < 2)
        {
            cout << "Provide input file name\n";
            return -1;
        }

        fs::path path{argv[1]};
        if (!fs::is_regular_file(path))
        {
            cout << "Path '" << argv[1] << "' does not exist or is not a file\n";
            return -1;
        }

        run(argv[1]);
    }
    catch (std::exception& e)
    {
        cout << e.what();
    }
}
