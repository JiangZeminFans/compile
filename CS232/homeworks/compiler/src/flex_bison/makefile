parser.cpp: parser.y
	bison -d -o parser.cpp parser.y
lexer.cpp: lexer.l
	flex -o lexer.cpp lexer.l
clean:
	rm parser.cpp parser.hpp lexer.cpp parser
all:
	make parser.cpp
	make lexer.cpp

