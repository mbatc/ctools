workspace "ctools"
  configurations {"Debug", "Release" }
  startproject "atEngine"

ctools_bin = "../../builds/bin"

win32Build = os.target() == "windows"
linuxBuild = os.target() == "linux"

dofile "setup/all-modules.lua"