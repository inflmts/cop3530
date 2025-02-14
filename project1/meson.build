project('gavl', 'cpp',
  default_options: ['cpp_std=c++14'])

add_project_arguments('-DGAVL_EXT', language: 'cpp')

catch2 = subproject('catch2').get_variable('catch2_with_main_dep')

sources = [
  'avl.cpp',
  'shell.cpp'
]

libgavl = shared_library('gavl', sources)

executable('gavl',
  'main.cpp',
  link_with: libgavl)

executable('gavl-test',
  'test.cpp',
  link_with: libgavl,
  dependencies: [catch2])
