#include<ncurses.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>



typedef struct{
  int x;
  int y;
}Cord;

typedef struct {
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
// mines:
// -1 if mine
// else amount of near mines
// select:
// 2 if marked as mine
// else if unveiled 1
// else -1 (or 0)

void init(Game* g, int width, int height, int  minesamount ){
  int length = width * height;
  int total =0;
  int* mines = malloc(length*sizeof(int));
  int* select = malloc(length*sizeof(int));
  int* floating = malloc(length*sizeof(int));
  double minesp = (double) minesamount / (double) length;
  for(int i=0;i<length;i++){
//    if(total >= minesamount) //TODO Verteilung nicht zuverlässig
//	    break;
    mines[i]  = 0;
    select[i] = 0;
    floating[i]=0;
    double rand =  drand48();
    if(rand < minesp){
      // is mine
      mines[i] = -1;
      total += 1;
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
}

void debug(Game* g){
  
  int length = g->length;
  int i = 0;
  for(int i=0;i<g->height;i++){
    for(int j=0;j<g->width;j++){
      printw("%d", g->mines[i*g->width+j]);
    }
    
    printw("\n");
  }
}

void print(Game* g){
  init_pair(1,COLOR_BLUE,COLOR_BLACK);
  init_pair(2,COLOR_GREEN,COLOR_BLACK);
  init_pair(3,COLOR_RED,COLOR_BLACK);
  init_pair(4,COLOR_MAGENTA,COLOR_BLACK);
  init_pair(5,COLOR_YELLOW,COLOR_BLACK);
  init_pair(6,COLOR_CYAN,COLOR_BLACK);
  int length = g->length;
  printw("Flags: %d\n", g->flagstotal - g->flagsfound);
  printw(" ");
  for(int i = 0;i<g->width;i++){
    printw("_");
  }	
  printw("\n");
  for(int i=0;i<g->height;i++){
    printw("|");
    for(int j=0;j<g->width;j++){
      if( i== g->cord.y&&j==g->cord.x)
	attron(A_REVERSE);		
      if(g->select[i*g->width+j]==2){
	printw("P");
      }else if(g->select[i*g->width+j]==1&&g->mines[i*g->width+j]==0){
	printw(" ");	
      }else if(g->mines[i*g->width+j]==0){
	printw("#");
      }else if(g->select[i*g->width+j]==1){
	switch(g->mines[i*g->width+j]){
	case 1:  attron(COLOR_PAIR(1));
	  break;
	case 2: attron(COLOR_PAIR(2));
	  break;
	case 3: attron(COLOR_PAIR(3));
	  break;
	case 4: attron(COLOR_PAIR(4));
	  break;
	case 5: attron(COLOR_PAIR(5));
	  break;
	case 6: attron(COLOR_PAIR(6));
	  break;
	  
	}
	printw("%d", g->mines[i*g->width+j]);
	attroff(COLOR_PAIRS);
      }else{
	printw("#");
      }
      if( i== g->cord.y&&j==g->cord.x)
	attroff(A_REVERSE);  
    }
    printw("|");
    printw("\n");
  }
  printw("|");
  for(int i = 0;i<g->width;i++){
    printw("=");
  }	
  printw("|\n");
}

// controls
int  cmove(Game *g){
  char ch = getch();
  switch(ch){
  case 'j': g->cord.y += 1;
    if(g->cord.y==g->height)
      g->cord.y=0;
    break;
  case 'k': g->cord.y -= 1;
    if(g->cord.y==-1)
      g->cord.y=g->height -1;
    break;
  case 'h': g->cord.x -=1;
    if(g->cord.x==-1)
      g->cord.x=g->width-1;
    break;
  case 'l': g->cord.x +=1;
    if(g->cord.x==g->width)
      g->cord.x=0;
    break;
  case 'q': return -1;
  case '$': g->cord.x = g->width -1;
    break;
  case '0': g->cord.x=0;
    break;
  case 'g': g->cord.y=0;
    break;
  case 'G': g->cord.y= g->height -1;
    break;
  case 'f': return -2;
  case 's': return -3;
  }
  return 0;
}

void flag(Game *g){
  if(g->select[g->cord.y*g->width+g->cord.x]== 2){
    g->select[g->cord.y*g->width+g->cord.x]=0;
    g->flagsfound -=1;
  }else{
    g->select[g->cord.y*g->width+g->cord.x]=2;
    g->flagsfound +=1;
  }  
}

// returns -1 if steped on mine
// else if not flagged  sets select 1 

int unveil(Game *g, int x, int y, int iteration){
	if(g->mines[y*g->width+x]>=0){
		if(g->select[y*g->width+x]!=2&&g->select[y*g->width+x]!=1)
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

int  test(Game *g){
   if(g->mines[g->cord.y*g->width+g->cord.x]==-1){
   	return -1;
   }
   if(g->mines[g->cord.y*g->width+g->cord.x]>0){
	   g->select[g->cord.y*g->width+g->cord.x]=1;
	   return 0;
   }
   if(g->select[g->cord.y*g->width+g->cord.x]==2){
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




int main(int argc, char *argv[]){
  initscr();
  int wt,hi,ch,mi, fd;
  wt=hi=mi=0;
  const char* errstr;
  
  while((ch=getopt(argc,argv,"h:w:m:"))!=-1){
    switch(ch){
    case 'h':
      hi=strtonum(optarg,0,250,&errstr);
      break;
    case 'w':
      wt=strtonum(optarg,0,250,&errstr);
      break;
    case 'm':
      mi=strtonum(optarg,0,250,&errstr);
      break;
      //default: usaage();
      argc -= optind;
      argv += optind;
      
      
      
    }
  }
  if(wt==0)
    wt=60;
  if(hi==0)
    hi=30;
  if(mi==0)
    mi=60;
  start_color();
  Game game;
  init(&game, wt, hi, mi);
  while(1){
    clear();
    print(&game);
    int ret = cmove(&game);
    if(ret  == -1){ 
      break ;
    }else if(ret == -2){
      flag(&game);
    }else if(ret==-3){
      if(test(&game)==-1){
	clear();
	printw("you lost\n");
	debug(&game);
	break;
      }
    }
    if(checkflags(&game)){
      clear();
      printw("You found all %d mines!", game.flagstotal);
      break;
    }
    
  }
  
  getch();
  endwin();	
  return 0;
}
