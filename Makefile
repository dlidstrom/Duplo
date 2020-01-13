# Compiler
CC = g++

# Flags
CXXFLAGS = -O3 -Wall -std=c++17
LDFLAGS =  ${CXXFLAGS}

# Remove object file if compilation fails
#.DELETE_ON_ERROR:

# Define what extensions we use
.SUFFIXES: .cpp

# Name of executable
PROG_NAME = duplo

# List of object files
OBJS = \
	ArgumentParser.o \
	CstyleUtils.o \
	CstyleCommentsFilter.o \
	Duplo.o \
	FileTypeFactory.o \
	FileTypeBase.o \
	FileType_C.o \
	FileType_CS.o \
	FileType_S.o \
	FileType_Unknown.o \
	FileType_VB.o \
	Main.o \
	NoopLineFilter.o \
	Options.o \
	HashUtil.o \
	SourceFile.o \
	SourceLine.o \
	StringUtil.o \
	TextFile.o

# Build process

all: ${PROG_NAME}

# Link
${PROG_NAME}: ${OBJS}
	${CC} ${LDFLAGS} -o ${PROG_NAME} ${OBJS}

# Each .cpp file compile
.cpp.o:
#	@touch $@
	${CC} ${CXXFLAGS} -c $*.cpp -o$@

# Remove all object files
clean:
	rm -f *.o
