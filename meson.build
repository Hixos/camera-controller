project('camera_controller_3', 'cpp', default_options : [ 'cpp_std=c++20'])

target = get_option('target')

inc = [include_directories( 'src', 
                            'src/utils/logger', 
                            'src/events', 
                            'lib/json/single_include',
                            'lib/argparse/include'
                            )]

# Sources
main = 'src/main.cpp'

src = [
       'src/camera/CameraWrapper.cpp',
       'src/camera/CameraWidget.cpp',
       'src/utils/logger/PrintLogger.cpp',
       'src/utils/logger/LogSink.cpp',
       'src/utils/logger/TcpLogSink.cpp',
       'src/events/Events.cpp',
       'src/events/EventBase.cpp',
       'src/events/EventBroker.cpp',
       'src/fsm/CameraController.cpp',
       'src/utils/debug/cli.cpp',
       'src/comm/JsonTcpServer.cpp'
       ]

# Test includes
inc_test = [include_directories('tests')]

# Catch tests
catch_main = 'tests/catch_main.cpp'

# List of catch entrypoints
src_catch = ['tests/camerawrapper.cpp']

# Other stand-alone tests
tests = [
              'tests/cli.cpp',
              'tests/event_demo.cpp',
              'tests/camera_controller_fsm.cpp',
              'tests/tcp_log.cpp',
              'tests/server.cpp',
              'tests/move.cpp',
              'tests/json_auto.cpp'
       ]
src_tests = []

# Dependencies
deps = []

deps += dependency('threads')
deps += dependency('libgphoto2')
deps += dependency('libgphoto2_port')

deps += dependency('fmt')

if meson.is_cross_build()
       sysroot = meson.get_cross_property('sys_root', '')
       deps += meson.get_compiler('cpp').find_library('scn', dirs: sysroot + '/usr/local/lib/arm-linux-gnueabihf/')
       deps += meson.get_compiler('cpp').find_library('sockpp', dirs: sysroot + '/usr/local/lib/arm-linux-gnueabihf/')
else
       deps += dependency('scn')
       deps += meson.get_compiler('cpp').find_library('sockpp')
endif


executable('cc3', [main, src], include_directories : inc, dependencies : deps)

if target=='test' or target=='all'
       # Catch
       # executable('catch', [catch_main, src, src_catch], include_directories : [inc, inc_test], dependencies : deps)


       # Generic tests
       foreach m  : tests
              name = m.split('/')[-1].split('.')[0]
              executable('test_' + name, [m, src, src_tests],  include_directories : [inc, inc_test], 
                     dependencies : deps)
       endforeach    
endif
