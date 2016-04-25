#pragma once

enum class Status {
	Playing,
	Paused,
	Stoped,
	Exit,
};

class Music {
public:
	Music();
	Status GetStatus() const;
	void SetStatus(Status s);
private:
	Status status;	
};