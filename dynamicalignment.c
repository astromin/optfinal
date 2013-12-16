/* Amanda Strominger
 * Optimization final project Fall 2013
 * Reads in two sequences and finds the best alignment given gap pentalty delta and mismatch penalty alpha
 * Would like to alter output file to be the two sequences with spaces where gaps are in the optimal solution
 * I would also like to allow different values of alpha for different mismatches
 */
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>

int min(int a, int b, int c);
void findsol(FILE *soln, int **sol, int i, int j);


//finds the actual pairing for a sequence alignment problem
void findsol(FILE *soln, int **sol, int i, int j){
	printf("%d %d\n", i, j);
	if (i < 1 || j < 1)
		return;
	if (sol[i][j] == 2){ //i and j are matched
		fprintf(soln, "%d, %d\n", i, j);
		findsol(soln, sol, i-1, j-1);
	}
	else if (sol[i][j] == 1){ //j matched to a gap
		findsol(soln, sol, i, j-1);
	}
	else{ //i matched to a gap
		findsol(soln, sol, i-1, j);
	}
}


//returns the minimum value of three integers, arbitrarily breaks ties
int min(int a, int b, int c){
	if (a < b){
		if (a < c) //a<b, a<c
			return a;
		else //c < a < b
			return c;
	}
	else { //b < a
		if (b<c)
			return b; //b < a, b<c
		else //c < b < a
			return c;
	}
}

int main(int argc, char *argv[]){
	FILE *sequence, *soln;
	//check cmdline args
	if(argc < 7){
		fprintf(stderr, "Don't forget command line arguments! \n Should be input file, seqlen1, seqlen2, alpha, delta, output file. \n");
		exit(EXIT_FAILURE);
	}
	//open file
	if (NULL == (sequence = fopen(argv[1], "r"))){
		fprintf(stderr, "Could not open input file \n");
		exit(EXIT_FAILURE);
	}	//open file
	if (NULL == (soln = fopen(argv[6], "w"))){
		fprintf(stderr, "Could not open output file \n");
		exit(EXIT_FAILURE);
	}
	int seqlen1 = atoi(argv[2]);
	int seqlen2 = atoi(argv[3]);
	int alpha = atoi(argv[4]);
	int delta = atoi(argv[5]);
	int seq1[seqlen1+1];
	int seq2[seqlen2+1];
	seq1[0] = seq2[0] = '\0'; //index bases starting at 1
	int ch, c=1;
	//read in first sequence
	while ((ch = fgetc(sequence))!='\n'){
		seq1[c] = ch;
		c++;
	}
	//check sequence length
	if ((c-1) != seqlen1){
		fprintf(stderr, "You entered an incorrect sequence length.\n");
		exit(EXIT_FAILURE);
	}
	c = 1;
	//read in second sequence
	while ((ch = fgetc(sequence))!=EOF){
		seq2[c] = ch;
		c++;
	}
	//check second sequence length
	if ((c-1) != seqlen1){
		fprintf(stderr, "You entered an incorrect sequence length.\n");
		exit(EXIT_FAILURE);
	}
	//array of ints that encodes the actual matching
	//if sol[i][j] == 2, i and j are matched in the optimal solution on i and j
	//if sol[i][j] == 1, j is matched to a gap
	//if sol[i][j] == 0, i is matched to a gap
    int **sol = malloc((seqlen1+1)*sizeof(int *));
    for (int l = 0; l < seqlen1+1; l++){
        sol[l]= malloc((seqlen2+1)*sizeof(int));
    } 
    int opt[seqlen1+1][seqlen2+1];
    int i,j;
    for (i = 0; i<seqlen1+1; i++){
        for (j=0; j<seqlen2+1; j++){
			if (i == 0)
				opt[i][j] = j*delta;
			else if (j == 0)
				opt[i][j] = i*delta;
			else
            	opt[i][j]=0;
			sol[i][j]=0;
        }
    }
	for (i =1; i<seqlen1+1; i++){
		for (j = 1; j<seqlen2+1; j++){
			if (seq1[i] == seq2[j]){ //i and j are the same
				opt[i][j] = min(opt[i-1][j-1], delta + opt[i-1][j], delta + opt[i][j-1]);
				if (opt[i][j] == opt[i-1][j-1])
					sol[i][j] = 2;
				else if (opt[i][j] == delta + opt[i-1][j]) // i matched to gap
					sol[i][j] = 0;
				else //j matched to gap
					sol[i][j] = 1;
			}
			else{
				opt[i][j] = min(alpha + opt[i-1][j-1], delta + opt[i-1][j], delta + opt[i][j-1]);
				if (opt[i][j] == alpha + opt[i-1][j-1])
					sol[i][j] = 2;
				else if (opt[i][j] == delta + opt[i-1][j])
					sol[i][j] = 0;
				else
					sol[i][j] = 1;
			}
		}
	}
	fprintf(soln, "%d\n", opt[seqlen1][seqlen2]);
	findsol(soln, sol, seqlen1, seqlen2);
	fclose(sequence);
	fclose(soln);


}
