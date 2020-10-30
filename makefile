lint:
	clang-format -i src/*.cpp src/*.h

obj/%.o: src/%.cpp
	g++ $< -c -o $@

exe: obj/input.o obj/student.o obj/main.o obj/evaluation.o obj/automatic_classroom_organizer.o
	g++ -o bin/$@ $^
# to make it automatic: 
# https://stackoverflow.com/questions/2908057/can-i-compile-all-cpp-files-in-src-to-os-in-obj-then-link-to-binary-in
# $@ == left-hand side of :
# $^ == right-hand side of : (i.e. dependencies)

run: exe
	./bin/exe

clean-run:
	make clean
	make run

clean:
	rm -f obj/*.o
	rm -f bin/exe
