
COMPILER=g++
VERSION=-std=c++2a

output: main.o train.o trade.o data.o encoder.o linear.o dnn.o
	$(COMPILER) $(VERSION) main.o train.o trade.o data.o encoder.o linear.o dnn.o -o trader
	rm *.o

main.o: ./src/main.cpp
	$(COMPILER) $(VERSION) -c ./src/main.cpp

train.o: ./src/train.cpp
	$(COMPILER) $(VERSION) -c ./src/train.cpp

trade.o: ./src/trade.cpp
	$(COMPILER) $(VERSION) -c ./src/trade.cpp

data.o: ./src/data.cpp
	$(COMPILER) $(VERSION) -c ./src/data.cpp

encoder.o: ./src/encoder.cpp
	$(COMPILER) $(VERSION) -c ./src/encoder.cpp

linear.o: ./src/linear.cpp
	$(COMPILER) $(VERSION) -c ./src/linear.cpp

dnn.o: ./src/dnn.cpp
	$(COMPILER) $(VERSION) -c ./src/dnn.cpp
