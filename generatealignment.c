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
    if (NULL == (glpsol = fopen("alignment.lp", "w"))){
        fprintf(stderr, "Could not open alignment.lp\n");
        exit(EXIT_FAILURE);
    }
	if (argc < 5){
		printf("Don't forget command line arguments!\n Should be seqlen1, seqlen2, alpha, delta. \n");
		exit(EXIT_FAILURE);
	}
    int seqlen1 = atoi(argv[1]); //length of first sequence
	int seqlen2 = atoi(argv[2]); //length of second sequence
	int a = atoi(argv[3]); //alpha = mismatch penalty
	int d = atoi(argv[4]); //delta = gap penalty
	int seq1[seqlen1];
	int seq2[seqlen2];
    int bases[] = {'A', 'C', 'G', 'U'};
    int i, j, k;
    for (i = 0; i<seqlen1; i++){
        int r = rand()%4;
        fputc(bases[r], sequence);
		seq1[i]=bases[r];
    }
    fputc('\n', sequence);
    for (i = 0; i<seqlen2; i++){
        int r = rand()%4;
        fputc(bases[r], sequence);
		seq2[i]=bases[r];
    }

    fprintf(glpsol, "Minimize\n");
    //minimize function
	for (i = 0; i<seqlen1; i++){
		for (j = 0; j<seqlen2; j++){
			if (seq1[i]!=seq2[j]){
				fprintf(glpsol, "+ %d X%d,%d ", a, i, j); //Xi,j = matching i to j
			}
		}
		fprintf(glpsol, "\n");
	}
	for (i = 0; i<seqlen1; i++){
		fprintf(glpsol, "+ %d Y%d,%d ", d, i, 1); // Yi,1 = matching i to a gap
	}
	fprintf(glpsol, "\n");
	for (i = 0; i<seqlen2; i++){
		fprintf(glpsol, "+ %d Y%d,%d ", d, i, 2); //Yi,2 = matching i to a gap
	}
	fprintf(glpsol, "\n");

    fprintf(glpsol, "Subject To\n");
    //constraints
	//everything in sequence one must be paired with exactly one thing
	for (i = 0; i < seqlen1; i++){
		for (j = 0; j<seqlen2; j++){
			fprintf(glpsol, "+ X%d,%d ", i, j); 
		}
		fprintf(glpsol, "+ Y%d,%d = 1\n", i, 1);
	}
	//everything in seq2 must be paired with exactly one thing
	for (i = 0; i < seqlen2; i++){
		for (j = 0; j<seqlen1; j++){
			fprintf(glpsol, "+ X%d,%d ", j, i); 
		}
		fprintf(glpsol, "+ Y%d,%d = 1\n", i, 2);
	}
	//noncrossing condition
	int l;
	for (i = 0; i<seqlen1; i++){
		for (j = 0; j<seqlen2; j++){
			for (k = 0; k<i; k++){
				for (l = j+1; l<seqlen2; l++){
					fprintf(glpsol, "+ X%d,%d + X%d,%d <= 1\n", i, j, k, l);
				}
			}
		}
	}

    fprintf(glpsol, "Bounds\n");
    //bounds
	for (i = 0; i<seqlen1; i++){
		for (j = 0; j<seqlen2; j++){
			fprintf(glpsol, "0 <= X%d,%d <= 1\n", i, j);
		}
	}
	for (i = 0; i<seqlen1; i++){
		fprintf(glpsol, "0 <= Y%d,%d <=1\n", i, 1);
	}
	for (j = 0; j<seqlen2; j++){
		fprintf(glpsol, "0 <= Y%d,%d <=1\n", j, 2);
	}

    fprintf(glpsol, "Integers\n");
    //integers
	for (i = 0; i<seqlen1; i++){
		for (j = 0; j<seqlen2; j++){
			fprintf(glpsol, "X%d,%d\n", i, j);
		}
	}
	for (i = 0; i<seqlen1; i++){
		fprintf(glpsol, "Y%d,%d\n", i, 1);
	}
	for (j = 0; j<seqlen2; j++){
		fprintf(glpsol, "Y%d,%d\n", j, 2);
	}
    fprintf(glpsol, "End");

    fclose(sequence);
    fclose(glpsol);
}
