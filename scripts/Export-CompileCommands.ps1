$ProjectRoot = Split-Path -Parent $PSScriptRoot 
$File        = Join-Path $ProjectRoot "src\sld.cpp"

$Command = @(
    "cl.exe"
    $File
    "/Fo:" + @(Join-Path $ProjectRoot "build\debug\obj\SLD.Core.obj")  
    "/I"   + @(Join-Path $ProjectRoot "src")
    "/I"   + @(Join-Path $ProjectRoot "include")
    "/I"   + @(Join-Path $ProjectRoot "3rd-party")
    "/nologo"             # startup banner disabled
    "/c"                  # compile without linking
    "/MD"                 # link against multithreaded runtime library (MSVCRT.dll)
    "/Z7"                 # generate debug info
    "/EHs-"               # disable exception handling
    "/std:c++17"          # c++17 standard mode
    "/Od"                 # disable optimizations
    "/D_HAS_EXCEPTIONS=0" # disable exceptions for STL and CRT
    "/DSLD_DLL_EXPORT"
) -join " "

$CompileCommands  = @()
$CompileCommands += [PSCustomObject]@{
    directory = $ProjectRoot  
    command   = $Command
    file      = $File 
}

ConvertTo-Json -InputObject $CompileCommands -Depth 10 |
    Set-Content "compile_commands.json"
