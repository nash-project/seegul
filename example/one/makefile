
compile:
	clang++ -o example ./main.cc -I../../src/include/ -L../../ -l:seegul.a

link-output: run
	ld -m elf_i386 -s -o temp temp.o


disa: link-output
	objdump -d temp

clean:
	rm -rf ./example ./temp ./temp.o
run: compile
	./example