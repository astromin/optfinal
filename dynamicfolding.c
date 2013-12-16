/* Amanda Strominger
 * Optimization final Fall 2013
 * Finds the maximum base pairing on a given sequence of RNA
 */

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>


//finds the actual pairs that bind in the optimal solution
//writes to soln
void findsol(FILE *soln, int **sol, int i, int j){
	if (i < j){
		if (sol[i][j] > 0){
			fprintf(soln, "%d, %d\n", sol[i][j], j);
			findsol(soln, sol, (sol[i][j]+1), (j-1));
			findsol(soln, sol, i, (sol[i][j]-1));
		}
		else{ //j is not paired with anything
			findsol(soln, sol, i, j-1);
		}
	}

}

int main(int argc, char *argv[]){
    FILE *sequence, *soln;
    if(argc < 4){
        fprintf(stderr, "Don't forget command line arguments! \n Should be input file, sequence length, output file.\n");
        exit(EXIT_FAILURE);
    }
	//sequence is the file containing the sequence of bases
    if (NULL == (sequence = fopen(argv[1], "r"))){
        fprintf(stderr, "Could not open sequence");
        exit(EXIT_FAILURE);
    }
	//soln will be a file containing a list of all bound base pairs
    if (NULL == (soln = fopen(argv[3], "w"))){
        fprintf(stderr, "Could not open foldingsolution");
        exit(EXIT_FAILURE);
    }
	int seqlen = atoi(argv[2]);
    int seq[seqlen+1];//an array holding the sequence
    seq[0]=0; //we will start our indexing at 1
	//sol holds the solution, sol[i][j] = the base pair j is bound to in the optimal solution on the interval (i, j)
	//0 if j is not bound to anything
    int **sol = malloc((seqlen+1)*sizeof(int *));
	for (int l = 0; l < seqlen+1; l++){
		sol[l]= malloc((seqlen+1)*sizeof(int));
	} 
    int opt[seqlen+1][seqlen+1];
    int i,j,k;
    for (i = 0; i<seqlen+1; i++){
        for (j=0; j<seqlen+1; j++){
            opt[i][j]=sol[i][j]=0;
        }
    }
    int ch;
    int c = 1;
    while((ch = fgetc(sequence))!=EOF){
        seq[c]=ch;
        c++;
    }
	//check sequence length
	if ((c-1)!=seqlen){
		fprintf(stderror, "You entered an incorrect sequence length.\n");
		exit(EXIT_FAILURE);
	}
    for (k=5; k<seqlen; k++){
        for(i = 1; i<= seqlen-k; i++){
            j = i+k;
            int max = opt[i][j-1];
            int s = 0;
			if(k==5 && (seq[j]+seq[i] == 'C' + 'G' || seq[j]+seq[i] == 'A' + 'U')){ 
				max = 1;
				s = i;
			}
            for (int t = i; t<j-4; t++){
				if (seq[j]+seq[t] == 'C' + 'G' || seq[j]+seq[t] == 'A' + 'U'){
	                int temp = (1 + opt[i][t-1] + opt[t+1][j-1]);
    	            if (temp > max){
	                    max = temp;
    	                s = t;
        	        }
				}
            }
            opt[i][j] = max;
            sol[i][j] = s;
        }
    }
	findsol(soln, sol, 1, seqlen);
	fclose(soln);
    fclose(sequence);
}
