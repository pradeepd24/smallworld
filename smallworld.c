/* I use a data structure called kd trees. Using this helps us to prune the search and not search all points to find the closest point */
#include <stdio.h>
#include <stdlib.h>
/* structure of a particular point in 2D cartesian co-ordinates */
struct item {
	double x; // x co-ordiante
	double y; // y co-ordinate
	int n;    // point number from the input file
};

/* node in the kd tree */
struct node {
	struct item a;
	struct node* left;
	struct node* right;
}*root;

/* inserting into kd tree */
void insert(struct item a) {
	struct node *temp = (struct node*)malloc(sizeof(struct node));
	struct node *temp1;
	int count = 0;
	temp->a = a;
	temp->left = NULL;
	temp->right = NULL;
	if(root == NULL) { 
		root = temp;
		return;
	}
 	temp1 = root;

	/* we need to insert temp at the right location in the tree using the while loop below */
	while(1) {
  		if(count %2 == 0) { /* compare x co-ordinates */
	  		if(temp1->a.x > temp->a.x) { 
				//add to left side
                		if(temp1->left == NULL) {
					temp1->left = temp;
					return;
				}
                		temp1 = temp1->left;
                		count++;
				continue;
			}
			else {
				// add to right side
                		if(temp1->right == NULL) {
					temp1->right = temp;
					return;
				}
                		temp1 = temp1->right;
                		count++;
                		continue;
 	  		}
  		}
  		else { /* compare y co-ordinates */
			if(temp1->a.y > temp->a.y) {
                 		//add to left side
				if(temp1->left == NULL) {
					temp1->left = temp;
					return;
				}
                		temp1 = temp1->left;
                		count++;
                		continue;
          		}
          		else {
                 		// add to right side
				if(temp1->right == NULL) {
					temp1->right = temp;
					return;
				}
                		temp1 = temp1->right;
                		count++;
                		continue;

			}
		}
	}/*end of while */
} /*end of insert function */

/* to store the 3 closest neighbors for a point */
struct item1 {
	int neighbor;
	double dist_sq;
	int valid; /* if this is 0, then still a neighbor has not been found */
}n[3];

/* for the four functions below, assume we have a square formed with a and neigh1 as the 2 diagonally opposite vertices */
/* this functions returns true if sear is within that square */
int within_sq(struct item a,struct item1 neigh,struct node *sear) {
	double dist_x,dist_y;
	dist_x = (sear->a.x-a.x)*(sear->a.x-a.x);
	dist_y = (sear->a.y-a.y)*(sear->a.y-a.y);
 
	if(dist_x < neigh.dist_sq && dist_y < neigh.dist_sq)
		return 1;
	else
		return 0;
}

/* this function reurns true if sear is to the left of the square */
int isleft_sq(struct item a,struct item1 neigh,struct node *sear) {
	double dist_x;
	dist_x = (sear->a.x-a.x)*(sear->a.x-a.x);
	if(dist_x >= neigh.dist_sq && sear->a.x < a.x)
		return 1;
	else
		return 0;
}

/* this function returns true if sear is to the right of the square */
int isright_sq(struct item a,struct item1 neigh,struct node *sear) {
	double dist_x;
	dist_x = (sear->a.x-a.x)*(sear->a.x-a.x);
	if(dist_x >= neigh.dist_sq && sear->a.x > a.x)
		return 1;
	else
		return 0;
}

/* this function returns true if sear is below the square */
int isdown_sq(struct item a,struct item1 neigh,struct node *sear) {
	double dist_y;
	dist_y = (sear->a.y-a.y)*(sear->a.y-a.y);
	if(dist_y >= neigh.dist_sq && sear->a.y < a.y)
		return 1;
	else
		return 0;
}

/* this function returns true if sear is above the square */
int isup_sq(struct item a,struct item1 neigh,struct node *sear) {
	double dist_y;
	dist_y = (sear->a.y-a.y)*(sear->a.y-a.y);
	if(dist_y >= neigh.dist_sq && sear->a.y > a.y)
		return 1;
	else
		return 0;
}

