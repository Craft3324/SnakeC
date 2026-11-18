---@diagnostic disable: undefined-global

set_project("SnakeC")
set_version("1.0.0")

add_rules("mode.debug", "mode.release")

set_languages("c23")
set_warnings("all", "error")

target("SnakeC")
set_kind("binary")

add_files("src/*.c", "src/screens/*.c", "src/game/*.c")
add_includedirs("include")

if is_plat("windows") then
    add_files("src/platform/windows.c")
elseif is_plat("linux", "macosx", "bsd") then
    add_files("src/platform/posix.c")
end
