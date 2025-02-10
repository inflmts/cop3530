project(
  'quiz04',
  'cpp',
  default_options: [
    'cpp_std=c++14',
    'prefer_static=true'])

bridges_dir = get_option('bridges_dir')
assert(bridges_dir != '')
bridges_inc = include_directories(bridges_dir / 'include')
bridges_libdir = bridges_dir / 'lib'
bridges = declare_dependency(
  include_directories: bridges_inc,
  link_args: ['-L' + bridges_libdir, '-lcurl'])

executable(
  'imdbtree',
  'imdbtree.cpp',
  dependencies: bridges)
