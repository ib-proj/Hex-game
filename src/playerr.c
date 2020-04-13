#include "player.h"

struct player{
  
  char* name;
  struct graph_t *graph;
  enum color_t color;
  struct move_t (*player_propose_opening)();
  int (*player_accept_opening)(const struct move_t opening);
  void (*player_initialize_color)(enum color_t id);
  void (*player_initialize_graph)(struct graph_t* graph);
  struct move_t (*player_play)(struct move_t previous_move);
  void (*finalize)();
  
};

void initialize_player_functions(struct player* player){
  player->player_propose_opening = propose_opening;
  player->player_accept_opening = accept_opening;
  player->player_initialize_graph = initialize_graph;
  player->player_initialize_color = initialize_color;
  player->player_play = play;
}

struct player jeremy = {.name ="Xx_JérémY_xX"};

struct player get_player(){  
  initialize_player_functions(&jeremy);
  jeremy.name = "player1";
  return jeremy;  
}


void initialize_color(enum color_t id){  
  jeremy.color = id;
}


void initialize_graph(struct graph_t* graph){
    jeremy.graph = graph;   
}

//struct move_t play(struct move_t previous_move){


int accept_opening(const struct move_t opening){
  if(0){
    printf("%zu\n", opening.m);
  }
  return 1;
}

void finalize(){
  free__graph_t(jeremy.graph);
}


struct move_t propose_opening(){//temporaire
  struct move opening;
  opening.c = jeremy.color;
  move.m = size__graph_t(jeremy.graph) / 2;
  return opening;
}
