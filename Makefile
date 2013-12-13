generatefolding: generatefolding.c
	gccx -o generatefolding generatefolding.c

generatealignment: generatealignment.c
	gccx -o generatealignment generatealignment.c

dynamicfolding: dynamicfolding.c
	gccx -o dynamicfolding dynamicfolding.c

clean:
	rm folding.lp sequence generatefolding solution generatealignment dynamicfolding alignment.lp


