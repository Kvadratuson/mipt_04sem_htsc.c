LIBRARY := htsc
TESTS   := utests

CFLAGS       := -std=c99 -Wall
CFLAGS_DEBUG := -std=c99 -Wall --coverage

CC := gcc

all: clean release debug
	@echo DONE

release: lib$(LIBRARY).a

lib$(LIBRARY).a: $(LIBRARY).o
	ar rcs lib$(LIBRARY).a $<

$(LIBRARY).o: $(LIBRARY).c $(LIBRARY).h
	$(CC) $< -c $(CFLAGS) -o $@

debug: $(TESTS)

$(TESTS): $(TESTS).c lib$(LIBRARY)_debug.a
	$(CC) $^ $(CFLAGS_DEBUG) -o $@

lib$(LIBRARY)_debug.a: $(LIBRARY)_debug.o
	ar rcs $@ $<

$(LIBRARY)_debug.o: $(LIBRARY).c $(LIBRARY).h
	$(CC) $< -c $(CFLAGS_DEBUG) -o $@

clean:
	rm -rf *.o *.a $(TESTS) *.gcno *.gcda *.gcov vgcore*

.PHONY: all release debug clean
