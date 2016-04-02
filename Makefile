OBJ = main.o
INC = -I "./"

raytracing: $(OBJ)
	g++ $(OBJ) -g -o raytracing
	rm -f $(OBJ)

main.o:
	g++ -g -c main.cpp $(INC)

clean:
	rm -f $(OBJ) raytracing