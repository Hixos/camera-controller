/**
 * Copyright (c) 2022 Luca Erbetta
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include <string>

#include "camera/CameraWrapper.h"
using std::string;

struct CameraCommonConfig
{
    gphotow::RadioMenuConfig shutter_speed;
    gphotow::RadioMenuConfig aperture;
    gphotow::RadioMenuConfig iso;
    int focal_length;
    string mode;
    string focus_mode;
    bool long_exposure_nr;
    bool vr;
    int battery;
};

template <>
struct fmt::formatter<CameraCommonConfig>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(CameraCommonConfig const& cfg, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(),
                              "( ss = {}, f-n = {}, iso = {}, fl = {}, mode = "
                              "{}, focus = {}, nr = {}, vr = {}, bat% = {} )",
                              cfg.shutter_speed, cfg.aperture, cfg.iso,
                              cfg.focal_length, cfg.mode, cfg.focus_mode,
                              cfg.long_exposure_nr, cfg.vr, cfg.battery);
    }
};

template <>
struct fmt::formatter<gphotow::RadioMenuConfig>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(gphotow::RadioMenuConfig const& radio, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(), "{}", radio.name);
    }
};