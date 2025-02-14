t() {
  ninja -C build gavl-test.exe && build/gavl-test.exe "$@"
}

g() {
  ninja -C build gavl.exe && build/gavl.exe "$@"
}
