# Compiler
CC = g++

# Flags
CXXFLAGS = -O3 -Wall
LDFLAGS =  ${CXXFLAGS}

# Define what extensions we use
.SUFFIXES : .cpp

# Name of executable
PROG_NAME = duplo

# List of object files
OBJS = StringUtil.o HashUtil.o ArgumentParser.o TextFile.o \
       SourceFile.o SourceLine.o Duplo.o FileType.o

# Build process

all: ${PROG_NAME}

# Link
${PROG_NAME}: ${OBJS}
	${CC} ${LDFLAGS} -o ${PROG_NAME} ${OBJS}

# Each .cpp file compile
.cpp.o:
	${CC} ${CXXFLAGS} -c $*.cpp -o$@

# Remove all object files
clean:	
	rm -f *.o




