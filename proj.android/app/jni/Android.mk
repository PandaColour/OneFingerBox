LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AddBoxProp.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/AStar.cpp \
                   $(LOCAL_PATH)/../../../Classes/Audio.cpp \
                   $(LOCAL_PATH)/../../../Classes/Barrier.cpp \
                   $(LOCAL_PATH)/../../../Classes/BigTree.cpp \
                   $(LOCAL_PATH)/../../../Classes/Blood.cpp \
                   $(LOCAL_PATH)/../../../Classes/Bomb.cpp \
                   $(LOCAL_PATH)/../../../Classes/BottomFrame.cpp \
                   $(LOCAL_PATH)/../../../Classes/Box.cpp \
                   $(LOCAL_PATH)/../../../Classes/BugMonster.cpp \
                   $(LOCAL_PATH)/../../../Classes/BugTestMonster.cpp \
                   $(LOCAL_PATH)/../../../Classes/Cherry.cpp \
                   $(LOCAL_PATH)/../../../Classes/CountryLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/Cross.cpp \
                   $(LOCAL_PATH)/../../../Classes/DataStream.cpp \
                   $(LOCAL_PATH)/../../../Classes/FloatScore.cpp \
                   $(LOCAL_PATH)/../../../Classes/Flower.cpp \
                   $(LOCAL_PATH)/../../../Classes/GameApp.cpp \
                   $(LOCAL_PATH)/../../../Classes/GameScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/GuidLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/Hander.cpp \
                   $(LOCAL_PATH)/../../../Classes/Hero.cpp \
                   $(LOCAL_PATH)/../../../Classes/LevelButton.cpp \
                   $(LOCAL_PATH)/../../../Classes/LevelScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/lv.cpp \
                   $(LOCAL_PATH)/../../../Classes/MainScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/Monster.cpp \
                   $(LOCAL_PATH)/../../../Classes/PurpleMonster.cpp \
                   $(LOCAL_PATH)/../../../Classes/Redheart.cpp \
                   $(LOCAL_PATH)/../../../Classes/RedMonster.cpp \
                   $(LOCAL_PATH)/../../../Classes/ResultLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/roadStar.cpp \
                   $(LOCAL_PATH)/../../../Classes/score.cpp \
                   $(LOCAL_PATH)/../../../Classes/ScoreLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/SettingScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/Shoes.cpp \
                   $(LOCAL_PATH)/../../../Classes/SmallTree.cpp \
                   $(LOCAL_PATH)/../../../Classes/Stone.cpp \
                   $(LOCAL_PATH)/../../../Classes/ZoneLayer.cpp 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cc_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
