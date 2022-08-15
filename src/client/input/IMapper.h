#ifndef RPG_IMAPPER_H
#define RPG_IMAPPER_H

template <typename T1, typename T2> class IMapper
{
public:
    virtual ~IMapper() = default;

    virtual T2 map(T1 key) = 0;

};

#endif // RPG_IMAPPER_H
