CC = g++
OBJ = main.cpp

run: $(OBJ)
	@ $(CC) -g $(OBJ) -o run

	@ if [ -f ./run ]; then \
		echo "[MAKEFILE]: Compile and Running $(OBJ)"; \
		./run; \
	fi

compile: $(OBJ)
	$(CC) -g $(OBJ) -o run

.PHONY:clean
clean:
	rm -rf *.o run 
