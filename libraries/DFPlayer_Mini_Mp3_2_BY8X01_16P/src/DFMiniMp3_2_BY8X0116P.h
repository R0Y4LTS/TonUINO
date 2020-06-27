/*-------------------------------------------------------------------------
BY8X01-16P library with the interface of DFMiniMp3 library.
Written by Timo Schulze.
 
Interface based on DFMiniMp3 library

Written by Michael C. Miller.

-------------------------------------------------------------------------

This software is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with NeoPixel.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------*/
#include <BY8X01-16P.h>

#pragma once

enum DfMp3_Error
{
    DfMp3_Error_Busy = 1,
    DfMp3_Error_Sleeping,
    DfMp3_Error_SerialWrongStack,
    DfMp3_Error_CheckSumNotMatch,
    DfMp3_Error_FileIndexOut,
    DfMp3_Error_FileMismatch,
    DfMp3_Error_Advertise,
    DfMp3_Error_General = 0xff
};


enum DfMp3_PlaybackMode
{
    DfMp3_PlaybackMode_Repeat,
    DfMp3_PlaybackMode_FolderRepeat,
    DfMp3_PlaybackMode_SingleRepeat,
    DfMp3_PlaybackMode_Random
};


enum DfMp3_Eq
{
    DfMp3_Eq_Normal,
    DfMp3_Eq_Pop,
    DfMp3_Eq_Rock,
    DfMp3_Eq_Jazz,
    DfMp3_Eq_Classic,
    DfMp3_Eq_Bass
};


enum DfMp3_PlaySource
{
    DfMp3_PlaySource_U,
    DfMp3_PlaySource_Sd,
    DfMp3_PlaySource_Aux,
    DfMp3_PlaySource_Sleep,
    DfMp3_PlaySource_Flash
};

const byte RANGE_FILE_INDEX = 254;
const byte STARTINDEX_SPOT_PLAY_FOLDER = 96;

