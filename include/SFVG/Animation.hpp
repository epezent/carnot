#ifndef SFVG_ANIMATION_HPP
#define SFVG_ANIMATION_HPP

#include <SFVG/KeyFrame.hpp>
#include <vector>

namespace sfvg {

template <typename... TProperties>
class Animation {
public:
    Animation(std::size_t count = 0) { setKeyFrameCount(count); }

    // void setSubject(TSubject* subject);

    KeyFrame<TProperties...>& getKeyFrame(std::size_t index) {
        return m_keyFrames[index];
    }

    KeyFrame<TProperties...>& addKeyFrame() {
        m_keyFrames.resize(m_keyFrames.size() + 1);
        return m_keyFrames[m_keyFrames.size() - 1];
    }

    void setKeyFrameCount(std::size_t count) { m_keyFrames.resize(count); }

    template <typename TSubject>
    void applyKeyFrame(std::size_t index, TSubject* subject) {
        m_keyFrames[index].apply(subject);
    }

private:

    std::vector<KeyFrame<TProperties...>> m_keyFrames;
};

}  // namespace sfvg

#endif  // SFVG_ANIMATION_HPP
