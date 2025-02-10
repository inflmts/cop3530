project(
  'quiz03',
  'cpp',
  default_options: ['cpp_std=c++17'])

catch2 = subproject('catch2').get_variable('catch2_with_main_dep')

executable(
  'quiz03',
  ['solution.cpp', 'test.cpp'],
  dependencies: catch2)
