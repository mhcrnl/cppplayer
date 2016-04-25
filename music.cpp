#include "music.h"

Music::Music() : status(Status::Stoped) {

}

Status Music::GetStatus() const {
	return status;
}