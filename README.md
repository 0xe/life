# Motivation

Implement [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway's_Game_of_Life) in C using olcPixelGameEngine for graphics.

# Rules (from wikipedia)

At any time tick, apply these rules in order:

- Any live cell with fewer than two live neighbours dies, as if by underpopulation.
- Any live cell with two or three live neighbours lives on to the next generation.
- Any live cell with more than three live neighbours dies, as if by overpopulation.
- Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

These rules are applied to the initial "seed" to compute the generation and then each successive one and so on.
