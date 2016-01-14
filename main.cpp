#include "player.h"
#include "utils.h"
int main(int argc, char* argv[]) {

	//workaround
	if(argc>2 && *argv[2] == 'd') {
			daemonize();
	}

	Player p;
	p.Initialize(argc, argv);
	p.Run();
}
