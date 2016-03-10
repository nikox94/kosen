OBJ = main.o
INC = -I "./"

raytracing: $(OBJ)
	g++ $(OBJ) -o raytracing
	rm -f $(OBJ)

main.o:
	g++ -c main.cpp $(INC)

clean:
	rm -f $(OBJ) raytracing