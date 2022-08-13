#include "bf.h"

int interpret_bf(std::string program)
{
    std::vector<uint32_t> loops = {0, 0};
    std::vector<uint32_t> loop_cache = {};
    std::vector<uint32_t> loop_stack = {};
    for (uint32_t i = 0; i < program.length(); i++)
    {
        if (i > 0)
        {
            if (program[i - 1] == ']')
            {
                loop_stack.pop_back();
                loop_cache.push_back(loop_stack.size() == 0 ? 0 : loop_stack.back());
            }
            else
                loop_cache.push_back(loop_cache[i - 1]);
        }
        else
            loop_cache.push_back(0);

        if (program[i] == '[')
        {
            uint32_t level = 0;
            for (uint32_t j = i; j < program.length(); j++)
            {
                if (program[j] == '[')
                    level++;
                if (program[j] == ']')
                {
                    level--;
                    if (level == 0)
                    {
                        loops.push_back(i);
                        loops.push_back(j);
                        break;
                    }
                }
            }
            if (level != 0)
                return 1;

            loop_stack.push_back((loops.size() - 2) / 2);
            loop_cache[i] = loop_stack.back();
        }
    }

    std::vector<uint8_t> ram = {0};
    std::vector<uint8_t> rram = {};
    int32_t pointer = 0;
    for (uint32_t i = 0; i < program.length(); i++)
    {
        switch (program[i])
        {
        case '<':
            pointer--;
            while (-pointer > (int)rram.size())
                rram.push_back(0);
            break;
        case '>':
            pointer++;
            while (pointer >= (int)ram.size())
                ram.push_back(0);
            break;
        case '+':
            (pointer < 0 ? rram[-pointer] : ram[pointer])++;
            break;
        case '-':
            (pointer < 0 ? rram[-pointer] : ram[pointer])--;
            break;
        case '.':
            putchar((pointer < 0 ? rram[-pointer] : ram[pointer]));
            // std::cout << " " << (int)(pointer < 0 ? rram[-pointer] : ram[pointer]) << " ";
            break;
        case ',':
            (pointer < 0 ? rram[-pointer] : ram[pointer]) = getchar();
            break;
        case '[':
            if ((pointer < 0 ? rram[-pointer] : ram[pointer]) == 0)
                i = loops[loop_cache[i] * 2 + 1];
            break;
        case ']':
            if ((pointer < 0 ? rram[-pointer] : ram[pointer]) != 0)
                i = loops[loop_cache[i] * 2];
            break;
        default:
            break;
        }
        // std::cout << i << " " << pointer << " (" << (int)(pointer < 0 ? rram[-pointer] : ram[pointer]) << ") [" << (ram.size() + rram.size()) << "]" << std::endl;
    }
    return 0;
}
