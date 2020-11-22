workspace "ctools"
  configurations {"Debug", "Release" }
  startproject "atEngine"

  ctools_bin = "../../builds/bin"

dofile "modules/common/project.lua"
  location "projects/common/"

dofile "modules/math/project.lua"
  location "projects/atLib/"
  
dofile "modules/data/project.lua"
  location "projects/atLib/"
  
dofile "modules/platform/project.lua"
  location "projects/atLib/"

dofile "modules/test/project.lua"
  location "projects/atLib/"
