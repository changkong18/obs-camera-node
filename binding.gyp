{
  'targets': [
    {
      'target_name': 'obs',
      'cflags': [ '-fno-exceptions', '-fPIC' ],
      'cflags_cc': [ '-fno-exceptions', '-fPIC' ],
      'sources': [
        '<!@(ls -1 external/libyuv/source/*.cc)'
      ],
      'conditions': [
        ['OS == "mac"', {
          'sources': [
            'native/macos/main.mm',
            'native/macos/server/OBSDALMachServer.mm'
          ],
          'link_settings': {
            "libraries": [
              'Foundation.framework',
              'CoreVideo.framework',
            ],
          },
        }],
        ['OS == "win"', {
          'sources': [
            'native/windows/main.cpp',
            'native/windows/queue/shared-memory-queue.c'
          ],
          'link_settings': {
            "libraries": [
              '/DEFAULTLIB:advapi32.lib',
            ],
          },
        }],
      ],
      'include_dirs': [
        "<!@(node -p \"require('node-addon-api').include\")",
        './external/libyuv/include'
      ],
      'defines': [ 
        'NAPI_DISABLE_CPP_EXCEPTIONS'
      ],
    }
  ]
}