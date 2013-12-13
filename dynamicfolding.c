#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    FILE *sequence, *soln;
    if(argc < 3){
        fprintf(stderr, "Don't forget command line arguments! \n Should be name of file followed by sequence length");
        exit(EXIT_FAILURE);
    }
    if (NULL == (sequence = fopen(argv[1], "r"))){
        fprintf(stderr, "Could not open sequence");
        exit(EXIT_FAILURE);
    }
    if (NULL == (soln = fopen("solution", "w"))){
        fprintf(stderr, "Could not open folding.lp");
        exit(EXIT_FAILURE);
    }
    int seqlen = atoi(argv[2]);
    int seq[seqlen+1];
    seq[0]=0;
    for (int l = 1; l<seqlen+1; l++){
		seq[l]=fgetc(sequence);
	}
    int sol[seqlen+1][seqlen+1];
    int opt[seqlen+1][seqlen+1];
    int i,j,k;
    for (i = 0; i<seqlen+1; i++){
        for (j=0; j<seqlen+1; j++){
            opt[i][j]=sol[i][j]=0;
        }
    }
    int ch;
    int c = 0;
    while((ch = fgetc(sequence))!=EOF){
        seq[c]=ch;
        c++;
    }
    for (k=5; k<seqlen; k++){
        for(i = 1; i<= seqlen-k; i++){
            j = i+k;
            int max = opt[i][j-1];
            int s = 0;
			if(k==5 && (seq[j]+seq[i] == 'C' + 'G' || seq[j]+seq[i] == 'A' + 'U')){
				max = 1;
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
            if (s!=0){
                sol[s][j] = 1;
            }
        }
    }
    for(i = 0; i<=seqlen; i++){
        for (j=0; j<=seqlen; j++){
            fputc((sol[i][j]+'0'),soln);
        }
        fputc('\n',soln);
    }
    fclose(sequence);
    fclose(soln);
}
