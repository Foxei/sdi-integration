set(SOURCE_FILES ${QT_RES}
    main.cpp
    model/Data.cpp
    view/Mainwindow.cpp
    view/sidebar/SideBar.cpp
    view/sidebar/CollapsibleElement.cpp
    view/control/Controlbar.cpp
    controller/Controller.cpp
    view/status/StatusBar.cpp
    controller/Worker.cpp
    view/chart/ChartView.cpp
    view/sidebar/PopupMessage.cpp)

add_executable(IntegrationGui ${SOURCE_FILES})

target_link_libraries(IntegrationGui integration_lib)
target_link_libraries(IntegrationGui Qt5::Core)
target_link_libraries(IntegrationGui Qt5::Widgets)
target_link_libraries(IntegrationGui Qt5::Gui)
target_link_libraries(IntegrationGui Qt5::Charts)