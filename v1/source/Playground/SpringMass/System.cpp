#include "System.hpp"

#include <cmath>
#include <limits>

// The types suffixed with '0' means they are the scratch data counterparts

struct Sm_PointMass0 {
    glm::vec2 netForce = {};
};

struct Sm_Spring0 {
    float length = std::numeric_limits<float>::quiet_NaN();
};

struct Sm_World0 {
    std::vector<Sm_PointMass0> pointMasses;
    std::vector<Sm_Spring0> springs;
};

#define SCRATCH_Sm_World(obj) auto d = static_cast<Sm_World0*>(obj._d);

Sm_World::Sm_World()
    : _d{ new Sm_World0() }
{
}

Sm_World::~Sm_World() {
    SCRATCH_Sm_World((*this));
    delete d;
}

static void IsMassImmovable(const Sm_PointMass& pm) {
    return std::isinf(pm.mass);
}

void Sm_StepSimulation(Sm_World& world, float dt) {
    SCRATCH_Sm_World(world);
    d.pointMasses.resize(world.pointMasses.size());
    d.springs.resize(world.springs.size());

    // FIXME this assumes each spring never changes id

    // Populate net force with basic gravity vector
    for (size_t i = 0; i < world.pointMasses.size(); ++i) {
        const auto& pm = world.pointMasses[i];
        auto& pm0 = d.pointMasses[i];
        if (!IsMassImmovable(pm)) {
            pm0.netForce += glm::vec2(0, world.gravity * pm.mass);
        }
    }

    for (size_t i = 0; i < world.springs.size(); ++i) {
        const auto& s = world.springs[i];
        if (s.snapped) continue;
        const auto& pmA = world.pointMasses[s.endpointA];
        const auto& pmB = world.pointMasses[s.endpointB];

        auto& s0 = d.springs[i];
        auto& pmA0 = d.pointMasses[s.endpointA];
        auto& pmB0 = d.pointMasses[s.endpointB];

        const auto lenVec = pmA.pos - pmB.pos
        // Recompute the spring length if it's not there from the last simulation step (e.g. newly added springs)
        if (std::isnan(s0.length)) {
            s0.length = glm::length(lenVec);
        }
        const auto dirVec = glm::normalize(lenVec);
        const float forceMag = springConstant * (len - natrualLength);
        // v - u yields a vector from u pointing to v
        if (!IsMassImmovable(pmA)) pmA0.netForce += -dirVec * forceMag;
        if (!IsMassImmovable(pmB)) pmB0.netForce += dirVec * forceMag;
    }

    for (size_t i = 0; i < world.pointMasses.size(); ++i) {
        const auto& pm = world.pointMasses[i];
        auto& pm0 = d.pointMasses[i];
        if (!IsMassImmovable(pm)) {
            pm.pos += vel * dt;

            auto acc = pm0.netForce / pm.mass;
            pm.vel += acc * dt;
        }
    }

    // Recompute spring lengths after the simulation step
    for (size_t i = 0; i < world.springs.size(); ++i) {
        const auto& s = world.springs[i];
        if (s.snapped) continue;
        const auto& pmA = world.pointMasses[s.endpointA];
        const auto& pmB = world.pointMasses[s.endpointB];

        auto& s0 = d.springs[i];

        s0.length = glm::length(pmA.pos - pmB.pos);
        if (s0.length > s.failureLength) {
            s.snapped = true;
        }
    }
}
