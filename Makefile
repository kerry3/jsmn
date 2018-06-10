# You can put your build options here
-include config.mk

all: libjsmn.a

libjsmn.a: jsmn.o
	$(AR) rc $@ $^

%.o: %.c jsmn.h
	$(CC) $(DEBUG) -c $(CFLAGS) $< -o $@

test: test_default test_strict test_links test_strict_links
test_default: test/tests.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o test/$@
	./test/$@
test_strict: test/tests.c
	$(CC) -DJSMN_STRICT=1 $(CFLAGS) $(LDFLAGS) $< -o test/$@
	./test/$@
test_links: test/tests.c
	$(CC) -DJSMN_PARENT_LINKS=1 $(CFLAGS) $(LDFLAGS) $< -o test/$@
	./test/$@
test_strict_links: test/tests.c
	$(CC) -DJSMN_STRICT=1 -DJSMN_PARENT_LINKS=1 $(CFLAGS) $(LDFLAGS) $< -o test/$@
	./test/$@

jsmn_test.o: jsmn_test.c libjsmn.a


myexample1: mysource/filesimple.o libjsmn.a
	$(CC) $(LDFLAGS) $^ -o $@
myexample: example/simple.o libjsmn.a
	$(CC) $(LDFLAGS) $^ -o $@
tueexample: example/tueseven.o libjsmn.a
	$(CC) $(LDFLAGS) $^ -o $@

jsondump: example/jsondump.o libjsmn.a
	$(CC) $(LDFLAGS) $^ -o $@
myexample2: mysource/filesimple2.o libjsmn.a
	$(CC) $(LDFLAGS) $^ -o $@
myexample3: mysource/filesimple3.o libjsmn.a
	$(CC) $(LDFLAGS) $^ -o $@
myproduct: mysource/myproduct.o libjsmn.a
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o example/*.o
	rm -f *.a *.so
	rm -f tueexample
	rm -f jsondump


.PHONY: all clean test
