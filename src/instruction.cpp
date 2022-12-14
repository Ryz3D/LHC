#include "instruction.h"

Instruction::Instruction(std::string label)
{
    this->label = label;
}

Instruction::Instruction(uint8_t control_word, uint8_t literal, std::string comment)
{
    this->control_word = control_word;
    this->literal = literal;
    this->comment = comment;
}

Instruction::Instruction(uint8_t control_word, std::string label_literal, std::string comment)
{
    this->control_word = control_word;
    this->label_literal = label_literal;
    this->comment = comment;
}

Instruction *Instruction::parse_ass(std::string str)
{
    Instruction *ins = new Instruction(0);
    bool out = true;
    bool has_literal = false;
    std::string cw_buffer = "";

    size_t i;
    for (i = 0; i < str.size(); i++)
        if (!CHAR_IS_EMPTY(str[i]))
            break;

    if (str.size() > 1)
    {
        if (str[i] == ';')
        {
            ins->label = str.substr(i + 1);
            return ins;
        }
        else if (str[i] == ':')
        {
            size_t start = i + 1;
            for (; i < str.size(); i++)
                if (CHAR_IS_EMPTY(str[i]) || str.substr(i, 2) == "->")
                    break;
            ins->label_literal = str.substr(start, i - start);
            for (; i < str.size(); i++)
                if (str[i] == '>')
                    break;
            i++;
            has_literal = true;
            out = false;
        }
    }

    for (; i < str.size(); i++)
    {
        if (out && i < str.size() - 1)
        {
            if (str[i] == '-' && str[i + 1] == '>')
            {
                ins->parse_ins_part(out, &has_literal, cw_buffer);
                cw_buffer.clear();
                out = false;
                i++;
            }
        }
        if (str[i] == '#')
        {
            ins->parse_ins_part(out, &has_literal, cw_buffer);
            ins->comment = str.substr(i + 1);
            return ins;
        }
        else if (str[i] == ' ')
        {
            ins->parse_ins_part(out, &has_literal, cw_buffer);
            cw_buffer.clear();
        }
        else if (('a' <= str[i] && str[i] <= 'z') ||
                 ('A' <= str[i] && str[i] <= 'Z') ||
                 ('0' <= str[i] && str[i] <= '9') ||
                 str[i] == '_')
            cw_buffer += str[i];
    }
    if (out)
        return nullptr;

    ins->parse_ins_part(out, &has_literal, cw_buffer);
    return ins;
}

uint8_t Instruction::parse_cw(bool out, std::string cw)
{
    if (cw == "RAM")
        return out ? 1 << INS_RAM_OUT : 1 << INS_RAM_IN;
    else if (cw == "RAM_P")
        return out ? 1 << INS_RAM_P_OUT : 1 << INS_RAM_P_IN;
    else if (cw == "A" && !out)
        return 1 << INS_A_IN;
    else if (cw == "B" && !out)
        return 1 << INS_B_IN;
    else if (cw == "ADD" && out)
        return 1 << INS_ALU_ADD;
    else if (cw == "INV" && out)
        return 1 << INS_ALU_INV;
    else
        return 0;
}

void Instruction::parse_ins_part(bool out, bool *has_literal, std::string str)
{
    if (out && !*has_literal)
    {
        if (str[0] == '!' && str.size() > 1)
        {
            size_t i = 1;
            while ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9') || str[i] == '_')
                label_literal += str[i++];
            *has_literal = true;
        }
        else
        {
            bool is_literal = false;
            bool is_negative = false;

            literal = 0;
            for (size_t j = 0; j < str.size(); j++)
            {
                if (j == 0 && str[j] == '-')
                    is_negative = true;
                else if ('0' <= str[j] && str[j] <= '9')
                {
                    is_literal = true;
                    literal *= 10;
                    literal += str[j] - '0';
                }
                else
                {
                    is_literal = false;
                    literal = 0;
                    break;
                }
            }
            if (is_literal)
            {
                if (is_negative)
                    literal = 256 - literal;
                *has_literal = true;
            }
        }
    }
    control_word |= Instruction::parse_cw(out, str);
}

bool Instruction::literal_out()
{
    return control_word >> 4 == 0;
}

std::string Instruction::to_ass()
{
    if (label.size() > 0)
        return ";" + label;

    std::string ass = "";

    if ((control_word >> INS_RAM_OUT) & 1)
        ass += "RAM ";
    if ((control_word >> INS_RAM_P_OUT) & 1)
        ass += "RAM_P ";
    if ((control_word >> INS_ALU_ADD) & 1)
        ass += "ADD ";
    if ((control_word >> INS_ALU_INV) & 1)
        ass += "INV ";

    if (literal_out())
    {
        if (label_literal.size() > 0)
            ass += ":" + label_literal + " ";
        else
            ass += std::to_string(literal) + " ";
    }

    ass += "-> ";

    if ((control_word >> INS_RAM_IN) & 1)
        ass += "RAM ";
    if ((control_word >> INS_RAM_P_IN) & 1)
        ass += "RAM_P ";
    if ((control_word >> INS_A_IN) & 1)
        ass += "A ";
    if ((control_word >> INS_B_IN) & 1)
        ass += "B ";

    while (ass.back() == ' ')
        ass = ass.substr(0, ass.size() - 1);

    if (comment.size() > 0)
        return ass + " # " + comment;
    else
        return ass;
}
