#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>

#include "ui.h"
#include "utils.h"

UICommand::UICommand() {
	fpipe = "/tmp/player++";
	mkfifo(fpipe, 0666);
}

UICommand::~UICommand() {
	unlink(fpipe);
}

char UICommand::GetChar() {
	int fd = open(fpipe, O_RDONLY);
	char c = 0;
	read(fd, &c, 1);
	close(fd);
	return c;
}

char UIConsole::GetChar() {
	return getch();
}