#ifndef RPG_VERTEX_ARRAY_H
#define RPG_VERTEX_ARRAY_H

class VertexArray final
{
private:
    unsigned int m_id{};
public:
    VertexArray();

    void bind();
    void unbind();

    void destroy();

    unsigned int getId() const;
};


#endif //RPG_VERTEX_ARRAY_H