

top_builddir=..
top_srcdir=..
srcdir=.


include ../Make.rules

LIB_DEPS=""
LIB_OBJS=pr_fnmatch.o sstrncpy.o strsep.o vsnprintf.o glibc-glob.o glibc-hstrerror.o glibc-mkstemp.o pr-syslog.o pwgrent.o hanson-tpl.o ccan-json.o

"":

dummy:

all:
	@echo "Running make from top level directory."
	cd ../ && $(MAKE) all

Makefile: Makefile.in ../config.status
	cd ../ && ./config.status

.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

libltdl: dummy
	cd libltdl/ && $(MAKE)

libsupp.a: $(LIB_OBJS)
	$(AR) rc libsupp.a $(LIB_OBJS)
	$(RANLIB) libsupp.a

lib: libsupp.a $(LIB_DEPS)

install:

clean:
	$(RM) *.o libsupp.a
	test -z $(LIB_DEPS) || (cd libltdl/ && $(MAKE) clean)

depend:
	$(MAKEDEPEND) $(CPPFLAGS) $(top_srcdir)/lib/*.c 2>/dev/null
	$(MAKEDEPEND) $(CPPFLAGS) -fMakefile.in $(top_srcdir)/lib/*.c 2>/dev/null

distclean: clean
	-$(RM) *.gcda *.gcno
	test -z $(LIB_DEPS) || (cd libltdl/ && $(MAKE) distclean)