/* this function searches the 3 neighbors of a and puts them in neigh */
void search_3_neighbor(struct item a, struct item1 *neigh, struct node *sear, int level) {
	double dist_sq;
	if(sear == NULL)
		return;
 
	/* just wrote down all different cases in within if else blocks */
	/* the first few cases are when no neighbor is found, 1 neighbour found and 2 neighbor found */
	if(neigh[0].valid == 0) {
		if(a.n != sear->a.n) { 
			neigh[0].valid = 1;
			neigh[0].neighbor = sear->a.n;
			neigh[0].dist_sq = (a.x-sear->a.x)*(a.x-sear->a.x) + (a.y-sear->a.y)*(a.y-sear->a.y);
		}
		search_3_neighbor(a,neigh,sear->left,level+1);
		search_3_neighbor(a,neigh,sear->right,level+1);
		return;
	}
	else if(neigh[1].valid == 0) {
		if(a.n != sear->a.n) {
			dist_sq = (sear->a.x-a.x)*(sear->a.x-a.x) + (sear->a.y-a.y)*(sear->a.y-a.y);
			if(dist_sq < neigh[0].dist_sq) {
				neigh[1] = neigh[0];
				neigh[0].dist_sq = dist_sq;
				neigh[0].neighbor = sear->a.n;
			}
			else { 
				neigh[1].valid = 1;
				neigh[1].neighbor = sear->a.n;
				neigh[1].dist_sq = dist_sq;
			}
		}
		search_3_neighbor(a,neigh,sear->left,level+1);
		search_3_neighbor(a,neigh,sear->right,level+1);
		return;
	}
	else if(neigh[2].valid == 0) {
		if(a.n != sear->a.n) {
			dist_sq = (sear->a.x-a.x)*(sear->a.x-a.x) + (sear->a.y-a.y)*(sear->a.y-a.y);
			if(dist_sq < neigh[0].dist_sq) {
				neigh[2] = neigh[1];
				neigh[1] = neigh[0];
				neigh[0].dist_sq = dist_sq;
				neigh[0].neighbor = sear->a.n;
			}
			else if(dist_sq < neigh[1].dist_sq) {
				neigh[2] = neigh[1];
				neigh[1].dist_sq = dist_sq;
				neigh[1].neighbor = sear->a.n;
			}
			else {
				neigh[2].valid = 1;
				neigh[2].neighbor = sear->a.n;
				neigh[2].dist_sq = dist_sq;
			}
		}
		search_3_neighbor(a,neigh,sear->left,level+1);
		search_3_neighbor(a,neigh,sear->right,level+1);
		return;
	}
	else { // these are the cases where all three neighbors are there and we have to check if the current point is closer than these neighbors and which path to search further
		if(a.n == sear->a.n || neigh[0].neighbor == sear->a.n || neigh[1].neighbor == sear->a.n || neigh[2].neighbor == sear->a.n) {
			search_3_neighbor(a,neigh,sear->left,level+1);
			search_3_neighbor(a,neigh,sear->right,level+1);
			return;
		}
		else {
			dist_sq = (a.x-sear->a.x)*(a.x-sear->a.x) + (a.y-sear->a.y)*(a.y-sear->a.y);
			if(dist_sq < neigh[0].dist_sq) {
				neigh[2] = neigh[1];
				neigh[1] = neigh[0];
				neigh[0].dist_sq = dist_sq;
				neigh[0].neighbor = sear->a.n;
			}
			else if(dist_sq < neigh[1].dist_sq) {
				neigh[2] = neigh[1];
				neigh[1].dist_sq = dist_sq;
				neigh[1].neighbor = sear->a.n;
			}
			else if(dist_sq < neigh[2].dist_sq) {
				neigh[2].dist_sq = dist_sq;
				neigh[2].neighbor = sear->a.n;
			}
			else { 
				/* do nothing */
			}
    
			if(within_sq(a,neigh[2],sear)) {
				search_3_neighbor(a,neigh,sear->left,level+1);
				search_3_neighbor(a,neigh,sear->right,level+1);
				return;
			}
			else if(level%2 == 0) {
				if(isleft_sq(a,neigh[2],sear)) { /* these are the cases where we skip searching one part of the tree. We are able to do this because of the structure of the kd tree */
					search_3_neighbor(a,neigh,sear->right,level+1);
					return;
				} 
				else if(isright_sq(a,neigh[2],sear)) { 
					search_3_neighbor(a,neigh,sear->left,level+1);
					return;
				}
				else { 
					search_3_neighbor(a,neigh,sear->left,level+1);
					search_3_neighbor(a,neigh,sear->right,level+1);
					return;
				}
			}
			else {
				if(isdown_sq(a,neigh[2],sear)) { /* these are the cases where we skip searching one part of the tree. We are able to do this because of the structure of the kd tree */
					search_3_neighbor(a,neigh,sear->right,level+1); 
					return;
				} 
				else if(isup_sq(a,neigh[2],sear)) {
					search_3_neighbor(a,neigh,sear->left,level+1);
					return;
				}
				else {
					search_3_neighbor(a,neigh,sear->left,level+1);
					search_3_neighbor(a,neigh,sear->right,level+1);
					return;
				}
			}
		}
	}
} /*end of search_3_neighbor function */


int main(int argc, char *argv[]) {
	FILE *fp;
	unsigned int N=0;
	struct item a;
	struct item *S,*S1;
	unsigned int i = 0, r;

	/* initially no neighbor is found */
	n[0].valid = 0;
	n[1].valid = 0;
	n[2].valid = 0;

	root = NULL;
	if(argc < 2) {
		printf("Please specify an input filename in the command line.\n");
		return 0;
	}

	fp = fopen(argv[1],"r");
	while(fscanf(fp,"%d %lf %lf",&a.n,&a.x,&a.y) != EOF ) {
		N++;
	}

	fclose(fp);

	S = (struct item*)malloc(sizeof(struct item)*N);// to store the points
	S1 = (struct item*)malloc(sizeof(struct item)*N); // to store the points
	fp = fopen(argv[1],"r");
	while(fscanf(fp,"%d %lf %lf",&a.n,&a.x,&a.y) != EOF ) {
		S[i] = a;
		S1[i] = a;
		i++;
	}

	/* randomly storing the points in the kd tree. Randomization is done to avoid the worst case structure in kd tree, just in case */
	for(i=0; i < N; i++) {
		r = rand()%(N-i);
		insert(S[r]);
		S[r] = S[N-i-1]; 
	}
	free(S);

	for(i=0; i < N; i++) {
		search_3_neighbor(S1[i],n,root,0); // we search the three neighbors if S1[i] and store it in the array n 
		printf("%d %d,%d,%d\n",S1[i].n,n[0].neighbor,n[1].neighbor,n[2].neighbor);
		/* for every new point, we have to make the following 0s to indicate that we are just starting to find neighbor */
		n[0].valid = 0;
		n[1].valid = 0;
		n[2].valid = 0;
	}
	free(S1);
	fclose(fp);
	return 0;

}/*end of main */


