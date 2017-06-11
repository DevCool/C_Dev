CCFLAGS=-Wno-unused-result -O2
#CCSTATICFLAGS=${CCFLAGS} -rdynamic
#CCDYNAMICFLAGS=${CCFLAGS} -fPIC
LDFLAGS=
#LDSTATICFLAGS=${LDFLAGS} -ldl -lpopt
#LDDYNAMICFLAGS=${LDFLAGS} -shared
TARBALL=customserv2/server2.tar

#OBJECTS= example.o


all: server #example.so

rebuild: clean all


%.o: %.c
	${CC} ${CCDYNAMICFLAGS} -c -o $@ $<

%.so: %.o
	${CC} ${LDDYNAMICFLAGS} -o $@ $<

hdl_client.o: hdl_client.c hdl_client.h
	${CC} ${CCSTATICFLAGS} -c -o $@ $<

custom.o: custom.c hdl_client.h custom.h
	${CC} ${CCSTATICFLAGS} -c -o $@ $<

debug: custom.o hdl_client.o
	${CC} -g ${CCSTATICFLAGS} ${LDSTATICFLAGS} -o $@ hdl_client.o custom.o

server: custom.o hdl_client.o
	${CC} ${CCSTATICFLAGS} ${LDSTATICFLAGS} -o $@ hdl_client.o custom.o

dist: clean
	(cd ..; tar cvf ${TARBALL} customserv2/*; gzip --best --force --verbose ${TARBALL})

clean:
	@echo "Cleaning project..."
	@rm -f *.o server
	@echo "Done."

