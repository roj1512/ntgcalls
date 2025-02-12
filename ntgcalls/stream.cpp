//
// Created by Laky64 on 12/08/2023.
//

#include "stream.hpp"

namespace ntgcalls {
    Stream::Stream() {
        audio = std::make_shared<AudioStreamer>();
        video = std::make_shared<VideoStreamer>();
        dispatchQueue = std::make_shared<DispatchQueue>("StreamQueue_" + rtc::CreateRandomUuid());
    }

    Stream::~Stream() {
        stop();
        audio = nullptr;
        video = nullptr;
        audioTrack = nullptr;
        videoTrack = nullptr;
        reader = nullptr;
        running = false;
    }

    void Stream::addTracks(const std::shared_ptr<wrtc::PeerConnection>& pc) {
        audioTrack = audio->createTrack();
        videoTrack = video->createTrack();
        pc->addTrack(audioTrack);
        pc->addTrack(videoTrack);
    }

    std::pair<std::shared_ptr<BaseStreamer>, std::shared_ptr<BaseReader>> Stream::unsafePrepareForSample() {
        std::shared_ptr<BaseStreamer> bs;
        std::shared_ptr<BaseReader> br;
        if (reader->audio && reader->video) {
            if (audio->nanoTime() <= video->nanoTime()) {
                bs = audio;
                br = reader->audio;
            } else {
                bs = video;
                br = reader->video;
            }
        } else if (reader->audio) {
            bs = audio;
            br = reader->audio;
        } else {
            bs = video;
            br = reader->video;
        }

        auto waitTime = bs->waitTime();
        if (waitTime.count() > 0) {
            std::this_thread::sleep_for(waitTime);
        }
        return {bs, br};
    }

    void Stream::checkStream() {
        if (reader->audio && reader->audio->eof()) {
            reader->audio = nullptr;
            onEOF(Audio);
        }
        if (reader->video && reader->video->eof()) {
            reader->video = nullptr;
            onEOF(Video);
        }
    }

    void Stream::sendSample() {
        if (idling || !(reader->audio || reader->video)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } else {
            auto bsBR = unsafePrepareForSample();
            auto sample = bsBR.second->read(bsBR.first->frameSize());
            bsBR.first->sendData(sample);
            if (sample != nullptr) delete[] sample;
            checkStream();
        }

        if (running) {
            dispatchQueue->dispatch([this]() {
                sendSample();
            });
        }
    }

    void Stream::setAVStream(MediaDescription streamConfig, bool noUpgrade) {
        auto audioConfig = streamConfig.audio;
        auto videoConfig = streamConfig.video;
        reader = std::make_shared<MediaReaderFactory>(streamConfig);
        idling = false;
        if (audioConfig) {
            audio->setConfig(
                audioConfig->sampleRate,
                audioConfig->bitsPerSample,
                audioConfig->channelCount
            );
        }
        bool wasVideo = hasVideo;
        if (videoConfig) {
            hasVideo = true;
            video->setConfig(
                videoConfig->width,
                videoConfig->height,
                videoConfig->fps
            );
        } else {
            hasVideo = false;
        }
        if (wasVideo != hasVideo && !noUpgrade) {
            checkUpgrade();
        }
    }

    void Stream::checkUpgrade() {
        onChangeStatus(MediaState{
            audioTrack->isMuted() && videoTrack->isMuted(),
            hasVideo,
            idling
        });
    }

    uint64_t Stream::time() {
        if (reader) {
            if (reader->audio && reader->video) {
                return (audio->time() + video->time()) / 2;
            } else if (reader->audio) {
                return audio->time();
            } else if (reader->video) {
                return video->time();
            }
        }
        return 0;
    }

    Stream::Status Stream::status() {
        if ((reader->audio || reader->video) && running) {
            return idling ? Status::Paused : Status::Playing;
        }
        return Status::Idling;
    }

    void Stream::start() {
        if (!running) {
            running = true;
            dispatchQueue->dispatch([this]() {
                sendSample();
            });
        }
    }

    bool Stream::pause() {
        auto res = std::exchange(idling, true);
        checkUpgrade();
        return !res;
    }

    bool Stream::resume() {
        auto res = std::exchange(idling, false);
        checkUpgrade();
        return res;
    }

    bool Stream::mute() {
        if (!audioTrack->isMuted() || !videoTrack->isMuted()) {
            audioTrack->Mute(true);
            videoTrack->Mute(true);
            checkUpgrade();
            return true;
        } else {
            checkUpgrade();
            return false;
        }
    }

    bool Stream::unmute() {
        if (audioTrack->isMuted() || videoTrack->isMuted()) {
            audioTrack->Mute(false);
            videoTrack->Mute(false);
            checkUpgrade();
            return true;
        } else {
            checkUpgrade();
            return false;
        }
    }

    void Stream::stop() {
        running = false;
        idling = false;
        dispatchQueue = nullptr;
        if (reader) {
            if (reader->audio) {
                reader->audio->close();
            }
            if (reader->video) {
                reader->video->close();
            }
        }
    }

    void Stream::onStreamEnd(std::function<void(Stream::Type)> &callback) {
        onEOF = callback;
    }

    void Stream::onUpgrade(std::function<void(MediaState)> &callback) {
        onChangeStatus = callback;
    }
}