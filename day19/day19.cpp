#include "utils.h"


struct Pos 
{
    int x{}; int y{}; int z{};
};
using Cube = set<Pos>;
using Rot  = Pos;


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


int manhattan(Pos p1, Pos p2)
{
    return abs(p1.x - p2.x) + abs(p1.y - p2.y) + abs(p1.z - p2.z);
}


int distance(Pos p1, Pos p2)
{
    int x = p1.x-p2.x;
    int y = p1.y-p2.y;
    int z = p1.z-p2.z;
    return x*x + y*y + z*z;
}


using Cloud = vector<Pos>;

struct Scanner
{
    // 24 sets of points created once.
    vector<Cloud> clouds{};
    int           rotation{}; // Index into clouds
    bool          solved{};   // We have fixed the location
    Pos           offset{};   // Relative to another.
    Scanner*      parent{};
    int           index{};  

    // Added later as a performance booster.
    // This was something I considered early on, but gave up in haste. I see 
    // that someone else used this idea, and I have revisited the code to add it. 
    // Basically create a characteristic from the distances between all the points 
    // in a scanner's point cloud. This is independently of orientation, and is a 
    // great filter to omit pointless checks of 24 * 24 rotations.
    vector<int>   fingerprint{}; 
    void make_fingerprint()
    {
        // We have 25 or so points, so have N(N-1)/2 distances to consider.
        const auto& c = clouds[0];
        for (auto i: aoc::range(c.size()))
            for (auto j: aoc::range(i + 1, c.size()))
                fingerprint.push_back(distance(c[i], c[j]));
    }

    Pos get_offset() const
    { 
        if (index == 0) 
        {
            //cout << index << "\n" << endl;    
            return Pos{0, 0, 0};
        }
        if (parent == nullptr)
        {
            cout << "null parent " << index << endl;    
            return Pos{0, 0, 0};
        }
        //cout << index << " -> ";
        return offset + parent->get_offset(); 
    }
};


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


Pos rotate(const Pos& pos, const Rot& rot)
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


Cloud rotate(const Cloud& cloud, const Rot& rot)
{
    Cloud res;
    for (auto p: cloud)
        res.push_back(rotate(p, rot));
    return res;
}


// Make points relative to one in the set.
Cloud offset(const Cloud& c, const Pos& p)
{
    Cloud r;
    for (auto& [x, y, z]: c)
        r.push_back({x-p.x, y-p.y, z-p.z});
    return r;
}


bool check_overlap(const Cloud& a, const Cloud& b, Pos& offset)
{
    // Check pairs of points as offsets to make overlaps happen.
    // About 25 * 25 tests at most. Not terrible, but nested in more loops.
    for (auto i: aoc::range(a.size()))
    {
        const auto& pa = a[i];
        for (auto j: aoc::range(b.size()))
        {
            const auto& pb = b[j];

            set<Pos> s;
            for (auto p: a) s.insert(p - pa);
            for (auto p: b) s.insert(p - pb);

            if ((s.size() + 12) <= (a.size() + b.size()))
            {
                //cout << s.size() << " " << a.size() << " " << b.size() << "\n";
                offset = pa - pb;
                return true;
            }
        }
    }

    return false;
}


bool check_fingerprints(Scanner& a, Scanner& b)
{
    set<int> s;
    for (auto d: a.fingerprint) s.insert(d);
    for (auto d: b.fingerprint) s.insert(d);

    // We expect at least 12 common points, so have N(N-1)/2 = 66 distances to be the same.
    return (s.size() + 66) <= (a.fingerprint.size() + b.fingerprint.size());
}


bool check_overlap(Scanner& a, Scanner& b, int& solved)
{
    //cout << "check_overlap scans  " << a.index << " " << b.index << "\n"; 

    // For pairs of rotated point clouds.
    // Up to 24 x 24 comparisons. This is starting to get expensive.
    for (auto i: aoc::range(a.clouds.size()))
    {
        if (a.solved && (i != a.rotation)) continue;
        for (auto j: aoc::range(b.clouds.size()))
        {
            if (b.solved && (j != b.rotation)) continue;

            if (!check_fingerprints(a, b)) continue;

            //cout << "check_overlap clouds " << i << " " << j << "\n"; 

            Pos offset{};
            if (check_overlap(a.clouds[i], b.clouds[j], offset))
            {
                if (!a.solved)
                {
                    a.solved   = true;
                    a.offset   = Pos{} - offset;
                    a.parent   = &b;
                    a.rotation = i; 
                    ++solved;

                    cout << "solved  " << solved <<  " " << a.index << "\n"; 
                    print(Pos{} - offset); 
                }
                if (!b.solved)
                {
                    b.solved   = true;
                    b.offset   = offset;
                    b.parent   = &a;
                    b.rotation = j; 
                    ++solved;

                    cout << "solved  " << solved <<  " " << b.index << "\n"; 
                    print(offset); 
                }

                return true;
            }
        }
    }

    return false;
}


auto part1(vector<Scanner>& scanners)
{
    cout << "Part1\n";
    
    int solved = 0;
    while (solved < scanners.size())
    {
        // For pairs of scanners.
        for (auto i: aoc::range(scanners.size()))
        {
            auto& sa = scanners[i];
            for (auto j: aoc::range(i + 1, scanners.size())) 
            {
                auto& sb = scanners[j];
                if ((i > 0) && !sa.solved && !sb.solved) continue;
                check_overlap(sa, sb, solved);
            }
        }
    }
    scanners[0].parent = nullptr; 

    // for (auto& s: scanners)
    // {
    //     Pos off = s.get_offset();
    //     print(off);
    // }

    set<Pos> beacons;
    for (auto& s: scanners)
    {
        Pos off = s.get_offset();
        for (auto p: s.clouds[s.rotation])
        {
            Pos b = p + off;
            //print(b);
            beacons.insert(b);
        }
    }
    return beacons.size();
}


auto part2(vector<Scanner>& scanners)
{
    int max_dist = 0;
    for (auto i: aoc::range(scanners.size()))
    {
        auto p1 = scanners[i].get_offset();
        for (auto j: aoc::range(i + 1, scanners.size()))
        {
            auto p2   = scanners[j].get_offset();
            auto dist = manhattan(p1, p2);
            max_dist  = max(max_dist, dist);
        }
    }

    return max_dist;
}

void run(const char* filename)
{
    auto lines = aoc::read_lines(filename, false); 

    vector<Scanner> scanners;
    int index = 0;
    size_t i = 0;
    while (i < lines.size())
    {
        auto line = lines[i++];
        if ((line[1] == '-'))
        {
            Cloud cloud;
            while ((i < lines.size()) && (lines[i][1] != '-'))
            {
                line = lines[i++];

                auto s = aoc::replace(line, ",", " ");
                istringstream is(s);
                Pos pos;
                is >> pos.x >> pos.y >> pos.z; 
                cloud.push_back(pos);
            }

            Scanner scanner;
            scanner.index = index++;           
            // Add all 24 rotations avoid doing this repeatedly.
            for (auto rot: g_rots)
            {
                scanner.clouds.push_back(rotate(cloud, rot));
            }
            scanner.make_fingerprint();

            scanners.push_back(scanner);
        }
    }

    auto p1 = part1(scanners);
    cout << "Part1: " << p1 << '\n';
    aoc::check_result(p1, 440U);

    auto p2 = part2(scanners);
    cout << "Part2: " << p2 << '\n';
    aoc::check_result(p2, 13382);
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
