# Makefile for Win32/macOS/Linux (modern systems)
# For pure DOS, use: make -f makefile.dos

prg=c3
source=$(prg).c
headers=defs.h engine.h game.h helper.h
target=$(prg)
test_dir=test
test_target=$(test_dir)/tst_eng
test_helper_target=$(test_dir)/tst_hlp
cc=gcc
cflags=--std=c99
lflags=-o $(target) -s

all: $(target)

$(target): $(source) $(headers)
	$(cc) $(cflags) $(source) $(lflags)

$(test_target): $(test_dir)/tst_eng.c $(headers)
	$(cc) $(cflags) $(test_dir)/tst_eng.c -o $(test_target)

$(test_helper_target): $(test_dir)/tst_hlp.c defs.h helper.h
	$(cc) $(cflags) $(test_dir)/tst_hlp.c -o $(test_helper_target)

test: $(test_target) $(test_helper_target)
	$(test_target)
	$(test_helper_target)

clean:
	rm -f $(target) $(test_target) $(test_helper_target)
