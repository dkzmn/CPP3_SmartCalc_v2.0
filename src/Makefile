CC=g++
FLAGS=-Wall -Wextra -Werror -std=c++17
FLAGS_SAN=-fsanitize=address -pedantic
COVFLAGS=--coverage 
LDLIBS=-lgtest

TEST_DIR = ./tests
REPORT_DIR = ./report

SOURCES = ./*.cc ./controller/*.cc ./model/*.cc ./view/*.cc ./tests/*.cc
HEADERS = ./*.h ./controller/*.h ./model/*.h ./view/*.h
TESTS = $(wildcard $(TEST_DIR)/*.cc ./model/model.cc)

OS:=$(shell uname -s)
ifeq ($(OS), Darwin)
	OPEN = open
	RUN = open ./build/SmartCalc2.app/
	LEAKS =	leaks -atExit -- 
else
	OPEN = xdg-open
	RUN = ./build/SmartCalc2
	LEAKS = valgrind 
endif

rebuild: clean all

all: tests gcov_report install run

install: clean
	mkdir -p build
	cmake -S ./ -B ./build
	cd build && make

run:
	$(RUN)

uninstall: 
	rm -rf ./build/

dvi:
	cd docs && pdflatex dvi.tex && open dvi.pdf

dist: install
	tar -cf smartcalc-2.0.tar build/*

tests: clean
	$(CC) $(FLAGS) $(FLAGS_SAN) $(TESTS) $(LDLIBS) -o tests.out
	./tests.out

$(REPORT_DIR):
	mkdir -p $(REPORT_DIR)

gcov_report: $(REPORT_DIR)
	$(CC) $(FLAGS) $(FLAGS_SAN) $(COVFLAGS) $(TESTS) $(LDLIBS) -o report.out
	./report.out
	lcov -o $(REPORT_DIR)/smartcalc.info -c -d . --no-external
	genhtml $(REPORT_DIR)/smartcalc.info -o $(REPORT_DIR)
	$(OPEN) $(REPORT_DIR)/index.html
	rm -rf *.gcda *.gcno

clean:
	@rm -rf *.o *.a *.css *.html *.gcda *.gcno *.info $(REPORT_DIR)
	@rm -rf report.out tests.out build* ./*txt.user smartcalc-2.0.tar
	@rm -rf docs/*.aux docs/*.dvi docs/*.log docs/*.pdf *.log

style:
	cppcheck $(SOURCES) $(HEADERS) --language=c++
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i $(SOURCES) $(HEADERS)
	clang-format -n $(SOURCES) $(HEADERS)
	rm -rf .clang-format

leaks: clean
	$(CC) $(FLAGS) $(TESTS) $(LDLIBS) -o tests.out
	$(LEAKS)./tests.out