#ifndef MP3_H_INCLUDED
#define MP3_H_INCLUDED

#include <SFML/Audio.hpp>
#include "mpg123.h"

namespace sfe
{
class mp3 : public sf::SoundStream
{
public :
    mp3();
    ~mp3();

    bool openFromFile(const std::string& filename);

protected :
    bool onGetData(Chunk& data);
    void onSeek(sf::Time timeOffset);

private :
    mpg123_handle*      myHandle;
    size_t              myBufferSize;
    unsigned char*      myBuffer;
    sf::Mutex           myMutex;
};

} // namespace sfe

#endif // MP3_H_INCLUDED
