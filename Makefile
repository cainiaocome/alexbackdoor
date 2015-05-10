ccflags = -Wall -O3

.PHONY: alexbackdoor info
alexbackdoor: alexbackdoor.c
	gcc $(ccflags) -o alexbackdoor alexbackdoor.c
info: alexbackdoor
	objdump -x alexbackdoor
clean:
	rm -rf alexbackdoor
