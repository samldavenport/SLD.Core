$OutDirs = [PSCustomObject]@{
    Bin = "build\debug\bin"
    Obj = "build\debug\obj"
    Lib = "build\debug\lib"
}

$OutFiles = [PSCustomObject]@{
    Obj = $OutDirs.Obj + "\SLD.Core.obj"
    Pdb = $OutDirs.Bin + "\SLD.Core.pdb" 
    Lib = $OutDirs.Lib + "\SLD.Core.lib"
    Dll = $OutDirs.Bin + "\SLD.Core.dll"
}

$CompileArgs = [PSCustomObject]@{
    Compiler = "cl.exe"
    In       = ".\src\sld.cpp"
    Out      = "/Fo:" + $OutFiles.Obj
    Include  = @(
        "/Isrc"
        "/Iinclude"
	"/I3rd-party"
    ) -join " "
    Flags = @(
        "/nologo"             # startup banner disabled
        "/c"                  # compile without linking
        "/MD"                 # link against multithreaded runtime library (MSVCRT.dll)
        "/Z7"                 # generate debug info
        "/EHs-"               # disable exception handling
        "/std:c++17"          # c++17 standard mode
        "/Od"                 # disable optimizations
        "/D_HAS_EXCEPTIONS=0" # disable exceptions for STL and CRT
    ) -join " "    
}

$LinkArgs = [PSCustomObject]@{
    Linker = "link.exe"
    In     = @(
        "SLD.Core.obj"
        "user32.lib"
    ) -join " "
    Out = @(
        "/OUT:build\debug\bin\SLD.Core.dll"
        "/IMPLIB:build\debug\lib\SLD.Core.lib"
    ) -join " "
    Include = @(
        "/LIBPATH:build\debug\obj"
        "/LIBPATH:build\debug\lib"
    ) -join " "
    Flags = @(
        "/nologo"
        "/SUBSYSTEM:WINDOWS"
        "/DEBUG"
        "/DLL"
    ) -join " "
}

$Expressions = [PSCustomObject]@{
    Compile = @(
        $CompileArgs.Compiler
        $CompileArgs.In
        $CompileArgs.Out
        $CompileArgs.Include
        $CompileArgs.Flags
    ) -join " "
    Link = @(
        $LinkArgs.Linker
        $LinkArgs.In
        $LinkArgs.Out
        $LinkArgs.Include
        $LinkArgs.Flags
    ) -join " "
}

$null = New-Item -ItemType Directory -Path $OutDirs.Bin -Force
$null = New-Item -ItemType Directory -Path $OutDirs.Obj -Force
$null = New-Item -ItemType Directory -Path $OutDirs.Lib -Force

Invoke-Expression $Expressions.Compile
Invoke-Expression $Expressions.Link

$CompileCommands = [PSCustomObject]@{
    directory = ".\"
    command   = $Expressions.Compile
    file      = ".\src\sld.cpp" 
}

$CompileCommands | ConvertTo-Json -Depth 20 | Set-Content -Path "compile_commands.json"

