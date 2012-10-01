include config.mk

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES = $(patsubst %.cpp,%.o,$(CPP_FILES))

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

${NAME} : $(OBJ_FILES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) ${LDFLAGS} $^ -o $@

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f ${NAME} ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/${NAME}

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/${NAME}

options:
	@echo ${NAME} build options:
	@echo "CXXFLAGS   = ${CXXFLAGS}"
	@echo "LDFLAGS    = ${LDFLAGS}"
	@echo "CXX        = ${CXX}"

clean:
	rm -f ${NAME} *.o ${NAME}-${VERSION}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p ${NAME}-${VERSION}
	@cp -R Makefile config.mk \
		${SRC} ${NAME}-${VERSION}
	@tar -cf ${NAME}-${VERSION}.tar ${NAME}-${VERSION}
	@gzip ${NAME}-${VERSION}.tar
	@rm -rf ${NAME}-${VERSION}

.PHONY: all options clean dist install uninstall
