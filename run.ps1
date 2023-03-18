$source_dir = "."
$build_dir = "build"
$target = $args[0]

if ( ("$target" -ne "Debug") -and ("$target" -ne "Release") )
{
    Write-Output "ERROR: Need to pass build target 'Debug' or 'Release'"
    return
}

& cmake --build build --target data-oriented-design --config $target
& "./$build_dir/$target/data-oriented-design.exe"
