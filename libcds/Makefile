
#CPPFLAGS=-g3 -Wall -m32 -O0 
#CPPFLAGS=-O3 -Wall -DNDEBUG 
CPPFLAGS=-Wall -Wextra -std=c++11 -pedantic -Wmissing-prototypes -Wstrict-prototypes \
    -Wold-style-definition -Werror -O3

all: clean libcompact

doc:
	@echo " [DOC] Generating documentation"
	@doxygen

libcompact: 
	@echo " [MSG] Entering directory src"
	@CPPFLAGS="$(CPPFLAGS)" make --no-print-directory -C src

tests: libcompact
	@echo " [MSG] Entering directory tests"
	@CPPFLAGS="$(CPPFLAGS)" make --no-print-directory -C tests 

clean:
	@echo " [MSG] Entering directory src"
	@make --no-print-directory -C src clean
	@echo " [MSG] Entering directory tests"
	@make --no-print-directory -C tests clean
	@echo " [CLN] Cleaning docs folder"
	@rm -rf docs/*
	@touch docs/delete_me
	@echo " [CLN] Cleaning lib folder"
	@rm -f lib/*
	@touch lib/delete_me
	@echo " [CLN] Cleaning includes folder"
	@rm -f includes/*
	@touch includes/delete_me


