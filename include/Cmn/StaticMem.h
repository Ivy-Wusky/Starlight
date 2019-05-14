#pragma once

#include "types.h"
#include "sead/string.h"
#include "Cmn/PlayerInfoAry.h"

namespace Cmn
{
    class StaticMem {
        public:
        static StaticMem* sInstance;

        _QWORD vtable;
        _QWORD qword8;
        _BYTE gap10[32];
        const char *pchar30;
        Cmn::PlayerInfoAry *playerInfoArray;
        _DWORD random;
        _BYTE gap44[4];
        sead::SafeStringBase<char> stageName;
    };
    class StaticMem
    {
        public:
        static StaticMem* sInstance;

        void* __vftable;
        u64 staticMem_x8;
        sead::IDisposer mDisposer;
        u64 staticMem_x30;
        Cmn::PlayerInfoAry *mPlayerInfoAry;
        u32 mTeam;
        u32 staticMem_x44;
        sead::BufferedSafeStringBase<char> mMapFileName1;
        char mMapFileNameData1[0x100];
        sead::BufferedSafeStringBase<char> mMapFileName2;
        char mMapFileNameData2[0x100];
        sead::BufferedSafeStringBase<char> mMapFileName3;
        char mMapFileNameData3[0x100];
        u64 staticMem_x390;
        u32 *mSceneHistory;
        u64 staticMem_x3A0;
        u32 mSceneHistoryCount;
        char gap_x3AC[0x18];
        u32 mDbgGameTime;
        u32 mVersusMode;
        u32 mVersusRule;
        u64 staticMem_x3D0;
        u64 staticMem_x3D8;
        sead::DateTime staticMem_x3E0;
        u64 staticMem_x3E8;
        Cmn::PlayerInfo mPlayerInfo;
        char gap_x590[0x1CC];
        u32 mMatchmakeTagPlayerNum;
        u32 mMatchmakeTagID;
        char gap_x764[0x24];
        sead::PtrArrayImpl staticMem_x788;
        char gap_x798[0x420];
        Lp::Net::FriendListHolder* mFriendListHolder;
        sead::DateTime staticMem_xBC0;
        char gap_xBC8[0x160];
        Cmn::Def::Mission::WorldAreaVolatileSave mWorldAreaVolatileSave[5];
        char gap_xDC8[0x24];
        u32 mAddonLoaded;
        u64* mNewInstalledAocGear;
        u32 mNewInstalledAocGearCount;
        char gap_xDFC[0x70];
        Cmn::Def::AmiiboData mAmiiboData;
    };
};