# tests/lockstep

Milestone W6. Runs each golden scenario in the native build with the same seed and autoplay,
dumps state at every turn boundary, and compares with the oracle. On a divergence it prints the
scenario, turn and nation, the first differing global by model name, and the first differing
`rand()` call.
