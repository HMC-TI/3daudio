// All of this is coming from:
//https://developer.apple.com/library/mac/#documentation/MusicAudio/Conceptual/AudioQueueProgrammingGuide/AQPlayback/PlayingAudio.html#//apple_ref/doc/uid/TP40005343-CH3-SW1


#include <AudioQueue.h>

//Sets the number of audio queue buffers to use. Three is typically a good number, as described in “Audio Queue Buffers.”
static const int kNumberBuffers = 3;
struct AQPlayerState {
    AudioStreamBasicDescription   mDataFormat;
    // audio queue where we put next audio buffers
    AudioQueueRef                 mQueue;
    // points to buffers
    AudioQueueBufferRef           mBuffers[kNumberBuffers];
    // ??? We are not playing from a file....
    AudioFileID                   mAudioFile;
    // size of audio queue buffer
    UInt32                        bufferByteSize;
    //The packet index for the next packet to play from the audio file.
    SInt64                        mCurrentPacket;
    // The number of packets to read on each invocation of the audio queue’s playback callback
    // smallest units of encoded audio (like short? or double?)
    UInt32                        mNumPacketsToRead;
    // bit rate of audio. ours is constant so this should be NULL
    AudioStreamPacketDescription  *mPacketDescs;
    // whether the audio queue is running
    bool                          mIsRunning;
};


