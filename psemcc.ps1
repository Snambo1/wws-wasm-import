param (
  [string]$script,
  [string]$local
)


if ($script -eq "") {
  Write-Output Example use
  Write-Output ./pub.bat scriptFile.c
  exit
}
$apiKey = 0
if ($script -eq "space-game.c") {
  if ($local -eq "local") {
    $apiKey = "85lyroy9apcxtfbvhfqkkpad5v2qg7yvf4sgi8rrnioirqppln"
  }
  else {
    $apiKey = 0
  }
}
elseif ($script -eq "whatever.c") {
  if ($local -eq "local") {
    $apiKey = 0
  }
  else {
    $apiKey = 0
  }
}


if ($apiKey -eq "") {
  Write-Output "apiKey not assigned for $script"
  exit
}
  


if ($local -eq "local") {
  $server = "https://localhost:7086/api/Wasm/Upload/"
}
else {
  $server = "https://www.d1ag0n.com/api/Wasm/Upload/"
}
    


emcc "$script" -I header  -O3 --no-entry -s WASM=1 -s ENVIRONMENT=web -s ERROR_ON_UNDEFINED_SYMBOLS=0 '-Wl,--export=__heap_base'  "-o $script.wasm"

if ($LastExitCode -eq 0) {

  if ($local -eq "local") {
    curl -k --fail -F "data=@./$script.wasm" "$server$apiKey"
  }
  else {
    curl --fail -F "data=@./$script.wasm" "$server$apiKey"
  }
}
   

if ($LastExitCode -eq 0) {
      
  Write-Output $script.wasm uploaded to $server
} 
    