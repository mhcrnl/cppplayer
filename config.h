class Config {
public:
	void Load();
private:
	struct Options {
		std::string daemonpipe = "/tmp/dplayer++";
		std::string clientpipe = "/tmp/cplayer++";
		path dir = ".";
		bool autostart = false;
	}opt;
};