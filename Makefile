FLAGS=-O0 --std=gnu99 -Wall --pedantic

ash: src/ash.c src/shell.c
	gcc $^ ${FLAGS} -o $@

clean:
	rm ash

install: ash
	cp ash /usr/bin/ash

uninstall:
	rm /usr/bin/ash
