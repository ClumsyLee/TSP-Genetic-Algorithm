CFLAG = -c -std=c++11 -O2 -Wall -Wextra

bin/solver: obj/main.o obj/genetic.o obj/individual.o | bin
	g++ -o $@ $^

obj/main.o: src/main.cpp src/genetic.h | obj
	g++ $(CFLAG) -o $@ $<

obj/genetic.o: src/genetic.cpp src/genetic.h src/individual.h | obj
	g++ $(CFLAG) -o $@ $<

obj/individual.o: src/individual.cpp src/individual.h | obj
	g++ $(CFLAG) -o $@ $<

bin:
	mkdir $@

obj:
	mkdir $@

clean:
	-rm obj/*.o
