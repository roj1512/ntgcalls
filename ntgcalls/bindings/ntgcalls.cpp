//
// Created by Laky64 on 29/08/2023.
//

#include <iostream>
#include "../ntgcalls.hpp"
#include "ntgcalls.h"

std::map<uint32_t, std::shared_ptr<ntgcalls::NTgCalls>> clients;
uint64_t uidGenerator;

int copyAndReturn(std::string s, char *buffer, int size) {
    if (!buffer)
        return int(s.size() + 1);

    if (size < int(s.size() + 1))
        return NTG_ERR_TOO_SMALL;

    std::copy(s.begin(), s.end(), buffer);
    buffer[s.size()] = '\0';
    return int(s.size() + 1);
}

template <typename T> int copyAndReturn(std::vector<T> b, T *buffer, int size) {
    if (!buffer)
        return int(b.size());

    if (size < int(b.size()))
        return NTG_ERR_TOO_SMALL;
    std::copy(b.begin(), b.end(), buffer);
    return int(b.size());
}

std::shared_ptr<ntgcalls::NTgCalls> safeUID(uint32_t uid) {
    if (clients.find(uid) == clients.end()) {
        throw ntgcalls::InvalidUUID("UUID" + std::to_string(uid) + " not found");
    }
    return clients[uid];
}

ntgcalls::BaseMediaDescription::InputMode parseInputMode(ntgInputMode mode) {
    switch (mode) {
        case ntgInputMode::NTG_FILE:
            return ntgcalls::BaseMediaDescription::InputMode::File;
        case ntgInputMode::NTG_SHELL:
            return ntgcalls::BaseMediaDescription::InputMode::Shell;
        case ntgInputMode::NTG_FFMPEG:
            return ntgcalls::BaseMediaDescription::InputMode::FFmpeg;
    }
}

ntgStreamStatus parseStatus(ntgcalls::Stream::Status status) {
    switch (status) {
        case ntgcalls::Stream::Playing:
            return ntgStreamStatus::NTG_PLAYING;
        case ntgcalls::Stream::Paused:
            return ntgStreamStatus::NTG_PAUSED;
        case ntgcalls::Stream::Idling:
            return ntgStreamStatus::NTG_IDLING;
    }
}

ntgcalls::MediaDescription parseMediaDescription(ntgMediaDescription& desc) {
    std::optional<ntgcalls::AudioDescription> audio;
    std::optional<ntgcalls::VideoDescription> video;
    if (desc.audio) {
        switch (desc.audio->inputMode) {
            case ntgInputMode::NTG_FILE:
            case ntgInputMode::NTG_SHELL:
                audio = ntgcalls::AudioDescription(
                    parseInputMode(desc.audio->inputMode),
                    desc.audio->sampleRate,
                    desc.audio->bitsPerSample,
                    desc.audio->channelCount,
                    std::string(desc.audio->input)
                );
                break;
            case ntgInputMode::NTG_FFMPEG:
                throw ntgcalls::FFmpegError("Not supported");
        }
    }
    if (desc.video) {
        switch (desc.audio->inputMode) {
            case ntgInputMode::NTG_FILE:
            case ntgInputMode::NTG_SHELL:
                video = ntgcalls::VideoDescription(
                    parseInputMode(desc.audio->inputMode),
                    desc.video->width,
                    desc.video->height,
                    desc.video->fps,
                    std::string(desc.video->input)
                );
                break;
            case ntgInputMode::NTG_FFMPEG:
                throw ntgcalls::FFmpegError("Not supported");
        }
    }
    return ntgcalls::MediaDescription(
        audio,
        video
    );
}

uint32_t ntg_init() {
    int uid = uidGenerator++;
    clients[uid] = std::make_shared<ntgcalls::NTgCalls>();
    return uid;
}

int ntg_destroy(uint32_t uid) {
    if (clients.find(uid) == clients.end()) {
        return NTG_INVALID_UID;
    }
    clients.erase(clients.find(uid));
    return 0;
}

int ntg_get_params(uint32_t uid, int64_t chatID, ntgMediaDescription desc, char* buffer, int size) {
    try {
        return copyAndReturn(safeUID(uid)->createCall(chatID, parseMediaDescription(desc)), buffer, size);
    } catch (ntgcalls::InvalidUUID) {
        return NTG_INVALID_UID;
    } catch (ntgcalls::ConnectionError) {
        return NTG_CONNECTION_ALREADY_EXISTS;
    } catch (ntgcalls::FileError) {
        return NTG_FILE_NOT_FOUND;
    } catch (ntgcalls::InvalidParams) {
        return NTG_ENCODER_NOT_FOUND;
    } catch (ntgcalls::FFmpegError) {
        return NTG_FFMPEG_NOT_FOUND;
    } catch (ntgcalls::ShellError) {
        return NTG_SHELL_ERROR;
    } catch (...) {
        return NTG_UNKNOWN_EXCEPTION;
    }
}

