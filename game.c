#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include <time.h>
#include<limits.h>
#include "game.h"
typedef struct myGame {
  int* mines;
  int* select;
  int* floating;
  int width;
  int height;
  int length;
  int flagstotal;
  int flagsfound;
  int flagsfit;
  Cord cord;
}Game;

Game* g_new(int width, int height, int  minesamount ){
  Game game;
  Game* g = malloc(sizeof(Game));
  *g = game; 
  int length = width * height;
  int total =0;
  int* mines = malloc(length*sizeof(int));
  int* select = malloc(length*sizeof(int));
  int* floating = malloc(length*sizeof(int));
  double minesp = (double) minesamount / (double) length;
  srand48(time(0));
  for(int i=0;i<length;i++){
    mines[i]  = 0;
    select[i] = 0;
    floating[i]=0;
    double rand =  drand48();
    if(rand < minesp){
  	  if(total==minesamount)
  		  continue;
      mines[i] = -1;
      total += 1;
    }
  }
  while(total<minesamount){
  for(int i=0;i<length;i++){
    double rand =  drand48();
    if(rand < minesp && mines[i]!=-1){
  	  if(total==minesamount)
  		  continue;
      mines[i] = -1;
      total += 1;
    }
  } 
  }
 //compute amount of near mines 
  for(int i=0;i<height;i++){
    for(int j=0;j<width;j++){
      if(mines[i*width+j] != -1){
	int mcount = 0;
	for(int k = i-1; k <= i+1;k++){
	  for(int l = j-1; l<= j+1;l++){
	    if(k>=0&&k<height&&l>=0&&l<width){
	      if(mines[k*width+l]==-1){
		mcount ++;
	      }
	    }
	  }
	}
	mines[i*width+j]=mcount;
      }
    }
  }
  
  g->length=length;
  g->mines=mines;
  g->floating=floating;
  g->select=select;
  g->width=width;
  g->height=height;
  g->flagstotal=total;
  g->flagsfound=0;
  g->flagsfit=0;
  Cord c;
  c.x=0;
  c.y=0;
  g->cord=c;
  return g;
}


void g_kill(Game *g){
	free(g->mines);
	free(g->select);
	free(g->floating);
	free(g);
}

int get_x(Game* g,int* L, int x, int y){
	return L[y*g->width+x];
}
//void set_x(int* L,int x, int y, int v){
//	L[y*g->height+x]=v;
//}

char g_mine(Game* g,int x, int y){
	if(get_x(g,g->mines,x,y)==-1)
		return 1;
	return 0;
}
char g_plain(Game* g, int x, int y){
	if(get_x(g,g->mines,x,y)==0){
		return 1;
	}
	return 0;
}

int amount_near_mines(Game *g,int x, int y){
	if(get_x(g,g->mines,x,y)>=1){
		return get_x(g,g->mines,x,y);
	}
	return 0;
}
Cord g_player_position(Game *g){
	return g->cord;
}
void g_set_player_position_x(Game *g, int x){
	g->cord.x=x;
}
void g_set_player_position_y(Game *g,int y){
	g->cord.y=y;
}
char g_unveiled(Game *g, int x, int y){
	if(get_x(g,g->select,x,y)==1){
		return 1;	
	}
	return 0;
}
char g_flaged(Game *g,int x,int y){
	if(get_x(g,g->select,x,y)==2){
		return 1;	
	}
	return 0;
}

int g_flags_total(Game *g){
	return g->flagstotal;
}
int g_flags_found(Game *g){
	return g->flagsfound;
}
int g_width(Game *g){
	return g-> width;
}
int g_height(Game *g){
	return g->height;
}

//TODO refactor me
void flag(Game *g){
  if(g->select[g->cord.y*g->width+g->cord.x]== 2){
    g->select[g->cord.y*g->width+g->cord.x]=0;
    g->flagsfound -=1;
  }else{
    g->select[g->cord.y*g->width+g->cord.x]=2;
    g->flagsfound +=1;
  }  
}

//TODO refactor me
int unveil(Game *g, int x, int y, int iteration){
	if(g->mines[y*g->width+x]>=0){
		if(g->select[y*g->width+x]!=2)
			g->select[y*g->width+x]=1;
		if(g->mines[y*g->width+x]==0){
			if(g->floating[y*g->width+x]==0){
				g->floating[y*g->width+x]=iteration+1;
				return 1;	
			}
		}
	}
	return 0;
}

//TODO refactor me
int floating_unveil(Game *g, int iteration){
	int amoun_unveiled =0;
	for(int i=0;i<g->height;i++){
    		for(int j=0;j<g->width;j++){
			if(g->floating[i*g->width+j]==iteration){
				for(int k=i-1;k<=i+1;k++){
					for(int l=j-1;l<=j+1;l++){
						if(l>=0&&k>=0&&k<g->height&&l<g->width){
							amoun_unveiled += unveil(g,l,k, iteration);
						}
					}
					
				}	
			}	
    		}
    
  	}
	return amoun_unveiled;

}

//TODO refactor me
int  test(Game *g){
   if(g->select[g->cord.y*g->width+g->cord.x]==2){
	   return 0;
   }
   if(g->mines[g->cord.y*g->width+g->cord.x]==-1){
   	return -1;
   }
   if(g->mines[g->cord.y*g->width+g->cord.x]>0){
	   g->select[g->cord.y*g->width+g->cord.x]=1;
	   return 0;
   }
   g->floating[g->cord.y*g->width+g->cord.x]=1;
   int ui = 1;
   int tmp =floating_unveil(g,ui);
   while(tmp > 0){
   	tmp =0;
	ui ++;
	tmp =floating_unveil(g,ui);
   }
   for(int i=0; i<g->length;i++){
  	g->floating[i]=0; 
   } 
   

  return 0;
}
//TODO refactor
int checkflags(Game *g){
  int fit = 0;
  for(int i =0; i<g->length; i++){
    if(g->mines[i] == -1 && g->select[i]==2)
      fit +=1;
  }
  if(fit == g->flagstotal)
    return 1;
  return 0;
}
