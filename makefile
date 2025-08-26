myassem: asembler.o pre_assembler.o auxiliary_functions.o fixed_tables.o first_path.o second_path.o output.o
	gcc -ansi -Wall -fsanitize=address,undefined -o myassem asembler.c pre_assembler.c first_path.c second_path.c output.c fixed_tables.c auxiliary_functions.c

output.o: output.c output.h pre_assembler.h fixed_tables.h auxiliary_functions_constants.h
	gcc -ansi -Wall -c output.c -o output.o
	
asembler.o: asembler.c auxiliary_functions_constants.h pre_assembler.h first_path.h second_path.h fixed_tables.h
	gcc -ansi -Wall -c asembler.c -o asembler.o

auxiliary_functions.o: auxiliary_functions.c auxiliary_functions_constants.h pre_assembler.h fixed_tables.h
	gcc -ansi -Wall -c auxiliary_functions.c -o auxiliary_functions.o

first_path.o: first_path.c  first_path.h pre_assembler.h fixed_tables.h auxiliary_functions_constants.h 
	gcc -ansi -Wall -c first_path.c -o first_path.o

pre_assembler.o: pre_assembler.c pre_assembler.h fixed_tables.h auxiliary_functions_constants.h
	gcc -ansi -Wall -c pre_assembler.c -o pre_assembler.o

second_path.o: second_path.c second_path.h first_path.h pre_assembler.h fixed_tables.h auxiliary_functions_constants.h
	gcc -ansi -Wall -c second_path.c -o second_path.o

fixed_tables.o: fixed_tables.c fixed_tables.h auxiliary_functions_constants.h pre_assembler.h
	gcc -ansi -Wall -c fixed_tables.c -o fixed_tables.o
