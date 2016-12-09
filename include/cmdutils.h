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

     void Quit() {
        music.SetStatus(Status::Exit);
    }

     void Play() {
        music.SetStatus(Status::Playing);
    }

     void Pause() {
        if(music.GetStatus() != Status::Playing) {
            music.SetStatus(Status::Playing);
        } else {
            music.SetStatus(Status::Paused);
        }
    }

     void Stop() {
        music.SetStatus(Status::Stoped);
    }

     void Next() {
        music.SetStatus(Status::Forwarding);
    }

     void Back() {
        music.SetStatus(Status::Backing);
    }   

     void SortRandom() {
        music.GetList().Sort(Order::RANDOM);
    }

     void SortLLF() {
        music.GetList().Sort(Order::LLF);
    }

     void GetArtist() {
        proto << music.GetCurrent().GetArtist();
    }

     void GetTitle() {
        proto << music.GetCurrent().GetTitle();
    }

     void GetFile() {
        proto << music.GetCurrent().GetFile();
    }

     void FilterArtist() {
        //Stop reproduction while we are filtering the list
        auto tmp = music.GetStatus();
        music.SetStatus(Status::Stoped);
        music.GetList().FilterArtist(proto.GetLine());
        music.SetStatus(tmp);
    }

     void AddFolder() {
        music.GetList().LoadDir(proto.GetLine());
    }

     void AddFile() {
        music.GetList().LoadFile(proto.GetLine());
    }

     void LoadPlaylist() {
        auto tmp = music.GetStatus();
        music.SetStatus(Status::Stoped);

        // For the moment works passing the full path of the PlayList
        music.GetList().LoadPlaylist(proto.GetLine());
        music.SetStatus(tmp);
    }

     void VolumeSet() {
        std::string volum;
        proto >> volum;
        spdlog::get("global")->debug("Vol: {}", volum);

        float new_volum;
        if(volum[0] == '+')
            new_volum = music.GetVolume() + std::atof(&volum[1]);
        else if(volum[0] == '-')
            new_volum = music.GetVolume() - std::atof(&volum[1]);
        else
            new_volum = std::atof(volum.c_str());
        if(new_volum > 100.0)  new_volum = 100;
        if(new_volum < 0) new_volum = 0;
        music.SetVolume(new_volum);        
    }

     void VolumeGet() {
        proto << std::to_string(music.GetVolume());
    }

     void TimeGetRemaining() {
        proto << music.GetRemainingMilliseconds();
    }

     void SetOffset() {
        music.SetPlayingOffset(std::stoi(proto.GetLine()));
    }

     void FileGet() {
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

     void FilePut() {
        //TODO: Implement this
    }

    void ClearPlaylist() {
        music.SetStatus(Status::Stoped);
        music.GetList().GetSongList().clear();
    }

    void SaveFile() {
        std::ofstream f(proto.GetLine());
        if(!f.is_open())
            throw std::runtime_error("Can not open playlist");

        f << music.GetCurrent().GetFile() << std::endl;
    }

    void SavePlaylist() {
        std::ofstream f(proto.GetLine());
        if(!f.is_open())
            throw std::runtime_error("Can not open playlist");

        for(auto& s : music.GetList().GetSongList())
            f << s->GetFile() << std::endl;
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
            case Command::SORT_LLF:             return cmd.SortLLF();
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
            case Command::FILE_PUT:             return cmd.FilePut();
            case Command::CLEAR_PLAYLIST:       return cmd.ClearPlaylist();
            case Command::SAVE_FILE:            return cmd.SaveFile();
            case Command::SAVE_PLAYLIST:        return cmd.SavePlaylist();
        }
    }
private:
    Music& music;
};
