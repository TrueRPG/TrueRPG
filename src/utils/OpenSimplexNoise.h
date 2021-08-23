#ifndef RPG_OPENSIMPLEXNOISE_H
#define RPG_OPENSIMPLEXNOISE_H

#include "Types.h"
#include <array>

#define PSIZE 2048
#define MAX_OCTAVES 9

class OpenSimplexNoise
{
private:

    struct SimplexCtx
    {
        i16 m_perm[PSIZE];
    } m_contexts[MAX_OCTAVES];

    static const std::array<i8, 16> m_gradients2D;
    u64 m_seed;
    i32 m_octaves;
    float m_lacunarity;
    float m_persistence;
    float m_period;
public:
    explicit OpenSimplexNoise(u64 seed, i32 octaves = 2, float lacunarity = 2.f, float persistence = .5f, float period = 32.f);
    OpenSimplexNoise();
    ~OpenSimplexNoise() = default;

    double getNoise(double x, double y) const;

    void setSeed(u64 seed);
    u64 getSeed() const;

    void setOctaves(int octaves);
    i32 getOctaves() const;

    void setLacunarity(float lacunarity);
    float getLacunarity() const;

    void setPersistence(float persistence);
    float getPersistence() const;

    void setPeriod(float period);
    float getPeriod() const;

private:
    void initContext(SimplexCtx& ctx, u64 seed);
    double eval(const SimplexCtx& ctx, double x, double y) const;
    double extrapolate(const SimplexCtx& ctx, i32 xsb, i32 ysb, double dx, double dy) const;

    void updateSeed();
};

#endif // RPG_OPENSIMPLEXNOISE_H