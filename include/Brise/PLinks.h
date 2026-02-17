#pragma once

#include <Brise/PContact.h>

namespace Brise {
    class ParticleLink : public ParticleContactGenerator
    {
    public:
        Particle* particle[2];

    protected:
        float CurrentLength() const;

    public:
        virtual unsigned AddContact(ParticleContact& contact, unsigned limit) const = 0;
    };

    // CABLES

    class ParticleCable : public ParticleLink
    {
    public:
        float maxLength;
        float restitution;

    public:
        virtual unsigned AddContact(ParticleContact& contact, unsigned limit) const;
    };

    // RODS

    class ParticleRod : public ParticleLink {
    public:
        float length;

    public:
        virtual unsigned AddContact(ParticleContact& contact, unsigned limit) const;
    };

}