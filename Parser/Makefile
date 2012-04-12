#################### Macros #####################

PROJ = Simplex

OBJECTS = Lexer Parser
INCLUDE = -I../SDL-1.2.14/include/SDL

EXTERNAL_BIN = ../SDL-1.2.14/bin
TARGETNAME = Simplex
TARGET_OBJECTS = Main

TESTERNAME = Test
TESTER_OBJECTS = LexerTests ParserTests TesterMain

# CC macros
COMMON_CFLAGS = -static-libgcc
CFLAGS = -O0 -g -mwindows
CDEFINES =

# CPP macros (additional to C macros)
COMMON_CPPFLAGS = -static-libstdc++
CPPFLAGS = 
CPPDEFINES =

# Universal macros
LIBS = -L../SDL-1.2.14/lib -lmingw32 -lSDLmain -lSDL -lpthread.dll
SOURCE = src
TARGET = .

include cpp_exec.mk