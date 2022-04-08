#ifndef RPG_RECT_H
#define RPG_RECT_H

template <typename T>
class Rect {
private:
    T m_left;
    T m_bottom;
    T m_width;
    T m_height;
public:
    Rect(T left, T bottom, T width, T height);

    inline T getLeft() const {
        return m_left;
    }

    inline T getBottom() const {
        return m_bottom;
    }

    inline T getWidth() const {
        return m_width;
    }

    inline T getHeight() const {
        return m_height;
    }

    inline bool contains(glm::vec2 pos) const
    {
        return pos.x > m_left && pos.x < m_left + m_width &&
               pos.y > m_bottom &&pos.y < m_bottom + m_height;
    }

    inline bool intersects(Rect<T> rect) const
    {
        return m_left < rect.m_left + rect.m_width &&
               m_left + m_width > rect.m_left &&
               m_bottom < rect.m_bottom + rect.m_height &&
               m_bottom + m_height > rect.m_bottom;
    }
};

template<typename T>
Rect<T>::Rect(T left, T bottom, T width, T height)
        : m_left(left), m_bottom(bottom), m_width(width), m_height(height) {

}

template<typename T, typename T2>
constexpr Rect<T> operator*(const Rect<T>& rect, T2 value)
{
    return Rect<T>(
        rect.getLeft() * value,
        rect.getBottom() * value,
        rect.getWidth() * value,
        rect.getHeight() * value
    );
}

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

#endif //RPG_RECT_H