template<class T_SERIAL_METHOD, class T_NOTIFICATION_METHOD> class DFMiniMp3
{
public:
    DFMiniMp3(T_SERIAL_METHOD& serial) :
        _serial(serial)
    {
        _audioController =  new BY8X0116P(_serial, 0);
    }

    void begin()
    {
        _serial.begin(9600);
        _serial.setTimeout(10000);
        _audioController->init();
        _lastPlaybackStatus = 0;
    }

    void loop()
    {
        bool playbackStatus = digitalRead(_busyPin);
        
        if ((_lastPlaybackStatus == 1) && (playbackStatus == 0)) // Flanke von Playing zu Non-Playing
        {
            _lastPlaybackStatus = playbackStatus;
            
            if (_audioController->getPlaybackStatus() == BY8X0116P_PlaybackStatus_Stopped) // Prüfen, ob stopped und nicht einfach paused
            {
                T_NOTIFICATION_METHOD::OnPlayFinished(random(65536));
            }
        }
       else
       {
           _lastPlaybackStatus = playbackStatus;
       }
    }

    // the track as enumerated across all folders
    void playGlobalTrack(uint16_t track = 0)
    {
        // not yet implemented
    }
    // value range of track: 0000 - 9999
    // sd:/mp3/####track name (DFMini only)
    
    // mapping to 8Y8X01
    //0000 - 0254 -> usb:/96/1 - usb:/96/255
    //0255 - 0509 -> usb:/97/1 - usb:/97/255
    //0510 - 0764 -> usb:/98/1 - usb:/98/255
    //0765 - 1019 -> usb:/99/1 - usb:/99/255

    void playMp3FolderTrack(uint16_t track)
    {
        byte folder, index;
        
        index = track % RANGE_FILE_INDEX + 1;
        folder = STARTINDEX_SPOT_PLAY_FOLDER + track / RANGE_FILE_INDEX;
        playFolderTrack(folder, index);
    }

    // sd:/###/###track name
    void playFolderTrack(uint8_t folder, uint8_t track)
    {
        _audioController->playFolderFileIndex(folder, track);
    }

    // sd:/##/####track name
    // track number must be four digits, zero padded
    void playFolderTrack16(uint8_t folder, uint16_t track)
    {
        // not yet implemented
    }

    void playRandomTrackFromAll()
    {
        // not yet implemented
    }

    void nextTrack()
    {
        // not yet implemented
    }

    void prevTrack()
    {
        // not yet implemented
    }

    uint16_t getCurrentTrack()
    {
        // not yet implemented
    }

    // 0- 30
    void setVolume(uint8_t volume)
    {
        _audioController->setVolume(volume);
    }

    uint8_t getVolume()
    {
        return _audioController->getVolume();
    }

    void increaseVolume()
    {
        _audioController->increaseVolume();
    }

    void decreaseVolume()
    {
        _audioController->decreaseVolume();
    }

    void loopGlobalTrack(uint16_t globalTrack)
    {
        // not yet implemented
    }

    DfMp3_PlaybackMode getPlaybackMode()
    {
        // not yet implemented
    }

    void setRepeatPlay(bool repeat)
    {
        // not yet implemented
    }


    void setEq(DfMp3_Eq eq)
    {
        // not yet implemented
    }

    DfMp3_Eq getEq()
    {
        // not yet implemented
    }


    void setPlaybackSource(DfMp3_PlaySource source)
    {
        // not yet implemented
    }

    void sleep()
    {
        // not yet implemented
    }

    void reset()
    {
        // not yet implemented
    }

    void start()
    {
        _audioController->play();
    }

    void pause()
    {
        _audioController->pause();
    }

    void stop()
    {
        _audioController->stop();
    }

    uint16_t getStatus()
    {
        // not yet implemented
    }

    uint16_t getFolderTrackCount(uint16_t folder)
    {
        // 1. Track aus dem Ordner folder anspielen, damit der Ordner folder aktueller Ordner wird.
        // Anschließend die Anzahl der Tracks im aktuellen Ordner bestimmen.
        // Lautstärke auf 0 stellen, damit man davon nichts hört.
        
        uint16_t count;
        
        uint8_t volume = _audioController->getVolume();
        _audioController->setVolume(0);
        
        byte folderIndex = (byte)constrain(folder, 0x00, 0x99);
        _audioController->playFolderFileIndex(folderIndex, 1);
        
        while (_audioController->getPlaybackStatus() != BY8X0116P_PlaybackStatus_Playing);
        {
            delay(10);
        }
        
        count = _audioController->getNumberOfTracksInCurrentFolder();
        
        _audioController->setVolume(volume);
        
        return count / 2; //Komischerweise wird immer die doppelte Anzahl zurückgegeben
    }

    uint16_t getTotalTrackCount()
    {
        // not yet implemented
    }

    uint16_t getTotalFolderCount()
    {
        // not yet implemented
    }

    // value range of track: 0000 - 9999
    // sd:/advert/####track name
    
    // mapping to 8Y8X01
    //0000 - 0254 -> usb:/96/1 - usb:/96/255
    //0255 - 0509 -> usb:/97/1 - usb:/97/255
    //0510 - 0764 -> usb:/98/1 - usb:/98/255
    //0765 - 1019 -> usb:/99/1 - usb:/99/255
    void playAdvertisement(uint16_t track)
    {
        byte folder, index;
        
        index = track % RANGE_FILE_INDEX + 1;
        folder = STARTINDEX_SPOT_PLAY_FOLDER + track / RANGE_FILE_INDEX;
        _audioController->spotPlayFolderFileIndex(folder, index);
    }

    void stopAdvertisement()
    {
        //not supported by BY8X01
    }
    
    BY8X0116P* getBY8X0116P()
    {
        return _audioController;
    }
    
    void setBusyPin(byte busyPin)
    {
        _busyPin = busyPin;
    }

private:
    T_SERIAL_METHOD& _serial;
    BY8X0116P* _audioController;
    byte _lastPlaybackStatus;
    byte _busyPin;

};
