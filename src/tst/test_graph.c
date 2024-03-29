#include <stdio.h>
#include <gsl/gsl_spmatrix.h>
#include "../graph.h"
#include "../pile.h"

int check(int n, int att)
{
  return n == att;
}

//tests for the initialization of a graph_t
int test_graph(void)
{
  int c = 0;
  struct graph_t *g = new__graph_t(3, 'h');
  c += check(g->num_vertices, 16);
  c += check(gsl_spmatrix_get(g->t, 0, 0), 0);
  c += check(gsl_spmatrix_get(g->t, 1, 0), 1);
  c += check(gsl_spmatrix_get(g->t, 0, 1), 1);
  c += check(gsl_spmatrix_get(g->t, 1, 1), 0);
  c += check(gsl_spmatrix_get(g->t, 1, 3), 0);
  c += check(gsl_spmatrix_get(g->t, 3, 0), 0);
  c += check(gsl_spmatrix_get(g->o, 1, 2), 0);
  c += check(gsl_spmatrix_get(g->o, 0, 2), 3);
  free__graph_t(g);
  return c;
}

//tests for the pile
int test_pile(void)
{
  int c = 0;
  struct pile *p = pile_vide();
  c += check(p->size, 0); 
  for (int i = 0; i < 10; i++)
  {
    empiler(p, i);
  }
  c += check(p->size, 10);
  for (int i = 0; i < 8; i++)
  {
    depiler(p);
  }
  c += check(p->size, 2);    
  c += check(depiler(p), 1); 
  c += check(depiler(p), 0); 
  c += check(p->size, 0);
  c += check(depiler(p), -1);
  pile_free(p);
  return c;
}

//tests for is_winning
int test_pathfinding(void)
{
  int c = 0;
  //carré
  struct graph_t *g = new__graph_t(3, 'c');
  struct move_t move = {5, 1};
  coloriate__graph_t(g, 1, move);
  c += check(is_winning(g, 1, move, 'c'), 0);

  move.m = 6;
  coloriate__graph_t(g, 1, move);
  c += check(is_winning(g, 1, move, 'c'), 1);
  free__graph_t(g);

  //hexa
  struct graph_t *gg = new__graph_t(5, 'h');
  move.m = 7;
  coloriate__graph_t(gg, 0, move);
  move.m = 8;
  coloriate__graph_t(gg, 0, move);
  move.m = 13;
  coloriate__graph_t(gg, 1, move);
  move.m = 19;
  coloriate__graph_t(gg, 1, move);
  move.m = 25;
  coloriate__graph_t(gg, 0, move);
  move.m = 14;
  coloriate__graph_t(gg, 0, move);

  c += check(is_winning(gg, 0, move, 'c'), 0);

  move.m = 6;
  coloriate__graph_t(gg, 0, move);

  move.m = 20;
  coloriate__graph_t(gg, 0, move);
  c += check(is_winning(gg, 0, move, 'c'), 1);
  free__graph_t(gg);

  //triangle
  struct graph_t *t = new__graph_t(5, 't');
  for (int i = 0; i < 15; i++)
  {
    move.m = 59 + i;
    coloriate__graph_t(t, 1, move);
  }
  move.m = 41;
  coloriate__graph_t(t, 0, move);
  c += check(is_winning(t, 0, move, 't'), 0);

  move.m = 58;
  coloriate__graph_t(t, 1, move);
  c += check(is_winning(t, 0, move, 't'), 0);
  free__graph_t(t);
  return c;
}

//tests for the initialization of the triangular graph_t
int test_tr()
{
  struct graph_t *g = new__graph_t(5, 't');
  int c = 0;
  c += check(g->num_vertices, 150);
  c += check(gsl_spmatrix_get(g->t, 0, 0), 0);
  c += check(gsl_spmatrix_get(g->t, 1, 0), 1);
  c += check(gsl_spmatrix_get(g->t, 0, 1), 1);
  c += check(gsl_spmatrix_get(g->t, 1, 1), 0);
  c += check(gsl_spmatrix_get(g->t, 1, 3), 0);
  c += check(gsl_spmatrix_get(g->t, 3, 0), 0);
  c += check(gsl_spmatrix_get(g->o, 1, 2), 0);
  c += check(gsl_spmatrix_get(g->o, 0, 2), 3);
  c += check(gsl_spmatrix_get(g->o, 1, 0), 2);
  c += check(gsl_spmatrix_get(g->o, 0, 0), 0);
  c += check(gsl_spmatrix_get(g->o, 1, 149), 3);
  c += check(gsl_spmatrix_get(g->o, 0, 149), 0);
  c += check(gsl_spmatrix_get(g->o, 1, 148), 0);
  c += check(gsl_spmatrix_get(g->o, 0, 148), 2);

  free__graph_t(g);
  return c;
}
