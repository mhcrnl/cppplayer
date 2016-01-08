#include <array>
#include <unistd.h>
#include <termios.h>
#include <cstring>

#include "utils.h"

using namespace std;


bool isSupported(path p) {
	const array<path, 1> formats = {path(".mp3")};
	for(auto& v : formats)
		if(p.extension() == v)
			return true;

	return false;
}

char getch() {
        char buf = 0;
        struct termios old;
        memset(&old, 0, sizeof(old));
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}