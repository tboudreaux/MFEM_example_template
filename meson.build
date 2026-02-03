project('mfem_test', ['cpp', 'c'], version: 'v0.1.0', default_options: ['cpp_std=c++23'], meson_version: '>=1.5.0')

add_project_arguments('-fvisibility=default', language: 'cpp')

subdir('build-config')

executable('main', 'main.cpp', dependencies: [mfem_dep, cli11_dep, config_dep, composition_dep, gridfire_dep])

