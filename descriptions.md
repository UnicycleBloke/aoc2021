## Brief descriptions of the puzzle types to aid with recycling code for later days.

- Day 1: Sonar Sweep 
    - Depth soundings for lost keys. Iterate a list to compare adjacent (or at a fixed offset) values. Note Part 2 unnecessary to sum groups.
- Day 2: Dive! 
    - Regex input for move instructions. **Mini-FSM** to track position and heading.
- Day 3: Binary Diagnostic 
    - Analysis of a set of binary strings "110110111111" in various ways, ... - the kind of thing Python eats for breakfast.
- Day 4: Giant Squid
    - Bingo - read lines in groups to create bingo cards, first line is the series of calls.
- Day 5: Hydrothermal Venture
    - Lines of vents - draw lines on the sea floor and count the overlaps. Regex file reader worked like a charm.

- Day 6: Lanternfish
    - Exponential growth of fish population - kind of a lagged Fibonacci series. Must learn to not brute force Part 1!!! **Use a rotating array of counters for the recurrence relation.**
- Day 7: The Treachery of Whales
    - Crab submarine alignment - minimise a cost function - sure to be a nice mathsy way, but O(N^2) solution was fine.
- Day 8:  Seven Segment Search 
    - Unscrambling jumbled 7-segment element map. Pay close attention to overlaps of the element sets for each digit. **Made this much harder than I needed to.**
- Day 9:  Smoke Basin 
    - Finding local minima in a height field, and flood filling from the low points.
- Day 10: Syntax Scoring 
    - Matching open/close pairs of characters. A stack-based approach worked best.

- Day 11: Dumbo Octopus 
    - **Cellular automaton** with a small rectangular grid. 
- Day 12: Passage Pathing 
    - Counting paths in a graph (caves are nodes, tunnels are edges). DFS or BFS both work well.
- Day 13: Transparent Origami 
    - Folding a large grid of points on X and Y axes to recover a code word.
- Day 14: Extended Polymerization
    - Counting elements in an exponentially growing string of letters.
- Day 15: Chiton
    - Need to implement **Dijkstra's algorithm** for optimal path through a grid of nodes. 

- Day 16: Packet Decoder
    - Write a **parser for a string of bits** representing a series of nested packets. Packets are literals or operators in an expression which we have to evaluate.
- Day 17: Trick Shot
    - A simple **stepwise simulation** for a dragged ballistic trajectory. Brute was sufficient but trimming the bounds of the initial dx really brought the time down.
- Day 18: Snailfish
    - Special snailfish numbers are represented by trees. **Parse expressions into trees** and then perform operations. The "explode" operation was really hard to get my head around, but the final implementation proved faily simple.  
- Day 19: Beacon Scanner
    - A 3D jigsaw puzzle in which point clouds needed to be **rotated and moved to find overlaps**. Not stupidly difficult but I found it very fiddly to implement correctly. Six nested loops made this slow until some refinements were added to make shortcuts.
- Day 20:
    - A **cellular automaton** very similar to **Conway's Game of Life** but with a more complicated rule for next state. The trick was that the grid extended to infinity but the edges toggled 0 and 1 values.

- Day 21:
- Day 22:
- Day 23:
- Day 24:
- Day 25:

