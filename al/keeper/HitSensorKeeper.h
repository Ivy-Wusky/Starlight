#pragma once

#include "types.h"
#include "sead/matrix.h"
#include "sead/vector.h"

namespace al
{
    class LiveActor;
    class HitSensor;

    class HitSensorKeeper
    {
    public:
        HitSensorKeeper(s32 numSensors);

        void* addSensor(al::LiveActor *, char const *, u32, f32, u16, sead::Vector3<f32> const *, sead::Matrix34<f32> const *,sead::Vector3<f32> const &);
        void update();
        u32 getSensorNum() const;
        al::HitSensor* getSensor(s32 sensorID) const;
        void attackSensor();
        void clear();
        void validate();
        void invalidate();
        void validateBySystem();
        void invalidateBySystem();
        u64* getSystem(char const *) const; // al::HitSensor*

        u32 mNumSensors; // _4
        al::HitSensor* mSensors; // _8
    };
};