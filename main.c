#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define RANDBTW(a,b) (rand()%((b)-(a)+1)+(a))
#define ARGCMP(a,b,c) ( strcmp((a),(b)) == 0 || strcmp((a),(c)) == 0)
#define OUTPUT(x,...) //printf(x,##_VA_ARGS_)	//for debug
#define USAGE_TEXT "usage: %s ([-l|--length] <length>) ([-w|--width] <width>)\n" \
				"([--genmin] <genmin>)([--genmax] <genmax>])\n" \
				"%s [-h|--help]" \
				, argv[0], argv[0]

int length = 4, width = 4;//length==x, width==y
int score = 0, step = 0;
int** map;

static inline void end(){
	printf("\nscore: %d\nstep: %d\n",score,step);
	while(length--) free(map[length]);
	free(map);
	exit(0);
}

static inline void output(){
	printf("\n\n\n\nscore: %d\nstep: %d\n----------\n",score,step);
	for(int i=0; i<width; i++){
	for(int j=0; j<length; j++){
		printf(map[j][i]==0?"      |":" %4d |",map[j][i]);
	}
	printf("\n");
	}
}

void generate(int genmin, int genmax){
	int i=RANDBTW(genmin, genmax);
	for(bool genflag=false; i>0; i--){
		OUTPUT("%d|",i);
		int count=0, *place = NULL;
		for(int j=0; j<width; j++){
		for(int k=0; k<length; k++){
			if(map[k][j] == 0 && (rand() % ++count == 0)) {
				place=&map[k][j];
				genflag = true;
				OUTPUT("_%d_", map[k][j]);
			}
			else OUTPUT("%d ",map[k][j]);
		}}
		OUTPUT("---\n");
		if(NULL == place){
			if(genflag) { break; }
			printf("You lose!%d",count);
			end();
		}
		*place = 2 * (rand()>RAND_MAX/10?1:2);//2:90% 4:10%
	}
	bool moveflag = false;
	for(int x=0; x<length; x++) {
	for(int y=0; y<width; y++) {
		moveflag = moveflag || ( x!=length-1 && map[x][y]==map[x+1][y] );
		moveflag = moveflag || ( y!=width-1 && map[x][y]==map[x][y+1]);
		if(moveflag) { break; }// faster
	}}
	if(!moveflag) {
		printf("You lose!");
		end();
	}
}

bool move(char ch){
	bool dx=false,d=true;//dx:xmove,d:+ 
	switch (ch) {
	case 'w':
		dx=false,d=false;
		break;
	case 'a':
		dx=true,d=false;
		break;
	case 's':
		dx=false,d=true;
		break;
	case 'd':
		dx=true,d=true;
		break;
	case 'q':
		end();
		//break;
	default:return false;
		//break;
	}
	int dmax=(dx?length:width)-1,line=(dx?width:length),start,end;
	int *xs=(dx?&start:&line),*ys=(dx?&line:&start),*xe=(dx?&end:&line),*ye=(dx?&line:&end);
	bool moved=false;
	while(line--){
		start=(d?dmax-1:1);
		end=(d?dmax:0);
		while(start!=-1 && start!=dmax+1){
			if(map[*xs][*ys]==0){
				start+=(d?-1:1);
			}else if(map[*xe][*ye]==0){
				map[*xe][*ye]=map[*xs][*ys];
				map[*xs][*ys]=0;
				start+=(d?-1:1);
				moved=true;
			}else if(map[*xs][*ys]==map[*xe][*ye]){
				map[*xe][*ye]+=map[*xs][*ys];
				score+=map[*xe][*ye];
				map[*xs][*ys]=0;
				start+=(d?-1:1);
				end+=(d?-1:1);
				moved=true;
			}else{
				end+=(d?-1:1);
				if(end==start)start+=(d?-1:1);
			}
		}
	}
	if(moved)step++;
	return moved;
}


int main(int argc, char** argv){
	srand((unsigned)time(NULL));

	//if (argc == 3) sscanf(argv[1], "%d", &length), sscanf(argv[2], "%d", &width);
	int genmin = 1, genmax = 1;
	for(int i=1; i<argc; i++){
		if (ARGCMP(argv[i], "-l", "--length")) {
			sscanf(argv[++i], "%d", &length);
		} else if (ARGCMP(argv[i], "-w", "--width")) {
			sscanf(argv[++i], "%d", &width);
		} else if (ARGCMP(argv[i], "--genmin", "--genmin")) {
			sscanf(argv[++i], "%d", &genmin);
		} else if (ARGCMP(argv[i], "--genmax", "--genmax")) {
			sscanf(argv[++i], "%d", &genmax);
		} else if (ARGCMP(argv[i], "-h", "--help")) {
			printf(USAGE_TEXT);
			return 0;
		} else {
			printf("no match: %s\n", argv[i]);
			printf(USAGE_TEXT);
			return 0;
		}
	}

	map = malloc(sizeof(int*)*length);
	for(int i=0; i<length; i++){
		map[i] = calloc(sizeof(int), width);
	}
	for(;;){
		generate(genmin, genmax);
		output();
		while(!move(getchar()));
	}
}
