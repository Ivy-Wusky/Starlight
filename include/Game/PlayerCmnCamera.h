#pragma once

#include "Cmn/CameraBase.h"
#include "types.h"

namespace Game {
    class PlayerCmnCamera : Cmn::CameraBase  {
        public:
        PlayerCmnCamera();
        ~PlayerCmnCamera();
        void onCalc() override;
        void genMessage(__int64 *) override; // Lp::Sys::HioContext *
        bool checkDerivedRuntimeTypeInfo(__int64 const*)const override; // sead::RuntimeTypeInfo::Interface const*
        __int64 *getRuntimeTypeInfo() override; // idk what this returns
    };
};