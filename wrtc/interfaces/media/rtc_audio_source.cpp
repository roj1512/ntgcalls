//
// Created by Laky64 on 19/08/2023.
//

#include "rtc_audio_source.hpp"


namespace wrtc {
    RTCAudioSource::RTCAudioSource() {
        factory = PeerConnectionFactory::GetOrCreateDefault();
        source = new rtc::RefCountedObject<AudioTrackSource>();
    }

    RTCAudioSource::~RTCAudioSource() {
        factory = nullptr;
        source = nullptr;
        PeerConnectionFactory::UnRef();
    }

    MediaStreamTrack *RTCAudioSource::createTrack() {
        return MediaStreamTrack::holder()->GetOrCreate(
            factory->factory()->CreateAudioTrack(rtc::CreateRandomUuid(), source.get())
        );
    }

    void RTCAudioSource::OnData(RTCOnDataEvent &data) {
        source->PushData(data);
    }
} // wrtc