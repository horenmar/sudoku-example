#!/usr/bin/env python3

import subprocess
import argparse
from pathlib import Path
import re
from statistics import stdev, mean, median_high
from math import floor, ceil

time_parser = re.compile(r'Solution found in (\d+.\d+) ms')

num_runs = 10

parser = argparse.ArgumentParser()
parser.add_argument('-b', '--binary', type=str, help='Path to binary to benchmark', required=True)
parser.add_argument('-i', '--input', type=str, help='Path to inputs for the benchmark. Should contain one sudoku per line.', required=True)

args = parser.parse_args()

if not Path(args.binary).is_file():
    print('Argument {} does not specify a valid path to a binary'.format(args.binary))
    exit(1)

if not Path(args.input).is_file():
    print('Argument {} does not specify a valid path to an input file'.format(args.binary))
    exit(2)

def unfurl_line(line):
    assert len(line) == 9 * 9
    return '\n'.join(line[i:i+9] for i in range(0, 81, 9))

def run_with_input(line):
    results  = []
    for i in range(num_runs):
        foo = subprocess.run([args.binary], check=True, input=unfurl_line(line),
                             universal_newlines=True,
                             stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE)
        matched = time_parser.match(foo.stderr)
        results.append(float(matched.groups()[0]))
        
    return mean(results), stdev(results)


with open(args.input, 'r') as input_file, open('table.md', 'w') as table_file, open('results.csv', 'w') as csv_file:
    table_file.write('| Problem | Time taken mean (ms) | Time taken stdev (ms) |\n')
    table_file.write('|---------|----------------------|-----------------------|\n')
    for idx, line in enumerate(input_file):
        line = line.rstrip()
        result = run_with_input(line)
        table_file.write('| {} | {} | {} |\n'.format(idx, *result))
        csv_file.write('{}, {}, {}\n'.format(idx, *result))
        print('Problem: {}, mean: {}, stdev: {}'.format(idx, *result))
