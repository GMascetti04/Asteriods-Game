project "OpenAL"
    kind "StaticLib"
    language "C++"


    targetdir ("out/bin/" .. output_directory .. "/%{prj.name}")
    objdir ("out/bin-int/" .. output_directory .. "/%{prj.name}")
    


    includedirs
    {
        "include/",
        "build/",
        "",
        "common/"
    }

    defines
    {
        "AL_LIBTYPE_STATIC",
        "RESTRICT=__restrict",
        "AL_ALEXT_PROTOTYPES",
        "NOMINMAX"
    }

    files
    {
        "al/auxeffectslot.cpp" ,
        "al/auxeffectslot.h" ,
        "al/buffer.cpp" ,
        "al/buffer.h" ,
        "al/effect.cpp" ,
        "al/effect.h" ,
        "al/effects/autowah.cpp",
        "al/effects/chorus.cpp",
        "al/effects/compressor.cpp",
        "al/effects/convolution.cpp",
        "al/effects/dedicated.cpp",
        "al/effects/distortion.cpp",
        "al/effects/echo.cpp",
        "al/effects/effects.h" ,
        "al/effects/equalizer.cpp",
        "al/effects/fshifter.cpp",
        "al/effects/modulator.cpp",
        "al/effects/null.cpp",
        "al/effects/pshifter.cpp",
        "al/effects/reverb.cpp",
        "al/effects/vmorpher.cpp",
        "al/error.cpp" ,
        "al/event.cpp" ,
        "al/event.h" ,
        "al/extension.cpp" ,
        "al/filter.cpp" ,
        "al/filter.h" ,
        "al/listener.cpp" ,
        "al/listener.h" ,
        "al/source.cpp" ,
        "al/source.h" ,
        "al/state.cpp" ,
        "alc/alc.cpp" ,
        "alc/alu.cpp" ,
        "alc/alu.h" ,
        "alc/alconfig.cpp" ,
        "alc/alconfig.h" ,
        "alc/context.cpp",
        "alc/context.h" ,
        "alc/device.cpp",
        "alc/device.h" ,
        "alc/effectslot.cpp" ,
        "alc/effectslot.h" ,
        "alc/effects/base.h" ,
        "alc/effects/autowah.cpp",
        "alc/effects/chorus.cpp",
        "alc/effects/compressor.cpp",
        "alc/effects/convolution.cpp",
        "alc/effects/dedicated.cpp",
        "alc/effects/distortion.cpp",
        "alc/effects/echo.cpp",
        "alc/effects/equalizer.cpp",
        "alc/effects/fshifter.cpp",
        "alc/effects/modulator.cpp",
        "alc/effects/null.cpp",
        "alc/effects/pshifter.cpp",
        "alc/effects/reverb.cpp",
        "alc/effects/vmorpher.cpp",
        "alc/inprogext.h" ,
        "alc/panning.cpp" ,
        "alc/backends/base.cpp" ,
        "alc/backends/base.h" ,
        "alc/backends/loopback.cpp" ,
        "alc/backends/loopback.h" ,
        "alc/backends/null.cpp",
        "alc/backends/null.h" ,
        "alc/backends/winmm.cpp" ,
        "alc/backends/winmm.h" ,
        "alc/backends/dsound.cpp" ,
        "alc/backends/dsound.h" ,
        "alc/backends/wasapi.cpp" ,
        "alc/backends/wasapi.h" ,
        "alc/backends/wave.cpp" ,
        "alc/backends/wave.h" ,
        "build/hrtf_default.h" ,
        "core/ambdec.cpp" ,
        "core/ambdec.h" ,
        "core/ambidefs.cpp" ,
        "core/ambidefs.h" ,
        "core/async_event.h" ,
        "core/bformatdec.cpp" ,
        "core/bformatdec.h" ,
        "core/bs2b.cpp" ,
        "core/bs2b.h" ,
        "core/bsinc_defs.h" ,
        "core/bsinc_tables.cpp" ,
        "core/bsinc_tables.h" ,
        "core/bufferline.h" ,
        "core/buffer_storage.cpp" ,
        "core/buffer_storage.h" ,
        "core/context.cpp",
        "core/context.h" ,
        "core/converter.cpp" ,
        "core/converter.h" ,
        "core/cpu_caps.cpp" ,
        "core/cpu_caps.h" ,
        "core/devformat.cpp" ,
        "core/devformat.h" ,
        "core/device.cpp",
        "core/device.h" ,
        "core/except.cpp" ,
        "core/except.h" ,
        "core/filters/biquad.h" ,
        "core/filters/biquad.cpp" ,
        "core/filters/nfc.cpp" ,
        "core/filters/nfc.h" ,
        "core/filters/splitter.cpp" ,
        "core/filters/splitter.h" ,
        "core/fmt_traits.cpp" ,
        "core/fmt_traits.h" ,
        "core/fpu_ctrl.cpp" ,
        "core/fpu_ctrl.h" ,
        "core/front_stablizer.h" ,
        "core/helpers.cpp" ,
        "core/helpers.h" ,
        "core/hrtf.cpp" ,
        "core/hrtf.h" ,
        "core/logging.cpp" ,
        "core/logging.h" ,
        "core/mastering.cpp" ,
        "core/mastering.h" ,
        "core/mixer.cpp" ,
        "core/mixer.h" ,
        "core/resampler_limits.h" ,
        "core/uhjfilter.cpp" ,
        "core/uhjfilter.h" ,
        "core/uiddefs.cpp" ,
        "core/voice.cpp" ,
        "core/voice.h" ,
        "core/voice_change.h" ,
        "core/mixer/defs.h" ,
        "core/mixer/hrtfbase.h" ,
        "core/mixer/hrtfdefs.h" ,
        "core/mixer/mixer_c.cpp" ,
        "core/mixer/mixer_sse.cpp" ,
        "core/mixer/mixer_sse2.cpp" ,
        "core/mixer/mixer_sse3.cpp" ,
        "core/mixer/mixer_sse41.cpp" ,
        "include/AL/al.h" ,
        "include/AL/alc.h" ,
        "include/AL/alext.h" ,
        "include/AL/efx.h" ,
        "common/almalloc.cpp",
        "common/alstring.cpp",
        "common/strutils.cpp",
        "common/**.cpp",

        "resources/openal32.rc" 

    }


    filter "system:windows"
    systemversion "latest"
    staticruntime "On"


filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    runtime "Release"
    optimize "on"