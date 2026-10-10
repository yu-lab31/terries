project(
  'terries',
  'c',
  meson_version : '>= 1.3.0',
  version : 'v0.0.1-beta.3',
  default_options : [
    'warning_level=3',
    'c_std=c2x',
  ],
)

inc_root = include_directories('.')
pthread = dependency('threads')

sources = [
  'arithmetic/arithmetic.c',
  'include/trapno.c'
]

static_link_args = [
  '-static',
  '-static-libgcc',
  '-Wl,-Bstatic,--whole-archive',
]

windows_link_args = [
  '-lwinpthread',
]

dynamic_link_args = [
  '-Wl,-Bdynamic,--no-whole-archive'
]

cross_build_args = []
if host_machine.system() == 'windows'
  cross_build_args = static_link_args + windows_link_args + dynamic_link_args
endif

shared_library(
  'arithmetic',
  sources,
  include_directories: inc_root,
  dependencies: [pthread],
  link_args: cross_build_args
)

arithmetic_test = executable(
  'arithmetic_test',
  sources + [ 'test_case/arithmetic_test.c' ],
  include_directories: inc_root,
  dependencies: [pthread],
  native: true
)

test('arithmetic test', arithmetic_test)
