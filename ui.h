
//Command -> Command line
class UICommand {
	public:
		UICommand();
		~UICommand();
		char GetChar();
	private:
		const char* fpipe;
};

//Console -> Terminal
class UIConsole {
	public:
		char GetChar();
};
