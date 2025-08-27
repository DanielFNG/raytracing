#ifndef RAYTRACER_DYNAMICS_H
#define RAYTRACER_DYNAMICS_H

struct State
{
    Vec3 position{};
    Vec3 velocity{};
};

class Dynamics
{
public:
    virtual State at(double time) = 0;

    virtual ~Dynamics() = default;

    virtual std::unique_ptr<Dynamics> make_unique() const = 0;
};

class Static : public Dynamics
{
public:
    explicit Static(const Vec3& position)
        : position{position}
    {}

    State at([[maybe_unused]] double time) override
    {
        return {position, {0.0,0.0,0.0}};
    }

    std::unique_ptr<Dynamics> make_unique() const override
    {
        return std::make_unique<Static>(*this);
    }

private:
    Vec3 position{};
};

class Newtonian : public Dynamics
{
public:
    Newtonian(const Vec3& position, const Vec3& velocity, const Vec3& acceleration)
        : position{position}, velocity{velocity}, acceleration{acceleration}
    {}

    State at(const double time) override
    {
        return {position + velocity*time + 0.5*acceleration*time*time, velocity + acceleration*time};
    }

    std::unique_ptr<Dynamics> make_unique() const override
    {
        return std::make_unique<Newtonian>(*this);
    }

private:
    Vec3 position{};
    Vec3 velocity{};
    Vec3 acceleration{};
};


#endif //RAYTRACER_DYNAMICS_H