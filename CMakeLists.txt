#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

set(APP_NAME OneFingerBox)

project(${APP_NAME})

set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )
if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

# add cross-platforms source files and header files 
list(APPEND GAME_SOURCE
     Classes/AdmobHelper.cpp
     Classes/AddBoxProp.cpp
     Classes/AppDelegate.cpp
     Classes/AStar.cpp
     Classes/Audio.cpp
     Classes/Barrier.cpp
     Classes/BigTree.cpp
     Classes/Blood.cpp
     Classes/Bomb.cpp
     Classes/BottomFrame.cpp
     Classes/Box.cpp
     Classes/BugMonster.cpp
     Classes/BugTestMonster.cpp
     Classes/Cherry.cpp
     Classes/CountryLayer.cpp
     Classes/Cross.cpp
     Classes/DataStream.cpp
     Classes/FloatScore.cpp
     Classes/Flower.cpp
     Classes/GameApp.cpp
     Classes/GameScene.cpp
     Classes/GuidLayer.cpp
     Classes/Hander.cpp
     Classes/Hero.cpp
     Classes/LevelButton.cpp
     Classes/LevelScene.cpp
     Classes/lv.cpp
     Classes/MainScene.cpp
     Classes/Monster.cpp
     Classes/PurpleMonster.cpp
     Classes/Redheart.cpp
     Classes/RedMonster.cpp
     Classes/ResultLayer.cpp
     Classes/roadStar.cpp
     Classes/score.cpp
     Classes/ScoreLayer.cpp
     Classes/SettingScene.cpp
     Classes/Shoes.cpp
     Classes/SmallTree.cpp
     Classes/Stone.cpp
     Classes/ZoneLayer.cpp
     )
list(APPEND GAME_HEADER
     Classes/AdmobHelper.h
     Classes/AddBoxProp.h
     Classes/AppDelegate.h
     Classes/AStar.h
     Classes/Audio.h
     Classes/Barrier.h
     Classes/BigTree.h
     Classes/Blood.h
     Classes/Bomb.h
     Classes/BottomFrame.h
     Classes/Box.h
     Classes/BugMonster.h
     Classes/BugTestMonster.h
     Classes/Cherry.h
     Classes/CountryLayer.h
     Classes/Cross.h
     Classes/DataStream.h
     Classes/FloatScore.h
     Classes/Flower.h
     Classes/GameApp.h
     Classes/GameScene.h
     Classes/GuidLayer.h
     Classes/Hander.h
     Classes/Hero.h
     Classes/LevelButton.h
     Classes/LevelScene.h
     Classes/lv.h
     Classes/MainScene.h
     Classes/Monster.h
     Classes/PurpleMonster.h
     Classes/Redheart.h
     Classes/RedMonster.h
     Classes/ResultLayer.h
     Classes/roadStar.h
     Classes/score.h
     Classes/ScoreLayer.h
     Classes/SettingScene.h
     Classes/Shoes.h
     Classes/SmallTree.h
     Classes/Stone.h
     Classes/TBase.h
     Classes/tdaf.h
     Classes/ZoneLayer.h
     )

if(ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
         proj.android/app/jni/hellocpp/main.cpp
         )
elseif(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         proj.win32/resource.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         proj.win32/game.rc
         ${common_res_files}
         )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             proj.ios_mac/ios/AppController.h
             proj.ios_mac/ios/RootViewController.h
             )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/ios/main.m
             proj.ios_mac/ios/AppController.mm
             proj.ios_mac/ios/RootViewController.mm
             proj.ios_mac/ios/Prefix.pch
             ${APP_UI_RES}
             )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/mac/main.cpp
             proj.ios_mac/mac/Prefix.pch
             ${APP_UI_RES}
             )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )
if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
endif()

target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

# mark app resources
setup_cocos_app_config(${APP_NAME})
if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    if(MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES
                              MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist"
                              )
    elseif(IOS)
        cocos_pak_xcode(${APP_NAME} INFO_PLIST "iOSBundleInfo.plist.in")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    endif()
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME} COPY_TO ${APP_RES_DIR}/..)
endif()

if(LINUX OR WINDOWS)
    cocos_copy_res(COPY_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()
