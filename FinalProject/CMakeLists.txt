cmake_minimum_required(VERSION 3.5)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

#set( CMAKE_BUILD_TYPE Release )
set( CMAKE_BUILD_TYPE Debug )

message( "build mode is: ${CMAKE_BUILD_TYPE}" )

project(bptree)

#find_package( OpenCV REQUIRED core imgproc highgui )
find_package( OpenCV REQUIRED )
find_package( Threads REQUIRED )

#Messaging while compiling
	if ( WIN32 )
		message( "CONFIGURING FOR WINDOWS" )
	else()
		message( "CONFIGURING FOR LINUX" )	
	endif()

	message( "Configuring main application: " )
#end messaging

file(GLOB SOURCES "*.cpp" )
file(GLOB HEADERS "*.h" )

include_directories( ${OpenCV_INCLUDE_DIRS} )
 
add_executable( ${PROJECT_NAME}  
				${HEADERS} 
				${SOURCES})
				
target_link_libraries( ${PROJECT_NAME} 
                       ${OpenCV_LIBS} )
