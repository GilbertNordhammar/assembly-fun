$SOLUTION_DIR = "AssemblyFun"
$TARGET = $args[0]
$EXE_PATH = $args[1]

if ( ("$TARGET" -ne "Debug") -and ("$TARGET" -ne "Release") )
{
    Write-Output "ERROR: Need to pass build target 'Debug' or 'Release'"
    return
}

if ("$EXE_PATH" -eq "")
{
    Write-Output "ERROR: Must path to .exe"
    return
}

& cmake --build build --target data-oriented-design --config $TARGET
# & "./$SOLUTION_DIR/$TARGET/$PROJECT.exe"
& "./AssemblyFun/$EXE_PATH"
