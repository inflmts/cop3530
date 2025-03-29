project('project2', 'cpp',
  default_options: ['cpp_std=c++14', 'warning_level=1'])

catch2 = dependency('catch2-with-main')

executable('p2', 'pagerank.cpp')
executable('p2test', 'test.cpp', dependencies: catch2)

maketest = find_program('maketest.sh')
custom_target(
  output: 'test.md',
  input: 'test.cpp',
  command: [maketest, '@INPUT0@', '@OUTPUT0@'],
  build_by_default: true)
