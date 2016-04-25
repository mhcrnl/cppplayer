#include "music.h"

Music::Music() : status(Status::Stoped) {

}

Status Music::GetStatus() const {
	return status;
}

void Music::SetStatus(Status s) {
	status = s;
}