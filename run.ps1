$a = $args[0]
$file = "stek.cpp"
$out = "build/stek.exe"
g++ $file -o $out
./build/stek.exe $a