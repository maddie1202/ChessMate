# Test the SW version of the ai
runtestSW: \
test/test.c \
test/test-game.c \
test/test-possible-move-generators.c \
test/test-ai-move-generator.c \
$(wildcard test/generator-tests/*.c) \
game.c \
possible-move-generators-SW.c \
generator-support-SW.c \
ai-move-generator.c \

	gcc -std=c99 -Wall -fsigned-char -g -o bin/runtestSW \
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

# Test the HW version of the AI
runtestHW: \
test/test.c \
test/test-game.c \
test/test-possible-move-generators.c \
test/test-ai-move-generator.c \
$(wildcard test/generator-tests/*.c) \
game.c \
possible-move-generators-SW.c \
generator-support-SW.c \
ai-move-generator.c \
hardware-setup.c \

	gcc -std=c99 -Wall -fsigned-char -g -o bin/runtestHW \
	test/test.c \
	test/test-game.c \
	test/test-possible-move-generators.c \
	test/test-ai-move-generator.c \
	$(wildcard test/generator-tests/*.c) \
	game.c \
	possible-move-generators-HW.c \
	generator-support-SW.c \
	ai-move-generator.c \
	hardware-setup.c \
	-lm

playgame: \
main.c \
comms.c \
game.c \
possible-move-generators-SW.c \
generator-support-SW.c \
ai-move-generator.c \
hardware-setup.c \
frozen/frozen.c \

	gcc -std=c99 -Wall -fsigned-char -g -o bin/playgame \
	main.c \
	comms.c \
	game.c \
	possible-move-generators-HW.c \
	generator-support-SW.c \
	ai-move-generator.c \
	hardware-setup.c \
	frozen/frozen.c \
	-lm -lcurl
