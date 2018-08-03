#!/usr/bin/env python3

import plotly
from plotly.graph_objs import Layout as Layout, Box as BoxPlot
from math import ceil


with open('gcc-results-classic.txt', 'r') as classic_file:
    classic_results = sorted([float(line.split(',')[1]) for line in classic_file])

with open('gcc-results-lto.txt', 'r') as lto_file:
    lto_results = sorted([float(line.split(',')[1]) for line in lto_file])


def percentile_bound(data, p):
    c = ceil((len(data) - 1) * (p / 100.))
    return data[c]

xs = list(range(len(classic_results)))


data = [
        BoxPlot(y = classic_results, name='-O3 NDEBUG'),
        BoxPlot(y = lto_results, name='-O3 NDEBUG LTO'),
    ]


plotly.offline.plot({
    'layout': Layout(title = 'SAT-based Sudoku solver runtimes',
                     yaxis = {'title':'Time [ms]'},
                     ),
    'data': data
})
