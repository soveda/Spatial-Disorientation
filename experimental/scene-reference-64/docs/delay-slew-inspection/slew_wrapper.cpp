// Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
#include "fixed.h"
extern "C" int old8(int a,int b){int d=b-a;return a+(d==0?0:d/256+(d>0?1:-1));}
extern "C" int new8(int a,int b){return spatial::SlewPower<8>(a,b);}
