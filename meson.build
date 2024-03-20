project('boiler', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++23']
)

add_project_link_arguments('-lstdc++_libbacktrace', '-lstdc++exp', language: 'cpp')

app_deps = [
  dependency('fmt'),
]

app_includes = [
  include_directories('include')
]

app_sources = [
  'src/app/app.cc'
]

app = library('app',
  app_sources,
  install: true,
  include_directories: [app_includes],
  dependencies: [app_deps]
)

exe = executable(
  'app',
  'src/main.cc',
  include_directories: [app_includes],
  install: true,
  link_with: [app]
)
