obj = main.o cout.o

vihex : $(obj)
	g++ -o vihex $(obj)

main.o : vihex.h

cout.o : vihex.h vhx_storage.h

debug :
	g++ -o debug.out main.cpp cout.cpp -ggdb

.PHONY : clean
clean :
	rm $(obj)