int ntg_connect(uint32_t uid, int64_t chatID, char* params) {
    try {
        safeUID(uid)->connect(chatID, std::string(params));
    } catch (ntgcalls::RTMPNeeded) {
        return NTG_RTMP_NEEDED;
    } catch (ntgcalls::InvalidParams) {
        return NTG_INVALID_TRANSPORT;
    } catch (ntgcalls::ConnectionError) {
        return NTG_CONNECTION_FAILED;
    } catch (ntgcalls::ConnectionNotFound) {
        return NTG_CONNECTION_NOT_FOUND;
    } catch (...) {
        return NTG_UNKNOWN_EXCEPTION;
    }
    return 0;
}

int ntg_change_stream(uint32_t uid, int64_t chatID, ntgMediaDescription desc) {
    try {
        safeUID(uid)->changeStream(chatID, parseMediaDescription(desc));
    } catch (ntgcalls::InvalidUUID) {
        return NTG_INVALID_UID;
    } catch (ntgcalls::FileError) {
        return NTG_FILE_NOT_FOUND;
    } catch (ntgcalls::InvalidParams) {
        return NTG_ENCODER_NOT_FOUND;
    } catch (ntgcalls::FFmpegError) {
        return NTG_FFMPEG_NOT_FOUND;
    } catch (ntgcalls::ShellError) {
        return NTG_SHELL_ERROR;
    } catch (ntgcalls::ConnectionNotFound) {
        return NTG_CONNECTION_NOT_FOUND;
    } catch (...) {
        return NTG_UNKNOWN_EXCEPTION;
    }
    return 0;
}

bool ntg_pause(uint32_t uid, int64_t chatID) {
    try {
        return safeUID(uid)->pause(chatID);
    } catch (...) {}
    return false;
}

bool ntg_resume(uint32_t uid, int64_t chatID) {
    try {
        return safeUID(uid)->resume(chatID);
    } catch (...) {}
    return false;
}

bool ntg_mute(uint32_t uid, int64_t chatID) {
    try {
        return safeUID(uid)->mute(chatID);
    } catch (...) {}
    return false;
}

bool ntg_unmute(uint32_t uid, int64_t chatID) {
    try {
        return safeUID(uid)->unmute(chatID);
    } catch (...) {}
    return false;
}

int ntg_stop(uint32_t uid, int64_t chatID) {
    try {
        safeUID(uid)->stop(chatID);
    } catch (ntgcalls::InvalidUUID) {
        return NTG_INVALID_UID;
    } catch (ntgcalls::ConnectionNotFound) {
        return NTG_CONNECTION_NOT_FOUND;
    }
    return 0;
}

uint64_t ntg_time(uint32_t uid, int64_t chatID) {
    try {
        return safeUID(uid)->time(chatID);
    } catch (...) {
        return 0;
    }
}

int ntg_calls(uint32_t uid, ntgGroupCall *buffer, int size) {
    try {
        auto callsCpp = safeUID(uid)->calls();
        std::vector<ntgGroupCall> groupCalls;
        for (auto call : callsCpp) {
            groupCalls.push_back(ntgGroupCall{
                call.first,
                parseStatus(call.second),
            });
        }
        return copyAndReturn(groupCalls, buffer, size);
    } catch (ntgcalls::InvalidUUID) {
        return NTG_INVALID_UID;
    }
}

int ntg_calls_count(uint32_t uid) {
    try {
        return safeUID(uid)->calls().size();
    } catch (ntgcalls::InvalidUUID) {
        return NTG_INVALID_UID;
    }
}

int ntg_on_stream_end(uint32_t uid, ntgStreamEndCallback callback) {
    try {
        safeUID(uid)->onStreamEnd([uid, callback](int64_t chatId, ntgcalls::Stream::Type type) {
            callback(uid, chatId, type == ntgcalls::Stream::Type::Audio ? ntgStreamType::NTG_STREAM_AUDIO:ntgStreamType::NTG_STREAM_VIDEO);
        });
    } catch (ntgcalls::InvalidUUID) {
        return NTG_INVALID_UID;
    } catch (...) {
        return NTG_UNKNOWN_EXCEPTION;
    }
    return 0;
}

int ntg_on_upgrade(uint32_t uid, ntgUpgradeCallback callback) {
    try {
        safeUID(uid)->onUpgrade([uid, callback](int64_t chatId, ntgcalls::MediaState state) {
            callback(uid, chatId, ntgMediaState{
                state.muted,
                state.videoPaused,
                state.videoStopped,
            });
        });
    } catch (ntgcalls::InvalidUUID) {
        return NTG_INVALID_UID;
    }
    return 0;
}