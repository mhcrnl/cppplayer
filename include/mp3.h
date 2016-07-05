#pragma once

#include <SFML/Audio.hpp>
#include "mpg123.h"

namespace sfe
{
class mp3 : public sf::SoundStream
{
public :
    mp3();
    ~mp3();

    sf::Time getDuration() const;
    
    void setPlayingOffset(sf::Time timeOffset);
    sf::Time getPlayingOffset();

    bool openFromFile(const std::string& filename);

protected :
    bool onGetData(Chunk& data);
    void onSeek(sf::Time timeOffset);

private :
	sf::Time 			myDuration;
    mpg123_handle*      myHandle;
    size_t              myBufferSize;
    unsigned char*      myBuffer;
    sf::Mutex           myMutex;
};

} // namespace sfe
