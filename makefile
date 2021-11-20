
COMPILER=g++
VERSION=-std=c++20

output: main.o train.o data.o linear.o dnn.o
	$(COMPILER) $(VERSION) main.o train.o data.o linear.o dnn.o -o trader
	rm *.o

main.o: ./src/main.cpp
	$(COMPILER) $(VERSION) -c ./src/main.cpp

train.o: ./src/train.cpp
	$(COMPILER) $(VERSION) -c ./src/train.cpp

data.o: ./src/data.cpp
	$(COMPILER) $(VERSION) -c ./src/data.cpp

linear.o: ./src/linear.cpp
	$(COMPILER) $(VERSION) -c ./src/linear.cpp

dnn.o: ./src/dnn.cpp
	$(COMPILER) $(VERSION) -c ./src/dnn.cpp
