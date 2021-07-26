
-- Project Settings

project "ctools-data"
configurations { "Debug", "Release" }

kind "StaticLib"
architecture "x64"
language "C++"
characterset ("MBCS")

-- Set Directories

symbolspath '$(OutDir)$(TargetName).pdb'
targetdir (ctools_bin)
debugdir (ctools_bin)
objdir "../../builds/output/%{cfg.platform}_%{cfg.buildcfg}"

-- Project Flags

flags { "FatalWarnings" }
flags { "MultiProcessorCompile" }

-- Build Options

-- Linker options

linkoptions { "/ignore:4006" }
linkoptions { "/ignore:4221" }
linkoptions { "/ignore:4075" }

-- Dependencies

dependson("ctools-common")
dependson("ctools-math")

links { "ctools-common" }
links { "ctools-math" }

libdirs { ctools_bin }

-- Shared Defines

  defines { "_CRT_SECURE_NO_WARNINGS" }

-- Includes

  includedirs { "include" } 
  includedirs { "../common/include" }
  includedirs { "../math/include" }

-- Project Files

  files { "**.cpp", "**.h", "**.inl" }

-- Debug Configuration Settings

  filter { "configurations:Debug" }
    defines { "DEBUG"}
    symbols "On"
	  editandcontinue "On"

-- Release Configuration Settings

  filter { "configurations:Release" }
    flags { "LinkTimeOptimization" }
    defines { "NDEBUG" }
    optimize "On"
	  editandcontinue "Off"

  filter {}
