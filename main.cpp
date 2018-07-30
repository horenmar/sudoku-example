#include "solver.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>


namespace {


/**
 * Input patterns:
 * [1-9] means that square has a digit assigned,
 * . means empty square
 * Example:
 *
 *  123...456
 *  .........
 *  .........
 *  456...789
 *  .........
 *  .........
 *  789...123
 *  .........
 *  .........
 *
 */
std::vector<std::vector<int>> read_board(std::istream& in) {
    std::vector<std::vector<int>> board(9, std::vector<int>(9));
    int lines = 0;
    std::string line;
    while (std::getline(in, line) && lines < 9) {
        if (line.size() != 9) {
            std::clog << "Line #" << lines + 1 << " has invalid size.\n";
            throw 1;
        }
        for (size_t ci = 0; ci < line.size(); ++ci) {
            char c = line[ci];
            if (c == '.') {
                continue;
            } else if (c >= '0' && c <= '9') {
                board[lines][ci] = c - '0';
            } else {
                std::clog << "Invalid character '" << c << "' in line #" << lines + 1 << '\n';
                throw 2;
            }
        }
        ++lines;
    }

    return board;
}


} // end anonymous namespace



int main() {
    try {
        //auto board = read_board(std::cin);
        Solver s(true);
    } catch(int) {
    }
}
