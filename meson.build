project('nvidia-fand', 'cpp',
  version : '0.1',
  default_options : ['warning_level=everything', 'cpp_std=c++26']
)

add_project_link_arguments('-lstdc++exp', '-lstdc++exp', language: 'cpp')

app_deps = [
  # app dependencies
  # <print> got implemented on GCC 14 so fmt is out
  dependency('nlohmann_json'),
  dependency('nvidia-ml')
]

app_includes = [
  include_directories('include')
]

app_sources = [
  'src/app/app.cc'
]

exe = executable(
  'nvidia-fand',
  [app_sources, 'src/main.cc'],
  include_directories: [app_includes],
  dependencies: [app_deps],
  install: true
)
