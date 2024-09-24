prg=c3
source=$(prg).c
headers=defs.h engine.h game.h helper.h
target=$(prg)
cc=gcc
cflags=--std=c99
lflags=-o $(target) -s

all: $(target)

$(target): $(source) $(headers)
	$(cc) $(cflags) $(source) $(lflags)

clean:
	rm $(target)
