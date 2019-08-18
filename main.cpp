#include "solver.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

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
board read_board(std::istream& in) {
    board parsed(9, std::vector<int>(9));
    int lines = 1;
    std::string line;
    while (std::getline(in, line) && lines <= 9) {
        if (line.size() != 9) {
            throw std::runtime_error("Line #" + std::to_string(lines) + " has invalid size.");
        }
        for (size_t ci = 0; ci < line.size(); ++ci) {
            char c = line[ci];
            if (c == '.') {
                continue;
            } else if (c >= '0' && c <= '9') {
                parsed[lines - 1][ci] = c - '0';
            } else {
                throw std::runtime_error("Line #" + std::to_string(lines) + "contains invalid character: '" + c + "'");
            }
        }
        ++lines;
    }
    if (lines != 10) {
        throw std::runtime_error("The input is missing a line");
    }

    return parsed;
}


} // end anonymous namespace



int main() {
    try {
        auto board = read_board(std::cin);
        auto t1 = std::chrono::high_resolution_clock::now();
        Solver s;
        if (!s.apply_board(board)) {
            std::clog << "There is a contradiction in the input!\n";
            return 2;
        }
        if (s.solve()) {
            std::chrono::duration<double, std::milli> time_taken = std::chrono::high_resolution_clock::now() - t1;
            std::clog << "Solution found in " << time_taken.count() << " ms\n";

            auto solution = s.get_solution();
            for (auto const& row : solution) {
                for (auto const& col : row) {
                    std::cout << col << ' ';
                }
                std::cout << '\n';
            }
        } else {
            std::clog << "Solving the provided input is not possible\n";
        }
    } catch(std::exception const& ex) {
        std::clog << "Failed parsing because: " << ex.what() << std::endl;
        return 1;
    }
}
