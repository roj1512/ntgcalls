//
// Created by Laky64 on 18/08/2023.
//

#include "video_decoder_factory.hpp"

namespace wrtc {
    // TODO: Needed template like this:
    // https://github.com/pytgcalls/ntgcalls/blob/85ee93f72f223405174759b23eb222373e0bc775/wrtc/video_factory/base_video_factory.cpp

    std::unique_ptr<webrtc::VideoDecoder>
    VideoDecoderFactory::CreateVideoDecoder(const webrtc::SdpVideoFormat &format) {
        int n = 0;
        for (auto& enc : decoders) {
            auto supported_formats = formats_[n++];
            for (const auto& f : supported_formats) {
                if (f.IsSameCodec(format)) {
                    return enc.CreateVideoCodec(format);
                }
            }
        }
        return nullptr;
    }

    std::vector<webrtc::SdpVideoFormat> VideoDecoderFactory::GetSupportedFormats() const {
        formats_.clear();
        std::vector<webrtc::SdpVideoFormat> r;
        for (auto enc : decoders) {
            auto formats = enc.GetSupportedFormats();
            r.insert(r.end(), formats.begin(), formats.end());
            formats_.push_back(formats);
        }
        return r;
    }
} // wrtc