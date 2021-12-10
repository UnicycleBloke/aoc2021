#include <array>
#include <stack>
#include <string>
#include <iostream>

int main()
{
    size_t pc{};
    std::string input = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";

    size_t pos{};
    std::array<char, 20> memory{};
    std::stack<size_t> stack;

    while (pc < input.size())
    {   
        auto c = input[pc++];
        //std::cout << c;    

        //for (auto m: memory)
        //    std::cout << m << ' ';
        //std::cout << " pc=" << pc << " pos=" << pos << " op=" << c << " sz=" << stack.size() << '\n';

        switch (c)
        {
            // Increment the data pointer (to point to the next cell to the right).
            case '>': ++pos; break;
            
            // Decrement the data pointer (to point to the next cell to the left).
            case '<': --pos; break;
            
            // Increment (increase by one) the byte at the data pointer.
            case '+': ++memory[pos]; break;
            
            // Decrement (decrease by one) the byte at the data pointer.
            case '-': --memory[pos]; break;
            
            // Output the byte at the data pointer.
            case '.': std::cout << memory[pos]; break; // << '\n'; break;
            
            // Accept one byte of input, storing its value in the byte at the data pointer. 
            case ',': std::cin >> memory[pos]; break;
            
            // If the byte at the data pointer is zero, then instead of moving the instruction 
            // pointer forward to the next command, jump it forward to the command after the 
            // matching ] command.
            case '[': 
                if (memory[pos] != 0)
                {
                    //std::cout << "push " << pc << '\n';
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
                    //std::cout << "skip to " << pc << '\n';
                }
                break;

            // If the byte at the data pointer is nonzero, then instead of moving the instruction 
            // pointer forward to the next command, jump it back to the command after the 
            // matching [ command.
            case ']': 
                if (memory[pos] != 0) 
                {
                    pc = stack.top();
                    //std::cout << "mem[" << pos << "]=" << memory[pos] << " goto " << pc << '\n';
                }
                else
                {
                    //std::cout << "mem[" << pos << "]=" << memory[pos] << '\n';
                    stack.pop();
                }
                break;  
        }
    }
       
}
