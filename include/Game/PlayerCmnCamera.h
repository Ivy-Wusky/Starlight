#pragma once

#include "Cmn/CameraBase.h"
#include "types.h"

namespace Game {
    class PlayerCmnCamera : Cmn::CameraBase  {
        public:
        PlayerCmnCamera();
        ~PlayerCmnCamera();
        void onCalc();
        void genMessage(__int64 *); // Lp::Sys::HioContext *
        bool checkDerivedRuntimeTypeInfo(__int64 const*)const; // sead::RuntimeTypeInfo::Interface const*
        __int64 *getRuntimeTypeInfo(); // idk what this returns
    };
};