#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
    FILE *sequence, *glpsol;
    if (NULL == (sequence = fopen("sequence", "w"))){
        fprintf(stderr, "Could not open sequence\n");
        exit(EXIT_FAILURE);
    }
    if (NULL == (glpsol = fopen("folding.lp", "w"))){
        fprintf(stderr, "Could not open folding.lp\n");
        exit(EXIT_FAILURE);
    }
    if (argc<2){
		printf("Don't forget command line arguments!\n");
		exit(EXIT_FAILURE);
	}
    int seqlen = atoi(argv[1]);
    int seq[seqlen];
    int bases[] = {'A', 'C', 'G', 'U'};
    int i;
    for (i = 0; i<seqlen; i++){
        int r = rand()%4;
        fputc(bases[r], sequence);
        seq[i]=bases[r];
    }

    fprintf(glpsol, "Maximize\n");
    for (int j = 0; j<seqlen-1; j++){
        for(int k = j+4; k<seqlen; k++){
            if (seq[j]+seq[k] == 'C'+'G' || seq[j]+seq[k] == 'A' + 'U') {
                fprintf(glpsol, "+ X%d,%d ", j, k);
            }
        }
        fprintf(glpsol, "\n");
    }
    fprintf(glpsol, "Subject To\n");
    //constraints
    //a base can only bind to one other base
    for (int j = 0; j<seqlen; j++){
        int count = 0;
		for (int l = 0; l<j-4; l++){
            if ((seq[l]+seq[j]) == ('C' + 'G') || (seq[l]+seq[j]) == ('A' + 'U')) {
				fprintf(glpsol, "+ X%d,%d ", l, j);
				count++;
			}
		}
        for(int k = j+5; k<seqlen; k++){
            if ((seq[k]+seq[j]) == ('C' + 'G') || (seq[k]+seq[j]) == ('A' + 'U')) {
                fprintf(glpsol, "+ X%d,%d ", j, k);
                count++;
            }
        }
        if(count >0) fprintf(glpsol, "<= 1\n");
    }


	//noncrossing condition
	for (int i =0; i<seqlen; i++){
		for (int k = i+1; k<seqlen; k++){
			for (int j = k+1; j<seqlen; j++){
				for (int l = j+1; l<seqlen; l++){
					if (seq[i]+seq[j] == 'C'+'G' || seq[i]+seq[j] == 'A'+'U'){
						if (seq[k]+seq[l] == 'C'+'G' || seq[k]+seq[l] == 'A'+'U'){
							fprintf(glpsol, "+ X%d,%d + X%d,%d <= 1\n", i, j, k, l);
						}
					}
				}
			}
		}
	}

    fprintf(glpsol, "Bounds\n");
    for (int j = 0; j<seqlen-1; j++){
        for(int k = j+1; k<seqlen; k++){
            if (seq[k]+seq[j] == 'C' + 'G' || seq[k]+seq[j] == 'A' + 'U') fprintf(glpsol, "0 <=  X%d,%d <= 1\n", j, k);
        }
    }
    fprintf(glpsol, "Integers\n");
    for (int j = 0; j<seqlen-1; j++){
        for(int k = j+1; k<seqlen; k++){
            if (seq[k]+seq[j] == 'C' + 'G' || seq[k]+seq[j] == 'A' + 'U') fprintf(glpsol, "X%d,%d \n", j, k);
        }
    }
    fprintf(glpsol, "End");

    fclose(sequence);
    fclose(glpsol);
}
