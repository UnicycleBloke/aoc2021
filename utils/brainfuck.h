#include <vector>
#include <stack>
#include <string>
#include <iostream>


namespace aoc {


//std::string input = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
// Hello, World!
void brainfuck(const std::string& input)
{
    size_t                  pc{};
    size_t                  pos{};
    std::vector<char>       memory(32000);
    std::stack<size_t>      stack{};

    while (pc < input.size())
    {   
        switch (input[pc++])
        {
            // Increment the data pointer (to point to the next cell to the right).
            case '>': ++pos; break;
            // Decrement the data pointer (to point to the next cell to the left).
            case '<': pos = (pos > 0) ? pos -1 : 0; break;
            // Increment (increase by one) the byte at the data pointer.
            case '+': ++memory[pos]; break;
            // Decrement (decrease by one) the byte at the data pointer.
            case '-': --memory[pos]; break;
            // Output the byte at the data pointer.
            case '.': std::cout << memory[pos]; break; ;
            // Accept one byte of input, storing its value in the byte at the data pointer. 
            case ',': std::cin >> memory[pos]; break;
            
            // If the byte at the data pointer is zero, then instead of moving the instruction 
            // pointer forward to the next command, jump it forward to the command after the 
            // matching ] command.
            case '[': 
                if (memory[pos] != 0)
                {
                    stack.push(pc);
                }
                else
                {
                    size_t count = 1;
                    while (count > 0)
                    {
                        count += input[pc] == '[';
                        count -= input[pc] == ']';
                        ++pc;
                    }
                }
                break;

            // If the byte at the data pointer is nonzero, then instead of moving the instruction 
            // pointer forward to the next command, jump it back to the command after the 
            // matching [ command.
            case ']': 
                if (memory[pos] != 0) 
                {
                    pc = stack.top();
                }
                else
                {
                    stack.pop();
                }
                break;  
        }
    }       
}


} // namespace aoc {
