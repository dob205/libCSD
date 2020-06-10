CPPFLAGS=-Wall -Wextra -std=c++11 -pedantic -Werror -O3
CLANG_EXTRA_FLAGS=-Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition

ifeq ($(CXX),clang)
	CPPFLAGS += $(CLANG_EXTRA_FLAGS)
endif

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


