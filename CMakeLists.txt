cmake_minimum_required(VERSION 3.10)
project(WinFtp)
set(CMAKE_CXX_STANDARD 14)
set(app_WINRC UI/app.rc)
set_property(SOURCE UI/app.rc APPEND PROPERTY
        OBJECT_DEPENDS
        )

##调用的机制是这样的，会直接以命令的形式自动调用moc uic rcc
##那么这三个软件一定要在环境变量中，这也就是qt需要进入系统环境变量的原因。
set(CMAKE_EXE_LINKER_FLAGS -static)  # 静态部署
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.13.0\\5.13.0\\mingw73_64\\lib\\cmake")
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
add_executable(WinFtp main.cpp  ${app_WINRC}
        UI/mainwindow.cpp UI/mainwindow.h ftpsock/login.cpp
        ftpsock/login.h ftpsock/ftpdata.cpp ftpsock/ftpdata.h
        utils/utils.cpp utils/utils.h
        utils/commands.cpp  utils/commands.h ftpsock/upload.cpp  UI/login.h  UI/login.cpp
        ftpsock/upload.h utils/ErrorCode.cpp utils/ErrorCode.h utils/resource.h UI/MainWindow/LocalEnd/localMenuSetup.cpp
        UI/MainWindow/LocalEnd/LocalWidgetSetUp.cpp  ftpsock/upload_qThread.cpp ftpsock/upload_qThread.h
        UI/MainWindow/RemoteEnd/RemoteWidgetSetUp.cpp UI/MainWindow/RemoteEnd/RemoteMenuSetup.cpp
        UI/Concurrent/JhProgressBar.h UI/Concurrent/JhButton.cpp UI/Concurrent/JhProgressBar.cpp UI/Concurrent/JhButton.h
        ftpsock/Thread_Upload.cpp  ftpsock/Thread_Download.cpp)
target_link_libraries(WinFtp Qt5::Core)
target_link_libraries(WinFtp Qt5::Widgets)
target_link_libraries(WinFtp wsock32 shlwapi)

