//
// Created by Laky64 on 18/08/2023.
//

#pragma once

#include <api/video_codecs/video_decoder_factory.h>

#include "video_base_config.hpp"

namespace wrtc {

    class VideoDecoderConfig: public VideoBaseConfig {
    public:
        VideoDecoderConfig() = default;

        ~VideoDecoderConfig();

        VideoDecoderConfig(webrtc::VideoCodecType codec, DecoderCallback createVideoDecoder);

        VideoDecoderConfig(FormatsRetriever getSupportedFormats, DecoderCallback createVideoDecoder);

        VideoDecoderConfig(std::unique_ptr<webrtc::VideoDecoderFactory> factory): factory(std::move(factory)) {}

        std::unique_ptr<webrtc::VideoDecoder> CreateVideoCodec(const webrtc::SdpVideoFormat& format);

    private:
        DecoderCallback decoder;
        std::shared_ptr<webrtc::VideoDecoderFactory> factory;

        bool isInternal() override;

        std::vector<webrtc::SdpVideoFormat> getInternalFormats() override;
    };

} // wrtc
