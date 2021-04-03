runtest: \
test/test.c \
test/test-game.c \
test/test-possible-move-generators.c \
test/test-ai-move-generator.c \
$(wildcard test/generator-tests/*.c) \
game.c \
possible-move-generators-SW.c \
generator-support-SW.c \
ai-move-generator.c

	gcc -std=c99 -Wall -fsigned-char -g -o bin/runtest \
	test/test.c \
	test/test-game.c \
	test/test-possible-move-generators.c \
	test/test-ai-move-generator.c \
	$(wildcard test/generator-tests/*.c) \
	game.c \
	possible-move-generators-SW.c \
	generator-support-SW.c \
	ai-move-generator.c \
	-lm