home=c:\tc
cc=$(home)\tcc
link=$(home)\tlink
pck=upx
lib=$(home)\lib
cflags=-c -mt
lflags=/t /x
prg=c3
source=$(prg).c
object=$(prg).obj
target=$(prg).com

all: $(target)

$(target): $(source)
	$(cc) $(cflags) $(source)
	$(link) $(lflags) $(lib)\c0t.obj $(object),$(target),,$(lib)\cs.lib
	$(pck) -9 $(target)

clean:
	erase c3.com
	erase c3.map
	erase c3.obj
