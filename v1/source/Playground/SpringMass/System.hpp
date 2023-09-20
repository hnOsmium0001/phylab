#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Sm_PointMass {
    // Unit: (m,m)
    glm::vec2 pos;
    glm::vec2 vel;
    // If +Inf, this point mass is considered immovable
    float mass;
};

struct Sm_Spring {
    // Connected Sm_PointMass objects, must be valid
    size_t endpointA;
    size_t endpointB;
    // We assume this is an ideal spring, i.e. this has unit N/m
    float springConstant;
    float natrualLength;
    float failureLength;
    bool snapped;
};

struct Sm_World {
    // Internal book-keeping data
    void* _d;
    std::vector<Sm_PointMass> pointMasses;
    std::vector<Sm_Spring> springs;
    // Unit: m/s^2
    float gravity;

    Sm_World();
    ~Sm_World();
};

void Sm_StepSimulation(Sm_World& world, float dt);

struct Sm_ShowOptions {
};

void Sm_Show(Sm_World& world, const Sm_ShowOptions& opt);
