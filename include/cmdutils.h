#pragma once 

#include "music.h"
#include "commands.h"
#include "utils.h"

#include <spdlog/spdlog.h>
#include <string>

template <class Protocol>
class CommandExecuter {
public:
    CommandExecuter(Protocol& p, Music& m) : proto(p), music(m) {}
    virtual ~CommandExecuter() =default;

    virtual void Quit() {
        music.SetStatus(Status::Exit);
    }

    virtual void Play() {
        music.SetStatus(Status::Playing);
    }

    virtual void Pause() {
        if(music.GetStatus() != Status::Playing) {
            music.SetStatus(Status::Playing);
        } else {
            music.SetStatus(Status::Paused);
        }
    }

    virtual void Stop() {
        music.SetStatus(Status::Stoped);
    }

    virtual void Next() {
        music.SetStatus(Status::Forwarding);
    }

    virtual void Back() {
        music.SetStatus(Status::Backing);
    }   

    virtual void SortRandom() {
        music.GetList().Sort(Order::RANDOM);
    }

    virtual void GetArtist() {
        proto << music.GetCurrent().GetArtist();
    }

    virtual void GetTitle() {
        proto << music.GetCurrent().GetTitle();
    }

    virtual void GetFile() {
        proto << music.GetCurrent().GetFile();
    }

    virtual void FilterArtist() {
        //Stop reproduction while we are filtering the list
        auto tmp = music.GetStatus();
        music.SetStatus(Status::Stoped);
        music.GetList().FilterArtist(proto.GetLine());
        music.SetStatus(tmp);
    }

    virtual void AddFolder() {
        music.GetList().LoadDir(proto.GetLine());
    }

    virtual void AddFile() {
        music.GetList().LoadFile(proto.GetLine());
    }

    virtual void LoadPlaylist() {
        auto tmp = music.GetStatus();
        music.SetStatus(Status::Stoped);

        // For the moment works passing the full path of the PlayList
        music.GetList().LoadPlaylist(proto.GetLine());
        music.SetStatus(tmp);
    }

    virtual void VolumeSet() {
        std::string volum;
        proto >> volum;
        #ifdef DEBUG
        spdlog::get("global")->debug("Vol: {}", volum);
        #endif
        music.SetVolume(std::atof(volum.c_str()));        
    }

    virtual void VolumeGet() {
        proto << std::to_string(music.GetVolume());
    }

    virtual void TimeGetRemaining() {
        proto << music.GetRemainingMilliseconds();
    }

    virtual void SetOffset() {
        music.SetPlayingOffset(std::stoi(proto.GetLine()));
    }

    virtual void FileGet() {
        std::ifstream file(music.GetCurrent().GetFile());
        if(!file.is_open())
            throw std::runtime_error("Can not open music file");

        std::size_t size = filesize(music.GetCurrent().GetFile());
        proto <<  size;
        while(size != 0) {
            proto.Put(file.get());
            --size;
        }

        //Flush file
        proto << "";
    }

    virtual void ClearPlaylist() {
        music.SetStatus(Status::Stoped);
        music.GetList().GetSongList().clear();
    }

private:
    Protocol& proto;
    Music& music;
};



class CommandControler {
public:
    CommandControler(Music& c) : music(c) {}

    template <typename T>
    void Execute(Command c, T& proto) {
        spdlog::get("global")->debug("Command:{}", (int)c);

        static CommandExecuter<T> cmd(proto, music);

        switch (c) {
            case Command::QUIT:                 return cmd.Quit();
            case Command::PLAY:                 return cmd.Play();
            case Command::PAUSE:                return cmd.Pause();
            case Command::STOP:                 return cmd.Stop();
            case Command::NEXT:                 return cmd.Next();
            case Command::BACK:                 return cmd.Back();
            case Command::SORT_RANDOM:          return cmd.SortRandom();
            case Command::GET_ARTIST:           return cmd.GetArtist();
            case Command::GET_TITLE:            return cmd.GetTitle();
            case Command::GET_FILE:             return cmd.GetFile();                
            case Command::FILTER_ARTIST:        return cmd.FilterArtist();
            case Command::ADD_FOLDER:           return cmd.AddFolder();
            case Command::ADD_FILE:             return cmd.AddFile();
            case Command::LOAD_PLAYLIST:        return cmd.LoadPlaylist();
            case Command::VOLUME_SET:           return cmd.VolumeSet();
            case Command::VOLUME_GET:           return cmd.VolumeGet();
            case Command::TIME_GET_REMAINING:   return cmd.TimeGetRemaining();
            case Command::SET_OFFSET:           return cmd.SetOffset();
            case Command::FILE_GET:             return cmd.FileGet();
            case Command::CLEAR_PLAYLIST:       return cmd.ClearPlaylist();

            //case Command::SAVE_FILE:
            //  break;
        }
    }
private:
    Music& music;
};