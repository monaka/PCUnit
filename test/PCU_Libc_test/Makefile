CFLAGS = -Wall -std=c99 -g

TARGET = PCU_Libc_test

OBJS = main.o
OBJS += PCU_Libc.o
OBJS += test_format_no_stdarg.o
OBJS += test_string.o
OBJS += test_strtol.o
OBJS += ../../PCUnit/libpcunit.a

.PHONY: all clean pcunit_register test xml

all: pcunit_register $(TARGET)

pcunit_register:
	-ruby ../../PCUnit/pcunit_register.rb
	cd ../../PCUnit && $(MAKE) clean
	cd ../../PCUnit && $(MAKE) CFLAGS="-Wall -W -std=c99 -DPCU_USE_WCHAR -g -DPCU_FORMAT_BUFSIZE=256"


PCU_LIBC_DEFINE = -DPCU_NO_LIBC -DPCU_NO_STDARG
PCU_LIBC_DEFINE += -DPCU_FORMAT_BUFSIZE=256
PCU_LIBC_DEFINE += -DPCU_set_putchar=PCU_set_putchar_dummy
PCU_LIBC_DEFINE += -DPCU_set_getchar=PCU_set_getchar_dummy
PCU_LIBC_DEFINE += -DPCU_getchar=PCU_getchar_dummy
PCU_LIBC_DEFINE += -DPCU_puts=PCU_puts_dummy
PCU_LIBC_DEFINE += -DPCU_formatW=PCU_formatW_dummy

PCU_Libc.o: ../../PCUnit/PCU_Libc.c
	$(CC) $(CFLAGS) $(PCU_LIBC_DEFINE) -c $<


.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $<


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
	cd ../../PCUnit && $(MAKE) clean

test:
	./$(TARGET)

xml:
	./$(TARGET) | ruby ../../PCUnit/pcunit_xml_output.rb $(TARGET)_results.xml

clean:
	rm -f *.o $(TARGET) *.xml


PCU_Libc.o: ../../PCUnit/PCU_Libc.h ../../PCUnit/PCUnit.h
main.o: ../../PCUnit/PCUnit.h
test_format_no_stdarg.o: ../../PCUnit/PCUnit.h
test_string.o: ../../PCUnit/PCUnit.h
test_strtol.o: ../../PCUnit/PCUnit.h

