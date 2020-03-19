#include <stddef.h>
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_spmatrix.h>
#include <math.h>

void assign_links(gsl_spmatrix* t, char c, int n);
void assign_tr(gsl_spmatrix* t, char c, int n);
void assign_colors(gsl_spmatrix* o, int n);

//le triangulaire ne marche pas pour n impair
struct graph_t* new__graph_t(int n, char c){
  if (c != 't'){
    n++;
  }
  struct graph_t* g = malloc(sizeof(struct graph_t));
  g->num_vertices = n*n * (1 + 5 * (c == 't'));//n² sauf si triangulaire auquel cas on a 6n²
  g->t = gsl_spmatrix_alloc(g->num_vertices, g->num_vertices);
  g->o = gsl_spmatrix_alloc(2, g->num_vertices);
  if (c != 't'){
    assign_links(g->t, c, n);
  }
  else{
    assign_tr(g->t, c, n);
  }
    printf("sortie1\n");
  if (c != 't'){
    assign_colors(g->o, n);
  }
  printf("sortie2\n");
  return g;
}

void free__graph_t(struct graph_t* g){
  gsl_spmatrix_free(g->o);
  gsl_spmatrix_free(g->t);
  free(g);
}
    
void print__mat(const struct graph_t* g){
  int n = g->num_vertices;
  for (int i = 0; i < n; i++){
    printf("(");
    for (int j = 0; j < n; j++){
      int s = (int) gsl_spmatrix_get(g->t, i, j);
	if (s != 0){
	  printf("\033[0;33m");
	}
      printf("%d ", s);
      printf("\033[0m");
    }
    printf(") %d\n", i);
  }
}

void coloriate__graph_t(struct graph_t* g, int color, int n){
  gsl_spmatrix_set(g->o, color, n, 1);
}

void print_square(struct graph_t* g){//pour graphe carré
  int m = sqrt(g->num_vertices);
  for (int i = 0; i < m; i++){
    printf("(");
    for (int j = 0; j < m; j++){
      int s = (int) gsl_spmatrix_get(g->o, 0, j + i*m);
      int ss = (int) gsl_spmatrix_get(g->o, 1, j + i*m);
      if (s > ss){
	printf("\033[0;31m");
      }
      if (s < ss){
	printf("\033[0;32m");
      }
      printf(" o ");
      printf("\033[0m");
    }
    printf(")\n");
  }
}

void print_hex(struct graph_t* g){//pour graphe hexa
  printf("\n");
  int m = sqrt(g->num_vertices);
  char* s = malloc(sizeof(char) * (2*m+5));
  char* ss = s + 2*m + 3;
  ss[0] = '\n';
  ss[1] = '\0';
  for (int i = 0; i < m; i++){
    if (i != 0){
      ss -= 2;
      ss[2] = ' ';
      ss[1] = ' ';
      ss[0] = '\n';
    }
    printf("%s(", ss);
    for (int j = 0; j < m; j++){
      int s = (int) gsl_spmatrix_get(g->o, 0, j + i*m);
      int ss = (int) gsl_spmatrix_get(g->o, 1, j + i*m);
      if (s > ss){
	printf("\033[0;31m");
      }
      if (s < ss){
	printf("\033[0;32m");
      }
      printf(" o ");
      printf("\033[0m");
    }
    printf(")");
  }
  printf("\n");
  free(s);
}

void assign_links(gsl_spmatrix* t, char c, int n){//initialise la matrice t pour un graphe no triangulaire
  for (int i = 0; i < n; i++){
    for (int j = 0; j < n; j++){
      int N = i*n + j;
      if (i > 0){
	gsl_spmatrix_set(t, N, N-n, 1);//carré, hex
      }
      if (i < n - 1){
	gsl_spmatrix_set(t, N, N+n, 1);//carré, hex
      }
      if (j > 0){
	gsl_spmatrix_set(t, N, N-1, 1);//carré, hex
      }
      if (j < n - 1){
	gsl_spmatrix_set(t, N, N+1, 1);//carré, hex      
      }
      if (i < n - 1 && c == 'h' && j > 0){
	gsl_spmatrix_set(t, N, N + n - 1, 1);//hex
      }
      if (i > 0 && c == 'h' && j < n-1){
	gsl_spmatrix_set(t, N, N - n + 1, 1);//hex
      }
    }
  }
}

void assign_tr(gsl_spmatrix* t, char c, int n){//initialise la matrice t pour un graphe triangulaire
  int i;
  int ligne = 0;
  int nb_elem = 2*n + 1;//décroit le long d'une ligne
  int nb_elem2 = 2*n + 1;//constant le long d'une ligne
  int checkp = !(n==1) * 1;
  int inc = 2;
  for (i = 0; i < 6*n*n; i++){
    //    printf("%d, %d, %d, %d\n", ligne, nb_elem, i, checkm);
    if (nb_elem != nb_elem2){
      gsl_spmatrix_set(t, i, i-1, 1);
    }

    if (nb_elem > 1){
      gsl_spmatrix_set(t, i, i+1, 1);
    }


    if (((((i % 2) == (ligne % 2)) && (i < 3*n*n))
	 || (((i % 2) != (ligne % 2)) && (i >= 3*n*n)))
	&& (ligne < 2*n - 1)){      	
      gsl_spmatrix_set(t, i, i + nb_elem2 - (i >= 3*n*n), 1);
    }

    
    if (((((i % 2) != (ligne % 2)) && (i < 3*n*n))
	 || (((i % 2) == (ligne % 2)) && (i >= 3*n*n)))
	&& (ligne > 0)){
      gsl_spmatrix_set(t, i, i - nb_elem2 + (i < 3*n*n), 1);
    }

    
    nb_elem--;
    if (nb_elem == 0){
      ligne++;
      if (ligne == n/2){
	checkp = 0;
	inc = -2;
	nb_elem2 += 2;
      }
      else{
	if (checkp == 0){
	  checkp = 1;
	}
	else{
	  nb_elem2 += inc;
	}
      }
      nb_elem = nb_elem2;
    }
  }
}

void assign_colors(gsl_spmatrix* o, int n){
  int m = n;
  for (int i = 0; i < m-1; i++){
    gsl_spmatrix_set(o, 0, i, 2);
    gsl_spmatrix_set(o, 1, m*(i+1), 2);
    gsl_spmatrix_set(o, 1, m*(i+1)-1, 2);
    gsl_spmatrix_set(o, 0, n*n - i - 1, 2);
  }
}

