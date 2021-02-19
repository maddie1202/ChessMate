runtest: \
test/test.c \
test/test-game.c \
test/test-possible-move-generators.c \
test/test-ai-move-generator.c \
$(wildcard test/generator-tests/*.c) \
game.c \
possible-move-generators.c \
ai-move-generator.c

	gcc -g -o bin/runtest \
	test/test.c \
	test/test-game.c \
	test/test-possible-move-generators.c \
	test/test-ai-move-generator.c \
	$(wildcard test/generator-tests/*.c) \
	game.c \
	possible-move-generators.c \
	ai-move-generator.c \
	-lm