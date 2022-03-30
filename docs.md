# Seegul Use Documentation

### Compiling Seegul
#### tools needed
 - clang++
 - gnu make
 - gnu ar
 
#### Compiling the library
the only command you need is 
```
make build-lib
```
That should compile the whole library. The output will be **seegul.a**.
### Your first program
we are going to generate a simple elf object file. It will only be a header file. It will be invalid, but it will be an example.
```c
#include <seegul/seegul.h>

int main(){
	Elf32 * elf = new Elf32();
	elf->write("output.o");
	elf->done();
}

```
seegul will make the elf header itself.
Now to compile this, run
```
clang++ -L./seegul/ (your file name) -l:seegul.a -I./seegul/inlcude/
```
That should compile and then you can run it.
It will output a file called **output.o**

