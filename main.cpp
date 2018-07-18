#include <minisat/core/Solver.h>

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

static const int rows = 9;
static const int columns = 9;
static const int values = 9;

using Minisat::Solver;
using Minisat::vec;
using Minisat::Var;
using Minisat::Lit;

namespace {

Minisat::Solver solver;

Minisat::Var toVar(int row, int column, int value) {
    assert(row >= 0    && row < rows    && "Attempt to get var for nonexistant row");
    assert(column >= 0 && column < columns && "Attempt to get var for nonexistant column");
    assert(value >= 0   && value < values   && "Attempt to get var for nonexistant value");
    return row * columns * values + column * values + value;
}

void init_variables() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            for (int v = 0; v < values; ++v) {
                static_cast<void>(solver.newVar());
            }
        }
    }
}

void exactly_one_true(vec<Lit> const& literals) {
    solver.addClause(literals);
    for (size_t i = 0; i < literals.size(); ++i) {
        for (size_t j = i + 1; j < literals.size(); ++j) {
            solver.addClause(~literals[i], ~literals[j]);
        }
    }
}

void one_square_one_value() {
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            Minisat::vec<Lit> literals;
            for (int value = 0; value < values; ++value) {
                literals.push(Minisat::mkLit(toVar(row, column, value)));
            }
            exactly_one_true(literals);
        }
    }
}

void non_duplicated_values() {
    // In each row, for each value, forbid two column sharing that value
    for (int row = 0; row < rows; ++row) {
        for (int value = 0; value < values; ++value) {
            Minisat::vec<Lit> literals;
            for (int column = 0; column < columns; ++column) {
                literals.push(Minisat::mkLit(toVar(row, column, value)));
            }
            exactly_one_true(literals);
        }
    }
    // In each column, for each value, forbid two rows sharing that value
    for (int column = 0; column < columns; ++column) {
        for (int value = 0; value < values; ++value) {
            Minisat::vec<Lit> literals;
            for (int row = 0; row < rows; ++row) {
                literals.push(Minisat::mkLit(toVar(row, column, value)));
            }
            exactly_one_true(literals);
        }
    }
    // Now forbid sharing in the 3x3 boxes
    for (int r = 0; r < 9; r += 3) {
        for (int c = 0; c < 9; c += 3) {
            for (int value = 0; value < values; ++value) {
                Minisat::vec<Lit> literals;
                for (int rr = 0; rr < 3; ++rr) {
                    for (int cc = 0; cc < 3; ++cc) {
                        literals.push(Minisat::mkLit(toVar(r + rr, c + cc, value)));
                    }
                }
                exactly_one_true(literals);
            }
        }
    }
}


void add_board_model() {
    init_variables();
    one_square_one_value();
    non_duplicated_values();
}

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
        auto board = read_board(std::cin);
        add_board_model();
    } catch(int) {
    }
}
