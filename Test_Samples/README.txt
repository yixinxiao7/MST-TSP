There are four input files, sample-ab.txt, sample-c.txt,
sample-d.txt, and sample-e.txt.
  sample-ab.txt has 10000 vertices, sample-c.txt and sample-d.txt
  each have 30, sample-e.txt has 11 vertices.

There are output files for sample-ab.txt run with MST and FASTTSP,
while samples c-e were run with all 3 modes.

Don't run sample ab in OPTTSP mode, there are simply too many
vertices for it to finish.  When running inside of Xcode or Visual
Studio, remember that a debug build (the default) will run much
slower than an optimized build.

When running FASTTSP mode, you might get different answers simply
because you chose a different heuristic.  The autograder knows this,
and you should try to be close to our tour cost.  If you go too far
over our tour cost, your points start degrading (the same as if you
had gone over time or memory).

The file sample-d.txt is the same as sample-c.txt, EXCEPT
that it has been shifted up and to the right by 50, putting all
vertices in Quadrant 1 of the graph.  If you get the same answers
when running samples c and d in MST mode, and sample c output is
wrong, it's because you forgot to account for transitioning from
one region to another.  When running FASTTSP or OPTTSP, if you
get sample d right but sample c wrong, it's because you forgot
that in FAST and OPT, you DO NOT consider the border between regions.

The file sample-e.txt has 11 vertices selected from sample d;
the vertices were selected so that our fast and optimal solutions
give different output.  This test case should help with debugging
OPTTSP mode.
